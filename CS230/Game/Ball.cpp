/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Ball.cpp
Project: CS230
Author: MINJAE YU
Creation date: 3/25/2021
-----------------------------------------------------------------*/
#include "Ball.h" //Ball
#include "../Engine/Camera.h" //camera
#include "../Engine/Engine.h" //Get logger
#include "Ball_Anims.h" //call Animation enum
#include "Gravity.h" //gravity
#include "GameObjectTypes.h" //return GameObjectTypes
#include "../Engine/Collision.h" //ResolveCollision
#include "Mode1.h" //gravity

Ball::Ball(math::vec2 startPos)
	: GameObject(startPos)
{
	AddGOComponent(new CS230::Sprite("Assets/Ball.spt", this));
	AddGOComponent(new Gravity(Mode1::gravity));
	currState = &stateBounce;
	currState->Enter(this);
}

GameObjectType Ball::GetObjectType()
{
	return GameObjectType::Ball;
}

std::string Ball::GetObjectTypeName()
{
	return "Ball";
}

bool Ball::CanCollideWith(GameObjectType objectBType)
{
	if (objectBType == GameObjectType::Floor)
	{
		return true;
	}
	return false;
}

void Ball::ResolveCollision(CS230::GameObject* objectB)
{
	if (objectB->GetObjectType() == GameObjectType::Floor)
	{
		if (GetPosition().y < objectB->GetGOComponent<CS230::RectCollision>()->GetWorldCoorRect().Top())
		{
			CS230::RectCollision* collideRect =
				static_cast<CS230::RectCollision*>(objectB->GetGOComponent<CS230::RectCollision>());
			SetPosition({ GetPosition().x, collideRect->GetWorldCoorRect().Top() });
			SetVelocity({ GetVelocity().x, 0 });
			ChangeState(&stateLand);
		}
	}
}

//state_bounce
void Ball::State_Bounce::Enter(GameObject* object)
{
	Ball* ball = static_cast<Ball*>(object);
	ball->GetGOComponent<CS230::Sprite>()->PlayAnimation(static_cast<int>(Ball_Anim::None_Anim));
	ball->UpdateVelocity(math::vec2{ 0,bounceVelocity });
}

void Ball::State_Bounce::Update(GameObject* object, double dt)
{
	Ball* ball = static_cast<Ball*>(object);
	ball->UpdateVelocity(math::vec2{ 0, -ball->GetGOComponent<Gravity>()->GetValue() * dt });
}

void Ball::State_Bounce::TestForExit(GameObject*) {}

//state_land
void Ball::State_Land::Enter(GameObject* object)
{
	Ball* ball = static_cast<Ball*>(object);
	ball->GetGOComponent<CS230::Sprite>()->PlayAnimation(static_cast<int>(Ball_Anim::Squish_Anim));
}

void Ball::State_Land::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] double dt) {}

void Ball::State_Land::TestForExit(GameObject* object)
{
	Ball* ball = static_cast<Ball*>(object);
	if (ball->GetGOComponent<CS230::Sprite>()->IsAnimationDone() == true)
	{
		ball->ChangeState(&ball->stateBounce);
	}
}