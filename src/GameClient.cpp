#include "GameClient.h"
#include "NetTransform.h"
#include "GamePacket.h"
#include "NetworkManager.h"
#include "SceneFactory.h"
#include "Application.h"

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
			short id = -1;
			gamePacket >> id;

			std::string sceneName;
			Packet& packet = static_cast<Packet&>(gamePacket);

			packet >> sceneName;

			if (!NetworkManager::Instance().HasNetEntity(id))
			{
				std::shared_ptr<Scene> scene = SceneFactory::Instance().GetScene(sceneName);

				if (scene != nullptr)
				{
					// Make an entity here
					std::unique_ptr<NetEntity>* netEntity = MakeEntity(scene);

					if (netEntity)
					{
						gamePacket >> *netEntity->get();

						if (Application::HasScene(scene))
						{
							for (const std::unique_ptr<NetEntity>& entity : pendingEntities[&scene])
							{
								scene->AddNetEntity(std::move(entity.get()));
							}
							pendingEntities.erase(&scene);
						}
					}
				}
			}
			break;

		}

		case PacketType::PT_ENTITY_CHANGE: // Entity exists, and we ar emaking changes to new/preexisting components
		{
			short id = -1;
			gamePacket >> id;

			std::unique_ptr<NetEntity>* entity = NetworkManager::Instance().GetNetEntity(id);

			if (entity != nullptr)
			{
				gamePacket >> **entity;
			}

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

std::unique_ptr<NetEntity>* GameClient::MakeEntity(std::shared_ptr<Scene> scene)
{
	std::unique_ptr<NetEntity> entityPtr { NetEntity::Instantiate(scene) };

	if (pendingEntities.count(&scene) > 0)
		pendingEntities[&scene].push_back(std::move(entityPtr));

	else
	{
		std::vector<std::unique_ptr<NetEntity>> entities = {};
		pendingEntities.emplace(&scene, std::move(entities));

		pendingEntities[&scene].push_back(std::move(entityPtr));
	}
		

	return &pendingEntities[&scene].front();
}

void GameClient::RequestEntityInstatiate(std::unique_ptr<NetEntity>* entityPtr)
{
	std::shared_ptr<GamePacket> entityPacket = std::make_shared<GamePacket>(PacketType::PT_ENTITY_INSTANTIATE);

	NetEntity& entity = *entityPtr->get();

	*entityPacket << entity.GetScene()->GetName();
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
