/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: EnemyShip.cpp
Project: CS230
Author: MINJAE YU
Creation date: 6/14/2021
-----------------------------------------------------------------*/
#include "EnemyShip.h" //EnemyShip
#include "../Engine/Engine.h" //GetGSComponent
#include "../Engine/Collision.h" //Collision
#include "../Engine/ShowCollision.h" //ShowCollision
#include "Flame_Anims.h" //Flame Animation
#include "GameObjectTypes.h" //GameObjectTypes
#include "Ship_Anims.h" //Ship Animation
#include "Score.h" //Add Score

EnemyShip::EnemyShip(GameObject* player) : player(player), GameObject(math::vec2{}, 0, math::vec2{ 0.75,0.75 }),
flameLeft("Assets/Flame.spt", this), flameRight("Assets/Flame.spt", this), isDead(false)
{
	AddGOComponent(new CS230::Sprite("Assets/enemyShip.spt", this));
	SetRotation(GetRandom(0, 2 * PI));
	SetPosition(math::vec2{ GetRandom(player->GetPosition().x - 1024, player->GetPosition().x - 2048),
		GetRandom(player->GetPosition().y - 1024, player->GetPosition().y - 2048) });
	flameLeft.PlayAnimation(static_cast<int>(Flame_Anim::Flame_Anim));
	flameRight.PlayAnimation(static_cast<int>(Flame_Anim::Flame_Anim));
}

void EnemyShip::Update(double dt)
{
	if (isDead == false)
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
		UpdateVelocity(math::RotateMatrix(GetRotation()) * math::vec2{ 0,accel * dt });
	}
	UpdateVelocity(-(GetVelocity() * drag * dt));

	flameLeft.Update(dt);
	flameRight.Update(dt);

	UpdatePosition(GetVelocity() * dt);
	UpdateGOComponents(dt);
}

void EnemyShip::Draw(math::TransformMatrix displayMatrix)
{
	if (isDead == false)
	{
		flameLeft.Draw(displayMatrix * GetMatrix() * math::TranslateMatrix(GetGOComponent<CS230::Sprite>()->GetHotSpot(1)));
		flameRight.Draw(displayMatrix * GetMatrix() * math::TranslateMatrix(GetGOComponent<CS230::Sprite>()->GetHotSpot(2)));
	}
	GetGOComponent<CS230::Sprite>()->Draw(displayMatrix * GetMatrix());
	if (Engine::GetGSComponent<ShowCollision>() != nullptr)
	{
		if (Engine::GetGSComponent<ShowCollision>()->IsEnabled() == true)
		{
			if (GetGOComponent<CS230::Collision>() != nullptr)
				GetGOComponent<CS230::Collision>()->Draw(displayMatrix);
		}
	}
}

GameObjectType EnemyShip::GetObjectType()
{
	return GameObjectType::EnemyShip;
}

std::string EnemyShip::GetObjectTypeName()
{
	return "EnemyShip";
}

bool EnemyShip::CanCollideWith(GameObjectType collideAgainstType)
{
	if (collideAgainstType == GameObjectType::Laser || collideAgainstType == GameObjectType::Ship)
	{
		return true;
	}
	return false;
}

void EnemyShip::ResolveCollision(GameObject* collidedWith)
{
	if (collidedWith->GetObjectType() == GameObjectType::Laser)
	{
		isDead = true;
		GetGOComponent<CS230::Sprite>()->PlayAnimation(static_cast<int>(Ship_Anim::Explode_Anim));
		RemoveGOComponent<CS230::Collision>();
		Engine::GetGameStateManager().GetGSComponent<Score>()->AddScore(300);
	}
}

double EnemyShip::GetRandom(double min, double max)
{
	return min + rand() / (RAND_MAX / (max - min));
}