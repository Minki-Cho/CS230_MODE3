#include "Apple.h" //Apple
#include "GameObjectTypes.h" //return GameObjectTypes
#include "../Engine/Collision.h" //removeCollision
#include "Score.h" //Add Score
#include "../Engine/Engine.h" //GetGameStateManager
Apple::Apple() : GameObject({ 0,0 })
{
	AddGOComponent(new CS230::Sprite("Assets/Final/apple.spt", this));
	
	SetPosition(math::vec2{ random_helper(400.0, 1000.0) ,random_helper(160.0, 760.0) });
}

GameObjectType Apple::GetObjectType()
{
	return GameObjectType::Apple;
}

std::string Apple::GetObjectTypeName()
{
	return "Apple";
}

void Apple::ResolveCollision(GameObject* objectB)
{
	if (objectB->GetObjectType() == GameObjectType::Apple)
	{
		Engine::GetGameStateManager().GetGSComponent<Score>()->AddScore(100);
		RemoveGOComponent<CS230::Collision>();
		RemoveGOComponent<CS230::Sprite>();
		//Isdead = true;
		AddGOComponent(new CS230::Sprite("Assets/Final/apple.spt", this));
		SetPosition(math::vec2{ static_cast<double>(rand() % 700),static_cast<double>(rand() % 700) });
	}
}

void Apple::Update(double dt)
{
	GameObject::Update(dt);
}
