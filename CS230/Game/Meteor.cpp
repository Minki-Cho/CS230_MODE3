/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Meteor.cpp
Project: CS230
Author: MINJAE YU
Creation date: 4/28/2022
-----------------------------------------------------------------*/
#include "Meteor.h" //Meteor
#include "../Engine/Engine.h" //Get Window Size
#include "ScreenWrap.h" //ScreenWrap
#include "GameObjectTypes.h" //return GameObjectTypes
#include "../Engine/GameObjectManager.h" //Get GameObjectManager
#include "Meteor_Anims.h" //Meteor Animation
#include "../Engine/Collision.h" //removeCollision
#include "Score.h" //Add Score
#include "Laser.h" //LaserVelocity
#include "GameParticles.h" //HitEmitter, MeteorBitEmitter

Meteor::Meteor() : GameObject({ 0,0 }), health(100), size(1)
{
	AddGOComponent(new CS230::Sprite("Assets/Meteor.spt", this));
	AddGOComponent(new ScreenWrap(*this));

	SetPosition(math::vec2{ static_cast<double>(rand() % Engine::GetWindow().GetSize().x),static_cast<double>(rand() % Engine::GetWindow().GetSize().y) });
	SetVelocity(math::vec2{ scoped_random<double>(velocity_MIN,velocity_MAX),scoped_random<double>(velocity_MIN,velocity_MAX) });
	SetScale(math::vec2{ scoped_random<double>(scale_MIN,scale_MAX),scoped_random<double>(scale_MIN,scale_MAX) });
	GetGOComponent<CS230::Sprite>()->PlayAnimation(static_cast<int>(Meteor_Anim::None_Anim));
}

Meteor::Meteor(Meteor& original) : GameObject({ 0,0 }), size(original.size)
{
	size++;
	health = 100;
	AddGOComponent(new CS230::Sprite("Assets/Meteor.spt", this));
	AddGOComponent(new ScreenWrap(*this));

	SetPosition(math::vec2{ original.GetPosition() });
	SetRotation(scoped_random<unsigned int>(0, PI * 2));
	SetVelocity(math::vec2{ scoped_random<double>(velocity_MIN,velocity_MAX),scoped_random<double>(velocity_MIN,velocity_MAX) });
	switch (size)
	{
	case 2:
		SetScale(math::vec2{ scoped_random<double>(scale_MAX / 2,scale_MIN),scoped_random<double>(scale_MAX / 2,scale_MIN) });
		break;
	case 3:
		SetScale(math::vec2{ scoped_random<double>(scale_MAX / 4,scale_MAX / 2),scoped_random<double>(scale_MAX / 4,scale_MAX / 2) });
		break;
	}
}

void Meteor::Update(double dt)
{
	GameObject::Update(dt);
	GetGOComponent<ScreenWrap>()->Update(dt);

	if (GetGOComponent<CS230::Sprite>()->IsAnimationDone() == true)
	{
		SetDestroyed(true);
	}
}

GameObjectType Meteor::GetObjectType()
{
	return GameObjectType::Meteor;
}

std::string Meteor::GetObjectTypeName()
{
	return "Meteor";
}

void Meteor::ResolveCollision(GameObject* objectB)
{
	if (objectB->GetObjectType() == GameObjectType::Laser)
	{
		//Particle
		math::vec2 vectorToObject = objectB->GetPosition() - GetPosition();
		math::vec2 collisionPoint = vectorToObject.Normalize() * GetGOComponent<CS230::CircleCollision>()->GetRadius() + GetPosition();
		Engine::GetGameStateManager().GetGSComponent<HitEmitter>()->Emit(1, collisionPoint, GetVelocity(), math::vec2{ 0,0 }, 0);

		math::vec2 laserVelocity = objectB->GetVelocity();
		math::vec2 emitVector = vectorToObject.Normalize() * 2 + laserVelocity.Normalize();
		Engine::GetGameStateManager().GetGSComponent<MeteorBitEmitter>()->Emit(10, collisionPoint, GetVelocity(), emitVector * 50, PI / 2);

		health -= 10;
		if (health > 0)
		{
			UpdateVelocity(0.01 * Laser::LaserVelocity);
		}
	}

	if (health <= 0)
	{
		if (size < 3)
		{
			Meteor* m1 = new Meteor(*this);
			Meteor* m2 = new Meteor(*this);
			m1->SetRotation(this->GetRotation() + PI / 6);
			m2->SetRotation(this->GetRotation() - PI / 6);
			Engine::GetGameStateManager().GetGSComponent<CS230::GameObjectManager>()->Add(m1);
			Engine::GetGameStateManager().GetGSComponent<CS230::GameObjectManager>()->Add(m2);
		}
		GetGOComponent<CS230::Sprite>()->PlayAnimation(static_cast<int>(Meteor_Anim::Fade_Anim));
		RemoveGOComponent<CS230::Collision>();
		Engine::GetGameStateManager().GetGSComponent<Score>()->AddScore(100 * size);
	}
}