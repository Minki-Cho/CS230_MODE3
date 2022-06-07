/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: BumperCar.cpp
Project: CS230
Author: MINJAE YU, MINKI CHO
Creation date: 6/7/2022
-----------------------------------------------------------------*/
#include "BumperCar.h" //BumperCar
#include "../Engine/Engine.h" //logger
#include "../Engine/Collision.h" //GetGOComponent
#include "../Engine/ShowCollision.h" //GetGSComponent
#include "GameObjectTypes.h" //return GameObjectTypes
#include "../Engine/GameObjectManager.h" //Get GameObjectManager
//#include "Score.h" //Add Score

BumperCar::BumperCar(math::vec2 startPos)
	: GameObject(startPos), rotateCounterKey(CS230::InputKey::Keyboard::A), rotateClockKey(CS230::InputKey::Keyboard::D), isDead(false),
	light{ "Assets/Final/car_light.spt",this}, velocity(200)
{
	AddGOComponent(new CS230::Sprite("Assets/Final/bumper_car.spt", this));
	//GetGOComponent<CS230::Sprite>()->PlayAnimation(static_cast<int>(Ship_Anim::None_Anim));
}

void BumperCar::Update(double dt)
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
		SetVelocity(math::RotateMatrix(GetRotation()) * math::vec2{ 0,velocity / 2 });
	}
	UpdatePosition(GetVelocity() * dt);
	UpdateGOComponents(dt);
}

void BumperCar::Draw(math::TransformMatrix cameraMatrix)
{
	if (isDead == false)
	{
		light.Draw(cameraMatrix * GetMatrix() * math::TranslateMatrix(GetGOComponent<CS230::Sprite>()->GetHotSpot(1)));
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

GameObjectType BumperCar::GetObjectType()
{
	return GameObjectType::BumperCar;
}

std::string BumperCar::GetObjectTypeName()
{
	return "BumperCar";
}

bool BumperCar::CanCollideWith(GameObjectType objectBType)
{
	if (objectBType == GameObjectType::Apple)
	{
		return true;
	}
	return false;
}

void BumperCar::ResolveCollision(CS230::GameObject* objectB)
{
	if (objectB->GetObjectType() == GameObjectType::Apple)
	{
		velocity += increase_val;
	}
}