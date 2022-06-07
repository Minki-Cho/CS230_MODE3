/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Hero.cpp
Project: CS230
Author: MINJAE YU
Creation date: 3/21/2022
-----------------------------------------------------------------*/
#include "Hero.h" //Hero
#include "../Engine/Engine.h" //logger
#include "Mode1.h" //gravity
#include "../Engine/Camera.h" //camera
#include "Hero_Anims.h" //call Animation enum
#include "Gravity.h" //gravity
#include "GameObjectTypes.h" //return GameObjectTypes
#include "../Engine/Collision.h" //RectCollision
#include "GameParticles.h" //Emit

Hero::Hero(math::vec2 startPos)
	: GameObject(startPos), moveLeftKey(CS230::InputKey::Keyboard::Left),
	moveRightKey(CS230::InputKey::Keyboard::Right), moveUpKey(CS230::InputKey::Keyboard::Up),
	hurtTimer(0), drawHero(false), isDead(false), standingOnObject(nullptr)
{
	AddGOComponent(new CS230::Sprite("Assets/Hero.spt", this));
	AddGOComponent(new Gravity(Mode1::gravity));

	for (GameObject* object : Engine::GetGameStateManager().GetGSComponent<CS230::GameObjectManager>()->Objects())
	{
		if (object->GetObjectType() == GameObjectType::Floor && DoesCollideWith(object) == true)
		{
			standingOnObject = object;
			SetPosition({ GetPosition().x, object->GetGOComponent<CS230::RectCollision>()->GetWorldCoorRect().Top() });
			break;
		}
	}

	currState = &stateIdle;
	currState->Enter(this);
}

void Hero::Update(double dt)
{
	GameObject::Update(dt);

	if (hurtTimer >= 0)
	{
		hurtTimer -= dt;
		drawHero = !drawHero;
	}
	else
	{
		drawHero = true;
	}

	//boundary check
	if (GetGOComponent<CS230::RectCollision>()->GetWorldCoorRect().Left() < 0)
	{
		SetPosition(math::vec2{ static_cast<double>(GetGOComponent<CS230::Sprite>()->GetHotSpot(0).x / 2) , GetPosition().y });
		SetVelocity(math::vec2{ 0,GetVelocity().y });
	}
	else if (GetGOComponent<CS230::RectCollision>()->GetWorldCoorRect().Right() > Engine::GetGSComponent<CS230::Camera>()->GetPosition().x + Engine::GetWindow().GetSize().x)
	{
		SetPosition(math::vec2{ static_cast<double>(Engine::GetGSComponent<CS230::Camera>()->GetPosition().x + Engine::GetWindow().GetSize().x - (GetGOComponent<CS230::Sprite>()->GetHotSpot(0).x / 2)), GetPosition().y });
		SetVelocity(math::vec2{ 0,GetVelocity().y });
	}
}

void Hero::Draw(math::TransformMatrix displayMatrix)
{
	if (drawHero == true)
	{
		GameObject::Draw(displayMatrix);
	}
}

const math::vec2& Hero::GetPosition() const
{
	return GameObject::GetPosition();
}

GameObjectType Hero::GetObjectType()
{
	return GameObjectType::Hero;
}

std::string Hero::GetObjectTypeName()
{
	return "Hero";
}

bool Hero::CanCollideWith(GameObjectType objectBType)
{
	if (objectBType == GameObjectType::Particle)
	{
		return false;
	}
	return true;
}

void Hero::ResolveCollision(GameObject* objectB)
{
	math::rect2 collideRect = objectB->GetGOComponent<CS230::RectCollision>()->GetWorldCoorRect();
	math::rect2 heroRect = GetGOComponent<CS230::RectCollision>()->GetWorldCoorRect();
	switch (objectB->GetObjectType())
	{
	case GameObjectType::Floor:
		[[fallthrough]];
	case GameObjectType::TreeStump:
		if (heroRect.Bottom() < collideRect.Top() && objectB->DoesCollideWith(GetPosition()) == true)
		{
			if (abs(GetVelocity().y) > abs(jumpVelocity))
			{
				Engine::GetGameStateManager().GetGSComponent<SmokeEmitter>()->Emit(1, math::vec2{ GetPosition() }, math::vec2{ 0,0 }, math::vec2{ 0,0 }, 0);
			}
			SetPosition(math::vec2{ GetPosition().x,collideRect.Top() });
			standingOnObject = objectB;
			currState->TestForExit(this);
			break;
		}

		if (heroRect.Right() > collideRect.Left() && heroRect.Right() < collideRect.Right())
		{
			UpdatePosition(math::vec2{ collideRect.Left() - heroRect.Right(),0.0 });
			SetVelocity(math::vec2{ 0.0,GetVelocity().y });
			standingOnObject = nullptr;
		}
		else if (heroRect.Left() < collideRect.Right() && heroRect.Left() > collideRect.Left())
		{
			UpdatePosition(math::vec2{ collideRect.Right() - heroRect.Left(),0.0 });
			SetVelocity(math::vec2{ 0.0,GetVelocity().y });
			standingOnObject = nullptr;
		}
		break;
	case GameObjectType::Bunny:
		if (currState == &stateSkidding && DoesCollideWith(objectB) == true)
		{
			objectB->ResolveCollision(this);
			double SmokeLocation;
			if (heroRect.Left() < collideRect.Left())
			{
				SmokeLocation = (heroRect.Right() - collideRect.Left()) / 2;
				Engine::GetGameStateManager().GetGSComponent<SmokeEmitter>()->Emit(1, math::vec2{ collideRect.Left() + SmokeLocation,GetPosition().y }, math::vec2{ 0,0 }, math::vec2{ 0,0 }, 0);
			}
			else
			{
				SmokeLocation = (heroRect.Left() - collideRect.Right()) / 2;
				Engine::GetGameStateManager().GetGSComponent<SmokeEmitter>()->Emit(1, math::vec2{ heroRect.Left() + SmokeLocation,GetPosition().y }, math::vec2{ 0,0 }, math::vec2{ 0,0 }, 0);
			}
			break;
		}
		[[fallthrough]];
	case GameObjectType::Ball:
		if (heroRect.Bottom() < collideRect.Top() && objectB->DoesCollideWith(GetPosition()) == true)
		{
			if (currState == &stateFalling)
			{
				SetVelocity(math::vec2{ GetVelocity().x,jumpVelocity / static_cast<int>(objectB->GetObjectType()) });
				standingOnObject = nullptr;
				if (objectB->GetObjectType() == GameObjectType::Bunny)
				{
					objectB->ResolveCollision(this);
					Engine::GetGameStateManager().GetGSComponent<SmokeEmitter>()->Emit(1, math::vec2{ GetPosition().x,GetPosition().y + (collideRect.Top() - heroRect.Bottom()) / 2 }, math::vec2{ 0,0 }, math::vec2{ 0,0 }, 0);
				}
			}
			break;
		}

		if (heroRect.Right() > collideRect.Left() && heroRect.Right() < collideRect.Right())
		{
			hurtTimer = hurtTime;
			SetPosition(math::vec2{ collideRect.Left() - (heroRect.Right() - heroRect.Left()) / 2,GetPosition().y });
			SetVelocity(math::vec2{ -max_x_vel / 2,GetVelocity().y });
		}
		else if (heroRect.Left() < collideRect.Right() && heroRect.Left() > collideRect.Left())
		{
			hurtTimer = hurtTime;
			SetPosition(math::vec2{ collideRect.Right() + (heroRect.Right() - heroRect.Left()) / 2,GetPosition().y });
			SetVelocity(math::vec2{ max_x_vel / 2,GetVelocity().y });
		}
		break;
	case GameObjectType::Trigger:
		objectB->ResolveCollision(this);
	}
}

void Hero::UpdateXVelocity(double dt)
{
	//left
	if (moveLeftKey.IsKeyDown())
	{
		//max limit
		if (GetVelocity().x <= -max_x_vel)
		{
			SetVelocity(math::vec2{ -max_x_vel,GetVelocity().y });
		}
		//acceleration
		else if (GetVelocity().x <= 0 && GetVelocity().x > -max_x_vel)
		{
			UpdateVelocity(math::vec2{ -x_accel * dt,0 });
		}
	}
	//right
	if (moveRightKey.IsKeyDown())
	{
		//max limit
		if (GetVelocity().x >= max_x_vel)
		{
			SetVelocity(math::vec2{ max_x_vel,GetVelocity().y });
		}
		//acceleration
		else if (GetVelocity().x >= 0 && GetVelocity().x < max_x_vel)
		{
			UpdateVelocity(math::vec2{ x_accel * dt,0 });
		}
	}
	if(moveLeftKey.IsKeyDown() == false && moveRightKey.IsKeyDown() == false)
	{
		//drag
		if (GetVelocity().x > 0)
		{
			UpdateVelocity(math::vec2{ -x_drag * dt,0 });
		}
		if (GetVelocity().x < 0)
		{
			UpdateVelocity(math::vec2{ x_drag * dt,0 });
		}

		//stop
		if (std::abs(GetVelocity().x) < x_drag * dt && GetVelocity().x != 0)
		{
			SetVelocity(math::vec2{ 0,GetVelocity().y });
		}
	}
}

//state_idle
void Hero::State_Idle::Enter(GameObject* object)
{
	Hero* hero = static_cast<Hero*>(object);
	if (hero->standingOnObject == nullptr)
	{
		Engine::GetLogger().LogError("Not standing on object!");
	}
	hero->GetGOComponent<CS230::Sprite>()->PlayAnimation(static_cast<int>(Hero_Anim::Hero_Idle_Anim));
}

void Hero::State_Idle::Update([[maybe_unused]] GameObject* hero, [[maybe_unused]] double dt) {}

void Hero::State_Idle::TestForExit(GameObject* object)
{
	Hero* hero = static_cast<Hero*>(object);
	if (hero->moveLeftKey.IsKeyDown() == true || hero->moveRightKey.IsKeyDown() == true)
	{
		hero->ChangeState(&hero->stateRunning);
	}
	if (hero->moveUpKey.IsKeyDown() == true)
	{
		hero->ChangeState(&hero->stateJumping);
	}
}

//state_jumping
void Hero::State_Jumping::Enter(GameObject* object) {
	Hero* hero = static_cast<Hero*>(object);
	hero->GetGOComponent<CS230::Sprite>()->PlayAnimation(static_cast<int>(Hero_Anim::Hero_Jump_Anim));
	hero->SetVelocity(math::vec2{ hero->GetVelocity().x,jumpVelocity }); //Set the velocity.y
	hero->standingOnObject = nullptr;
}

void Hero::State_Jumping::Update(GameObject* object, double dt) {
	Hero* hero = static_cast<Hero*>(object);
	hero->UpdateVelocity(math::vec2{ 0,-(hero->GetGOComponent<Gravity>()->GetValue() * dt)}); //Apply Gravity
	hero->UpdateXVelocity(dt); //Change X Velocity stuff
}

void Hero::State_Jumping::TestForExit(GameObject* object) {
	Hero* hero = static_cast<Hero*>(object);
	if (hero->moveUpKey.IsKeyDown() == false)
	{
		//Jump Key not pressed
		hero->SetVelocity(math::vec2{ hero->GetVelocity().x,0 });
		hero->ChangeState(&hero->stateFalling);
	}
	if (hero->GetVelocity().y <= 0)
	{
		hero->ChangeState(&hero->stateFalling);
	}
}

//state_running
void Hero::State_Running::Enter(GameObject* object)
{
	Hero* hero = static_cast<Hero*>(object);
	if (hero->standingOnObject == nullptr)
	{
		Engine::GetLogger().LogError("Not standing on object!");
	}
	hero->GetGOComponent<CS230::Sprite>()->PlayAnimation(static_cast<int>(Hero_Anim::Hero_Run_Anim));
	if (hero->moveLeftKey.IsKeyDown())
	{
		hero->SetScale(math::vec2{ -1.0,1.0 });
	}
	else if (hero->moveRightKey.IsKeyDown())
	{
		hero->SetScale(math::vec2{ 1.0,1.0 });
	}
}

void Hero::State_Running::Update(GameObject* object, double dt)
{
	Hero* hero = static_cast<Hero*>(object);
	hero->UpdateXVelocity(dt); //Change X Velocity stuff
}

void Hero::State_Running::TestForExit(GameObject* object)
{
	Hero* hero = static_cast<Hero*>(object);
	//idle
	if (hero->GetVelocity().x == 0)
	{
		hero->ChangeState(&hero->stateIdle);
	}
	//jumping
	if (hero->moveUpKey.IsKeyDown())
	{
		hero->ChangeState(&hero->stateJumping);
	}
	//skidding
	if (hero->GetVelocity().x < 0 && hero->moveRightKey.IsKeyDown())
	{
		hero->ChangeState(&hero->stateSkidding);
	}
	if (hero->GetVelocity().x > 0 && hero->moveLeftKey.IsKeyDown())
	{
		hero->ChangeState(&hero->stateSkidding);
	}
	//falling
	if (hero->standingOnObject != nullptr && hero->standingOnObject->DoesCollideWith(hero->GetPosition()) == false)
	{
		hero->standingOnObject = nullptr;
		hero->ChangeState(&hero->stateFalling);
	}
}

//state_falling
void Hero::State_Falling::Enter(GameObject* object)
{
	Hero* hero = static_cast<Hero*>(object);
	hero->GetGOComponent<CS230::Sprite>()->PlayAnimation(static_cast<int>(Hero_Anim::Hero_Fall_Anim));
}

void Hero::State_Falling::Update(GameObject* object, double dt)
{
	Hero* hero = static_cast<Hero*>(object);
	hero->UpdateVelocity(math::vec2{ 0,-(hero->GetGOComponent<Gravity>()->GetValue() * dt) });
	hero->UpdateXVelocity(dt); //Change X Velocity stuff
}

void Hero::State_Falling::TestForExit(GameObject* object)
{
	Hero* hero = static_cast<Hero*>(object);
	if (hero->standingOnObject != nullptr)
	{
		hero->SetVelocity(math::vec2{ hero->GetVelocity().x,0 });
		//idle
		if (hero->GetVelocity().x == 0)
		{
			hero->ChangeState(&hero->stateIdle);
		}
		//running
		if (hero->moveLeftKey.IsKeyDown() && hero->GetVelocity().x <= 0)
		{
			hero->ChangeState(&hero->stateRunning);
		}
		if (hero->moveRightKey.IsKeyDown() && hero->GetVelocity().x >= 0)
		{
			hero->ChangeState(&hero->stateRunning);
		}
		//skidding
		if (hero->GetVelocity().x < 0 && hero->moveRightKey.IsKeyDown() == true)
		{
			hero->ChangeState(&hero->stateSkidding);
		}
		if (hero->GetVelocity().x > 0 && hero->moveLeftKey.IsKeyDown() == true)
		{
			hero->ChangeState(&hero->stateSkidding);
		}
	}
	//isDead
	if (hero->GetPosition().y < hero->deadline)
	{
		hero->isDead = true;
	}
}

//state_skidding
void Hero::State_Skidding::Enter(GameObject* object)
{
	Hero* hero = static_cast<Hero*>(object);
	hero->GetGOComponent<CS230::Sprite>()->PlayAnimation(static_cast<int>(Hero_Anim::Hero_Skid_Anim));
}

void Hero::State_Skidding::Update(GameObject* object, double dt)
{
	Hero* hero = static_cast<Hero*>(object);
	//left
	if (hero->moveLeftKey.IsKeyDown())
	{
		if (hero->GetVelocity().x > 0)
		{
			hero->UpdateVelocity(math::vec2{ -(x_drag + x_drag) * dt,0 });
		}
	}
	//right
	if (hero->moveRightKey.IsKeyDown())
	{
		if (hero->GetVelocity().x < 0)
		{
			hero->UpdateVelocity(math::vec2{ (x_drag + x_drag) * dt,0 });
		}
	}
}

void Hero::State_Skidding::TestForExit(GameObject* object)
{
	Hero* hero = static_cast<Hero*>(object);
	//jumping
	if (hero->moveUpKey.IsKeyDown())
	{
		hero->ChangeState(&hero->stateJumping);
	}
	//running
	if (hero->moveLeftKey.IsKeyDown() && hero->GetVelocity().x <= 0)
	{
		hero->ChangeState(&hero->stateRunning);
	}
	if (hero->moveRightKey.IsKeyDown() && hero->GetVelocity().x >= 0)
	{
		hero->ChangeState(&hero->stateRunning);
	}
	if (!hero->moveLeftKey.IsKeyDown() && !hero->moveRightKey.IsKeyDown())
	{
		hero->ChangeState(&hero->stateRunning);
	}
}