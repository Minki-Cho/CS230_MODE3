/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Laser.cpp
Project: CS230
Author: MINJAE YU
Creation date: 5/23/2022
-----------------------------------------------------------------*/
#include "Laser.h" //Laser
#include "..\Engine\Engine.h" //GetWindow

Laser::Laser(math::vec2 pos, double rotation, math::vec2 scale, math::vec2 laserVelocity)
	: GameObject(pos, rotation, scale)
{
	AddGOComponent(new CS230::Sprite("Assets/Laser.spt", this));

	SetVelocity(laserVelocity);
}

void Laser::Update(double dt)
{
	GameObject::Update(dt);

	if (GetPosition().x < 0 || GetPosition().x > Engine::GetWindow().GetSize().x)
	{
		SetDestroyed(true);
	}
	if (GetPosition().y < 0 || GetPosition().y > Engine::GetWindow().GetSize().y)
	{
		SetDestroyed(true);
	}
}

bool Laser::CanCollideWith(GameObjectType objectBType)
{
	if (objectBType == GameObjectType::Meteor || objectBType == GameObjectType::EnemyShip)
	{
		return true;
	}
	return false;
}

void Laser::ResolveCollision(GameObject* objectB)
{
	objectB->ResolveCollision(this);
	SetDestroyed(true);
}