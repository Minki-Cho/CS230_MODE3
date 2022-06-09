#include "EnemyCar.h" //EnemyCar
#include "../Engine/Engine.h" //GetGSComponent
#include "../Engine/Collision.h" //Collision
#include "../Engine/ShowCollision.h" //ShowCollision
#include "Flame_Anims.h" //Flame Animation
#include "GameObjectTypes.h" //GameObjectTypes
#include "EnemyCar_Anim.h" //Car Animation
#include "GameParticles.h"

EnemyCar::EnemyCar(GameObject* player) : player(player), GameObject(math::vec2{}, 0, math::vec2{ 0.75,0.75 })
{
	AddGOComponent(new CS230::Sprite("Assets/Final/slasher.spt", this));
	SetRotation(GetRandom(0, 2 * PI));
	SetPosition(math::vec2{ GetRandom(player->GetPosition().x + 10, player->GetPosition().x),GetRandom(0, Engine::GetWindow().GetSize().y )});
	GetGOComponent<CS230::Sprite>()->PlayAnimation(static_cast<int>(EnemyCar_Anim::Rot_Anim));
}

void EnemyCar::Update(double dt)
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
		//math::vec2 vectorToObject = collidedWith->GetPosition() - GetPosition();
		//math::vec2 collisionPoint = vectorToObject.Normalize() * GetGOComponent<CS230::CircleCollision>()->GetRadius() + GetPosition();
		//Engine::GetGameStateManager().GetGSComponent<HitEmitter>()->Emit(1, collisionPoint, GetVelocity(), math::vec2{ 0,0 }, 0);

		math::vec2 vectorToObject = collidedWith->GetPosition() - GetPosition();
		math::vec2 collisionPoint = vectorToObject.Normalize() * GetGOComponent<CS230::CircleCollision>()->GetRadius() + GetPosition();
		math::vec2 laserVelocity = collidedWith->GetVelocity();
		math::vec2 emitVector = vectorToObject.Normalize() * 2 + laserVelocity.Normalize();
		Engine::GetGameStateManager().GetGSComponent<MeteorBitEmitter>()->Emit(10, collisionPoint, GetVelocity(), emitVector * 50, PI / 2);

		RemoveGOComponent<CS230::Collision>();
	}
}

double EnemyCar::GetRandom(double min, double max)
{
	return min + rand() / (RAND_MAX / (max - min));
}