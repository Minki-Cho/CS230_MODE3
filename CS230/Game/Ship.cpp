/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Ship.cpp
Project: CS230
Author: MINJAE YU
Creation date: 3/21/2022
-----------------------------------------------------------------*/
#include "Ship.h" //Ship
#include "../Engine/Engine.h" //logger
#include "Flame_Anims.h" //Get Flame Anim
#include "ScreenWrap.h" //ScreenWrap
#include "../Engine/Collision.h" //GetGOComponent
#include "../Engine/ShowCollision.h" //GetGSComponent
#include "GameObjectTypes.h" //return GameObjectTypes
#include "Ship_Anims.h" // Ship Animation enum
#include "../Engine/GameObjectManager.h" //Get GameObjectManager
#include "Laser.h" //LaserVelocity

Ship::Ship(math::vec2 startPos)
	: GameObject(startPos, 0, math::vec2{ 0.75,0.75 }), rotateCounterKey(CS230::InputKey::Keyboard::A), rotateClockKey(CS230::InputKey::Keyboard::D),
	accelerateKey(CS230::InputKey::Keyboard::W), accelerateReverseKey(CS230::InputKey::Keyboard::S), laserKey(CS230::InputKey::Keyboard::Space),
	flame1("Assets/Flame.spt", this), flame2("Assets/Flame.spt", this), isDead(false)
{
	AddGOComponent(new CS230::Sprite("Assets/Ship.spt", this));
	AddGOComponent(new ScreenWrap(*this));
	GetGOComponent<CS230::Sprite>()->PlayAnimation(static_cast<int>(Ship_Anim::None_Anim));
}

void Ship::Update(double dt)
{
	if (isDead == false)
	{
		if (rotateCounterKey.IsKeyDown())
		{
			UpdateRotation(rotate_speed * dt);
		}
		if (rotateClockKey.IsKeyDown())
		{
			UpdateRotation(-(rotate_speed * dt));
		}
		if (accelerateKey.IsKeyDown())
		{
			UpdateVelocity(math::RotateMatrix(GetRotation()) * math::vec2{ 0,accel * dt });
		}
		if (accelerateReverseKey.IsKeyDown())
		{
			UpdateVelocity(math::RotateMatrix(GetRotation()) * math::vec2{ 0,-accel * dt });
		}

		//check accelerating
		if (accelerateKey.IsKeyDown() && !is_Accelerating)
		{
			is_Accelerating = true;
			Engine::GetLogger().LogDebug("Accelerating");
			flame1.PlayAnimation(static_cast<int>(Flame_Anim::Flame_Anim));
			flame2.PlayAnimation(static_cast<int>(Flame_Anim::Flame_Anim));
		}
		if (accelerateKey.IsKeyReleased())
		{
			Engine::GetLogger().LogDebug("Stopped Accelerating");
			is_Accelerating = false;
			flame1.PlayAnimation(static_cast<int>(Flame_Anim::None_Anim));
			flame2.PlayAnimation(static_cast<int>(Flame_Anim::None_Anim));
		}
		//fire laser
		if (laserKey.IsKeyReleased())
		{
			Engine::GetGameStateManager().GetGSComponent<CS230::GameObjectManager>()->Add(new Laser(GetMatrix() * static_cast<math::vec2>(GetGOComponent<CS230::Sprite>()->GetHotSpot(3)), GetRotation(), GetScale(), math::RotateMatrix(GetRotation()) * Laser::LaserVelocity));
			Engine::GetGameStateManager().GetGSComponent<CS230::GameObjectManager>()->Add(new Laser(GetMatrix() * static_cast<math::vec2>(GetGOComponent<CS230::Sprite>()->GetHotSpot(4)), GetRotation(), GetScale(), math::RotateMatrix(GetRotation()) * Laser::LaserVelocity));
		}
	}

	UpdateVelocity(-(GetVelocity() * Ship::drag * dt));

	flame1.Update(dt);
	flame2.Update(dt);

	UpdatePosition(GetVelocity() * dt);
	if(GetGOComponent<Laser>() != nullptr)
	GetGOComponent<Laser>()->Update(dt);
	GetGOComponent<ScreenWrap>()->Update(dt);
	UpdateGOComponents(dt);
}

void Ship::Draw(math::TransformMatrix cameraMatrix)
{
	if (isDead == false)
	{
		flame1.Draw(cameraMatrix * GetMatrix() * math::TranslateMatrix(GetGOComponent<CS230::Sprite>()->GetHotSpot(1)));
		flame2.Draw(cameraMatrix * GetMatrix() * math::TranslateMatrix(GetGOComponent<CS230::Sprite>()->GetHotSpot(2)));
	}
	GetGOComponent<CS230::Sprite>()->Draw(cameraMatrix * GetMatrix());
	if (Engine::GetGSComponent<ShowCollision>() != nullptr)
	{
		if (Engine::GetGSComponent<ShowCollision>()->IsEnabled() == true)
		{
			if (GetGOComponent<CS230::Collision>() != nullptr)
				GetGOComponent<CS230::Collision>()->Draw(cameraMatrix);
		}
	}
}

GameObjectType Ship::GetObjectType()
{
	return GameObjectType::Ship;
}

std::string Ship::GetObjectTypeName()
{
	return "Ship";
}

bool Ship::CanCollideWith(GameObjectType objectBType)
{
	if (objectBType == GameObjectType::Meteor || objectBType == GameObjectType::EnemyShip)
	{
		return true;
	}
	return false;
}

void Ship::ResolveCollision(CS230::GameObject* objectB)
{
	if (objectB->GetObjectType() == GameObjectType::Meteor || objectB->GetObjectType() == GameObjectType::EnemyShip)
	{
		isDead = true;
		GetGOComponent<CS230::Sprite>()->PlayAnimation(static_cast<int>(Ship_Anim::Explode_Anim));
		RemoveGOComponent<CS230::Collision>();
	}
}