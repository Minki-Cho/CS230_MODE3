/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Apple.cpp
Project: CS230
Author: Minki Cho, Minjae Yu
Creation date: 6/12/2022
-----------------------------------------------------------------*/
#include "Apple.h" //Apple
#include "GameObjectTypes.h" //return GameObjectTypes
#include "../Engine/Collision.h" //removeCollision
#include "Score.h" //Add Score
#include "../Engine/Engine.h" //GetGameStateManager
#include "../Engine/GameObjectManager.h" // GameObjectManager
#include "GameParticles.h" // Particle
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
		//Particle
		math::vec2 vectorToObject = objectB->GetPosition() - GetPosition();
		math::vec2 collisionPoint = vectorToObject.Normalize() * GetGOComponent<CS230::CircleCollision>()->GetRadius() + GetPosition();

		math::vec2 laserVelocity = objectB->GetVelocity();
		math::vec2 emitVector = vectorToObject.Normalize() * 2 + laserVelocity.Normalize();
		Engine::GetGameStateManager().GetGSComponent<StarBitEmitter>()->Emit(10, collisionPoint, GetVelocity(), emitVector * 50, 3.141592 / 2);

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
