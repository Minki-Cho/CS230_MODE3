/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: EnemyCar.cpp
Project: CS230
Author: Minki Cho, Minjae Yu
Creation date: 6/12/2022
-----------------------------------------------------------------*/
#include "EnemyCar.h" //EnemyCar
#include "../Engine/Engine.h" //GetGSComponent
#include "../Engine/Collision.h" //Collision
#include "../Engine/ShowCollision.h" //ShowCollision
#include "Flame_Anims.h" //Flame Animation
#include "GameObjectTypes.h" //GameObjectTypes
#include "EnemyCar_Anim.h" //Car Animation
#include "GameParticles.h"

EnemyCar::EnemyCar(GameObject* player, math::vec2 position, std::vector<double> moveableNodes, EnemyCarType type) : position(position), GameObject(math::vec2{}, 0,
	math::vec2{ 0.75,0.75 }), changeDir(false), moveableNodes(moveableNodes), type(type), player(player)
{
	AddGOComponent(new CS230::Sprite("Assets/Final/slasher.spt", this));
	SetPosition(math::vec2{ position });
	GetGOComponent<CS230::Sprite>()->PlayAnimation(static_cast<int>(EnemyCar_Anim::Rot_Anim));
}

EnemyCar::EnemyCar(GameObject* player, EnemyCarType type) :player(player), position(math::vec2{}), GameObject(math::vec2{}, 0, math::vec2{ 0.75,0.75 }),
changeDir(false), type(type)
{
	AddGOComponent(new CS230::Sprite("Assets/Final/slasher.spt", this));
	SetPosition({ static_cast<double>(Engine::GetWindow().GetSize().x), static_cast<double>(Engine::GetWindow().GetSize().y - 50.0) });
}

void EnemyCar::Update(double dt)
{
	if (type == EnemyCarType::Chasing)
	{
		math::vec2 facingVector = (math::RotateMatrix(GetRotation()) * math::vec2{ 0,1 }).Normalize();
		math::vec2 playerVector = (player->GetPosition() - GetPosition()).Normalize();
		if (facingVector.Cross(playerVector) >= 0.05)
		{
			UpdateRotation(rotationRate * dt);
		}
		if (facingVector.Cross(playerVector) <= -0.05)
		{
			UpdateRotation(-rotationRate * dt);
		}
		UpdateVelocity(-(GetVelocity() * drag * dt));
		UpdateVelocity(math::RotateMatrix(GetRotation()) * math::vec2{ 0,accel * dt });
		UpdatePosition(GetVelocity() * dt);
		UpdateGOComponents(dt);
	}

	if (type == EnemyCarType::UpDown)
	{
		UpdatePosition({ speed.x * dt,speed.y * dt });
		if (GetPosition().x > moveableNodes[0])
		{
			changeDir = true;
		}
		else if (GetPosition().x < moveableNodes[1])
		{
			changeDir = true;
		}
		if (changeDir == true)
		{
			speed.x = -speed.x;
			speed.y = -speed.y;
			changeDir = false;
		}
	}
}

GameObjectType EnemyCar::GetObjectType()
{
	return GameObjectType::EnemyCar;
}

std::string EnemyCar::GetObjectTypeName()
{
	return "EnemyCar";
}

bool EnemyCar::CanCollideWith(GameObjectType collideAgainstType)
{
	if (collideAgainstType == GameObjectType::BumperCar)
	{
		return true;
	}
	return false;
}

void EnemyCar::ResolveCollision(GameObject* collidedWith)
{
	if (collidedWith->GetObjectType() == GameObjectType::BumperCar)
	{
		math::vec2 vectorToObject = collidedWith->GetPosition() - GetPosition();
		math::vec2 collisionPoint = vectorToObject.Normalize() * GetGOComponent<CS230::CircleCollision>()->GetRadius() + GetPosition();
		math::vec2 Velocity = collidedWith->GetVelocity();
		math::vec2 emitVector = vectorToObject.Normalize() * 10 + Velocity.Normalize();
		Engine::GetGameStateManager().GetGSComponent<CarBitEmitter>()->Emit(20, collisionPoint, GetVelocity(), emitVector * 50, PI);

		RemoveGOComponent<CS230::Collision>();
	}
}