#include "Apple.h" //Apple
#include "GameObjectTypes.h" //return GameObjectTypes
#include "../Engine/Collision.h" //removeCollision
#include "Score.h" //Add Score
#include "../Engine/Engine.h" //GetGameStateManager
#include "../Engine/GameObjectManager.h"
Apple::Apple() : GameObject({ 0,0 }), random_val({0,0})
{
	random_val = { random_helper(400.0, 1000.0),random_helper(160.0, 760.0) };
	AddGOComponent(new CS230::Sprite("Assets/Final/apple.spt", this));
	SetPosition(math::vec2{ random_val.x ,random_val.y });
}

GameObjectType Apple::GetObjectType()
{
	return GameObjectType::Apple;
}

std::string Apple::GetObjectTypeName()
{
	return "Apple";
}

bool Apple::CanCollideWith(GameObjectType objectBType)
{
	if (objectBType == GameObjectType::BumperCar)
	{
		return true;
	}
	return false;
}

void Apple::ResolveCollision(GameObject* objectB)
{
	if (objectB->GetObjectType() == GameObjectType::BumperCar)
	{
		RemoveGOComponent<CS230::Collision>();
		RemoveGOComponent<CS230::Sprite>();
		SetDestroyed(true);
		Engine::GetGSComponent<CS230::GameObjectManager>()->Add(new Apple);
	}
}

void Apple::Update(double dt)
{
	GameObject::Update(dt);
}
