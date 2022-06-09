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
#include "Score.h" //Add Score
#include "Arena.h" //Map Collision
#include "Car_Anim.h"
BumperCar::BumperCar(math::vec2 startPos)
	: GameObject(startPos,0,math::vec2{0.5,0.5}), rotateCounterKey(CS230::InputKey::Keyboard::A), rotateClockKey(CS230::InputKey::Keyboard::D), isDead(false),
	light{ "Assets/Final/car_light.spt",this}, velocity(400), isOut(false)
{
	AddGOComponent(new CS230::Sprite("Assets/Final/bumper_car.spt", this));
	GetGOComponent<CS230::Sprite>()->PlayAnimation(static_cast<int>(Car_Anim::None_Anim));
}

void BumperCar::Update(double dt)
{
	//map collision
	if (GetPosition().x < (Engine::GetWindow().GetSize().x - Engine::GetGSComponent<Arena>()->Size().x) / 2 ||
		GetPosition().x > (Engine::GetWindow().GetSize().x + Engine::GetGSComponent<Arena>()->Size().x) / 2 ||
		GetPosition().y < (Engine::GetWindow().GetSize().y -Engine::GetGSComponent<Arena>()->Size().y) / 2 ||
		GetPosition().y > (Engine::GetWindow().GetSize().y + Engine::GetGSComponent<Arena>()->Size().y) / 2)
	{
		if (isDead == false)
		{
			isOut = true;
			isDead = true;
		}
	}

	if (isOut == true && isDead == true)
	{
		GetGOComponent<CS230::Sprite>()->PlayAnimation(static_cast<int>(Car_Anim::Disappear_Anim));
		isOut = false;
	}

	//rotate
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
	if (objectBType == GameObjectType::Apple || objectBType == GameObjectType::EnemyCar)
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
		Engine::GetGameStateManager().GetGSComponent<Score>()->AddScore(100);
		objectB->ResolveCollision(this);
	}
	if (objectB->GetObjectType() == GameObjectType::EnemyCar)
	{
		Engine::GetGameStateManager().GetGSComponent<Score>()->AddScore(100);
		objectB->ResolveCollision(this);
	}
}