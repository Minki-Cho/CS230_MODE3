#include "EnemyCar.h" //EnemyCar
#include "../Engine/Engine.h" //GetGSComponent
#include "../Engine/Collision.h" //Collision
#include "../Engine/ShowCollision.h" //ShowCollision
#include "Flame_Anims.h" //Flame Animation
#include "GameObjectTypes.h" //GameObjectTypes
#include "Ship_Anims.h" //Ship Animation

EnemyCar::EnemyCar(GameObject* player) : player(player), GameObject(math::vec2{}, 0, math::vec2{ 0.75,0.75 })
{
	//AddGOComponent(new CS230::Sprite("Assets/enemyShip.spt", this));
	SetRotation(GetRandom(0, 2 * PI));
	SetPosition(math::vec2{ GetRandom(player->GetPosition().x - 1024, player->GetPosition().x - 2048),
		GetRandom(player->GetPosition().y, player->GetPosition().y) });
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
		UpdateVelocity(math::RotateMatrix(GetRotation()) * math::vec2{ 0,accel * dt });
	UpdatePosition(GetVelocity() * dt);
	UpdateGOComponents(dt);
}

void EnemyCar::Draw(math::TransformMatrix displayMatrix)
{
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

GameObjectType EnemyCar::GetObjectType()
{
	return GameObjectType::EnemyShip;
}

std::string EnemyCar::GetObjectTypeName()
{
	return "EnemyShip";
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
		RemoveGOComponent<CS230::Collision>();
	}
}

double EnemyCar::GetRandom(double min, double max)
{
	return min + rand() / (RAND_MAX / (max - min));
}