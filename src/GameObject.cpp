#include "GameObject.h"


GameObject::GameObject()
{
	transform = &AddComponent<Transform>();
}

GameObject::GameObject(const std::string& _name)
{
	name = _name;
	transform = &AddComponent<Transform>();
}

GameObject::GameObject(const GameObject& gameObject)
{
	name = gameObject.name;
}

GameObject* GameObject::Instantiate()
{
	GameObject* go = new GameObject();
	return go;
}

GameObject* GameObject::Instantiate(const GameObject& gameObject)
{
	GameObject* go = new GameObject(gameObject);
	return go;
}	