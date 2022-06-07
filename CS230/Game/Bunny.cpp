/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Bunny.cpp
Project: CS230
Author: MINJAE YU
Creation date: 4/28/2022
-----------------------------------------------------------------*/
#include "Bunny.h" //Bunny
#include "GameObjectTypes.h" //return GameObjectTypes
#include "../Engine/Collision.h" //ResolveCollision
#include "Bunny_Anims.h" //BunnyAnimation
#include "../Engine/Engine.h" //GetGSComponent
#include "Score.h" //AddScore
#include "Hero.h" //heroPtr

Bunny::Bunny(math::vec2 pos, std::vector<double> patrolNodes, Hero* heroPtr) : GameObject(pos), patrolNodes(patrolNodes), heroPtr(heroPtr), currPatrolNode(0)
{
	AddGOComponent(new CS230::Sprite("Assets/bunny.spt", this));

	currState = &statePatrol;
	currState->Enter(this);
}

void Bunny::ResolveCollision(GameObject* objectA)
{
	if (objectA->GetObjectType() == GameObjectType::Hero)
	{
		ChangeState(&stateDead);
	}
}

//State_Patrol
void Bunny::State_Patrol::Enter(GameObject* object)
{
	Bunny* bunny = static_cast<Bunny*>(object);
	bunny->GetGOComponent<CS230::Sprite>()->PlayAnimation(static_cast<int>(Bunny_Anim::Walk_Anim));
	if (bunny->GetPosition().x > bunny->patrolNodes[bunny->currPatrolNode])
	{
		bunny->SetVelocity(math::vec2{ -bunny->velocity, 0 });
		bunny->SetScale(math::vec2{ -1,1 });
	}
	if(bunny->GetPosition().x < bunny->patrolNodes[bunny->currPatrolNode])
	{
		bunny->SetVelocity(math::vec2{ bunny->velocity, 0 });
		bunny->SetScale(math::vec2{ 1,1 });
	}
}

void Bunny::State_Patrol::Update(GameObject* object, double)
{
	Bunny* bunny = static_cast<Bunny*>(object);
	if (bunny->GetVelocity().x > 0 && bunny->GetPosition().x >= bunny->patrolNodes[bunny->currPatrolNode])
	{
		if (bunny->currPatrolNode >= bunny->patrolNodes.size() - 1)
		{
			bunny->currPatrolNode = 0;
		}
		else
		{
			bunny->currPatrolNode++;
		}
		bunny->ChangeState(&bunny->statePatrol);
	}
	if (bunny->GetVelocity().x < 0 && bunny->GetPosition().x <= bunny->patrolNodes[bunny->currPatrolNode])
	{
		if (bunny->currPatrolNode >= bunny->patrolNodes.size() - 1)
		{
			bunny->currPatrolNode = 0;
		}
		else
		{
			bunny->currPatrolNode++;
		}
		bunny->ChangeState(&bunny->statePatrol);
	}
}

void Bunny::State_Patrol::TestForExit(GameObject* object)
{
	Bunny* bunny = static_cast<Bunny*>(object);
	if (bunny->heroPtr->GetPosition().y == bunny->GetPosition().y)
	{
		if (bunny->GetVelocity().x < 0 && (bunny->heroPtr->GetPosition().x > bunny->patrolNodes[bunny->currPatrolNode] && bunny->heroPtr->GetPosition().x < bunny->GetPosition().x))
		{
			bunny->ChangeState(&bunny->stateAttack);
		}
		if (bunny->GetVelocity().x > 0 && (bunny->heroPtr->GetPosition().x < bunny->patrolNodes[bunny->currPatrolNode] && bunny->heroPtr->GetPosition().x > bunny->GetPosition().x))
		{
			bunny->ChangeState(&bunny->stateAttack);
		}
	}
}

//State_Attack
void Bunny::State_Attack::Enter(GameObject* object)
{
	Bunny* bunny = static_cast<Bunny*>(object);
	bunny->GetGOComponent<CS230::Sprite>()->PlayAnimation(static_cast<int>(Bunny_Anim::Attack_Anim));
	if (bunny->GetPosition().x > bunny->patrolNodes[bunny->currPatrolNode])
	{
		bunny->SetVelocity(math::vec2{ -bunny->velocity * 2, 0 });
		bunny->SetScale(math::vec2{ -1,1 });
	}
	if (bunny->GetPosition().x < bunny->patrolNodes[bunny->currPatrolNode])
	{
		bunny->SetVelocity(math::vec2{ bunny->velocity * 2, 0 });
		bunny->SetScale(math::vec2{ 1,1 });
	}
}

void Bunny::State_Attack::Update(GameObject* object, double)
{
	Bunny* bunny = static_cast<Bunny*>(object);
	if (bunny->GetVelocity().x > 0 && bunny->GetPosition().x >= bunny->patrolNodes[bunny->currPatrolNode])
	{
		bunny->ChangeState(&bunny->statePatrol);
	}
	if (bunny->GetVelocity().x < 0 && bunny->GetPosition().x <= bunny->patrolNodes[bunny->currPatrolNode])
	{
		bunny->ChangeState(&bunny->statePatrol);
	}
}

void Bunny::State_Attack::TestForExit(GameObject*) {}

//State_Dead
void Bunny::State_Dead::Enter(GameObject* object)
{
	Bunny* bunny = static_cast<Bunny*>(object);
	bunny->SetVelocity(math::vec2{ 0,0 });
	bunny->RemoveGOComponent<CS230::Collision>();
	bunny->GetGOComponent<CS230::Sprite>()->PlayAnimation(static_cast<int>(Bunny_Anim::Dead_Anim));
	Engine::GetGameStateManager().GetGSComponent<Score>()->AddScore(100);
}

void Bunny::State_Dead::Update(GameObject*, double) {}

void Bunny::State_Dead::TestForExit(GameObject*) {}