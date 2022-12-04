#include "GameClient.h"
#include "NetTransform.h"
#include "GamePacket.h"
#include "NetworkManager.h"
#include "SceneFactory.h"
#include "Application.h"
#include "Logging.h"

// How do I register components for being sent over the network?
// Will this be on a per/component basis?
// Does a component need a "force update" feature?
// Otherwise I'm thinking it needs to be on a certain total
// Also need a way to wrap all entities being changed in packet to avoid sending N entities of data over the network

using namespace GEngine::Networking;

GameClient::GameClient()
{
}

bool GameClient::ConnectToIP(const std::string ip)
{
	if (Connect(ip))
	{
		std::shared_ptr<GamePacket> versionPacket = std::make_shared<GamePacket>(PacketType::PT_VERIFY);
		short version = NetworkManager::Version;
		*versionPacket << version;

		SendPacket(versionPacket);
		return true;
	}
	else
	{
		return false;
	}
}

void GameClient::LeaveSession()
{
	CloseConnection("Session left");
}

GameClient::~GameClient()
{
	if (!NetworkManager::Instance().HasAuthority())
		NetworkManager::Instance().EndSession();
}

/// <summary>
/// Process the packet on the client
/// </summary>
/// <param name="packet"> The packet being processed. </param>
/// <returns> True if the packet was processed successfully. </returns>
bool GameClient::ProcessPacket(std::shared_ptr<Packet> packet)
{
	Logging::INFO("Received Packet.");
	GamePacket& gamePacket = *reinterpret_cast<GamePacket*>(packet.get());
	switch (packet->GetPacketType())
	{
		case PacketType::PT_KICK:
		{
			std::string reason;
			*packet >> reason;

			CloseConnection(reason);
			NetworkManager::Instance().EndSession();

			break;
		}

		case PacketType::PT_SCENE_LOAD:
		{
			std::string sceneName;
			gamePacket >> sceneName;

			Application::OpenScene(sceneName);
			break;
		}

		// Instantiate an entity in a particular scene
		// we are sending packets before we load the scene on the client
		// above is probably the solution to this

		case PacketType::PT_ENTITY_INSTANTIATE:
		{
			short machineID = -1;
			gamePacket >> machineID;

			short clientID = -1;
			gamePacket >> clientID;

			short serverNetID = -1;
			gamePacket >> serverNetID;

			std::string sceneName;
			Packet& packet = static_cast<Packet&>(gamePacket);

			packet >> sceneName;

			if (!NetworkManager::Instance().HasNetEntity(serverNetID))
			{
				std::shared_ptr<Scene> scene = SceneFactory::Instance().GetScene(sceneName);

				if (scene != nullptr)
				{
					if (Application::HasScene(scene))
					{
						NetEntity* netEntity = MakeEntity(machineID, serverNetID, machineID, scene);
						gamePacket >> *netEntity;

						scene->AddNetEntity(netEntity);
					}
					else
					{
						NetEntity* netEntity = MakeTempEntity(scene);
						gamePacket >> *netEntity;
					}
				}
			}
			break;
		}

		case PacketType::PT_ENTITY_CHANGE: // Entity exists, and we ar emaking changes to new/preexisting components
		{
			short id = -1;
			gamePacket >> id;

			NetEntity* entity = NetworkManager::Instance().GetNetEntity(id);

			if (entity != nullptr)
			{
				gamePacket >> *entity;
			}

			break;
		}

		case PacketType::PT_VERIFY:
		{
			if (NetworkManager::Instance().HasAuthority())
				break;

			gamePacket >> clientID;
			OnVerified();
			break;
		}

		case PacketType::PT_INVALID:
		{
			return false;
		}
		default:
			return false;
	}
	return true;
}

void GameClient::OnConnect()
{
	
}

void GameClient::OnDisconnect(std::string reason)
{
	// Fallback disconnect
	// reason should be an enum I think
	std::cout << reason << std::endl;
	NetworkManager::Instance().EndSession();
}

void GameClient::SendPacket(std::shared_ptr<GamePacket> packet)
{
	if (NetworkManager::Instance().HasAuthority())
	{
		NetworkManager::Instance().GetServer().ProcessLocalPacket(packet);
	}
	else
	{
		connection.pm_outgoing.Append(packet);
	}
}

NetEntity* GameClient::MakeTempEntity(std::shared_ptr<Scene> scene)
{
	short tempID = pendingEntityID++;
	
	NetEntity* entity = NetEntity::Instantiate(tempID, scene);

	std::unique_ptr<NetEntity> entityPtr{ std::move(entity) };

	if (pendingEntities.count(scene) == 0)
	{
		std::unordered_map<short, std::unique_ptr<NetEntity>> entities = {};
		pendingEntities.emplace(scene, std::move(entities));
	}

	pendingEntities[scene].emplace(tempID, std::move(entityPtr));

	return pendingEntities[scene][tempID].get();
}

NetEntity* GameClient::MakeEntity(short machineID, short serverID, short clientID, std::shared_ptr<Scene> scene)
{
	NetEntity* entity = nullptr;

	if (machineID == GetClientID())
	{
		if (pendingEntities.count(scene) > 0 && pendingEntities[scene].count(clientID) > 0)
		{
			auto it = pendingEntities[scene].find(clientID);

			if (it != pendingEntities[scene].end())
			{
				entity = it->second.release();
				pendingEntities[scene].erase(clientID);

				if (pendingEntities[scene].size() == 0)
					pendingEntities.erase(scene);
			}
		}
	}

	if (entity == nullptr)
		entity = NetEntity::Instantiate(serverID, scene);
	

	std::unique_ptr<NetEntity>* entityPtr = scene->AddNetEntity(entity);
	return entityPtr->get();

}

void GameClient::OnVerified()
{
	m_isVerified = true;

	for (const auto& element : pendingEntities) 
	{
		if (Application::HasScene(element.first))
		{
			for (const auto& entity : element.second)
			{
				RequestEntityInstatiate(entity.second.get());
			}
		}
	}

	pendingEntities.clear();
}

void GameClient::RequestEntityInstatiate(NetEntity* entityPtr)
{
	std::shared_ptr<GamePacket> entityPacket = std::make_shared<GamePacket>(PacketType::PT_ENTITY_INSTANTIATE);
	
	NetEntity& entity = *entityPtr;

	*entityPacket << GetClientID();
	*entityPacket << entity.GetID();

	*entityPacket << entity.GetScene()->GetName();
	*entityPacket << entity;

	SendPacket(entityPacket);
}

void GameClient::RequestEntityChange(NetEntity* entityPtr)
{
	std::shared_ptr<GamePacket> entityPacket = std::make_shared<GamePacket>(PacketType::PT_ENTITY_CHANGE);

	NetEntity& entity = *entityPtr;

	// at some point here we should do something inteligent with componnets here
	*entityPacket << entity;
	SendPacket(entityPacket);
}

bool GameClient::ProcessLocalPacket(std::shared_ptr<Packet> packet)
{
	return ProcessPacket(packet);
}

void GameClient::Tick()
{
	if (IsConnected())
	{
		Frame();
	}
}
