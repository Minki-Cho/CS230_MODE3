/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: ParticleEmitter.cpp
Project: CS230
Author: MINJAE YU
Creation date: 5/26/2022
-----------------------------------------------------------------*/
#include "ParticleEmitter.h" //ParticleEmitter
#include "Engine.h" //GetGSComponent, GetLogger
#include "GameObjectManager.h" //gameObjectManager

//ParticleEmitter
CS230::ParticleEmitter::ParticleEmitter(double lifetime) : lifetime(lifetime), particleIndexToUse(0) {}

CS230::ParticleEmitter::~ParticleEmitter()
{
	particleMemoryPool.clear();
}

void CS230::ParticleEmitter::AddParticle(Particle* particleData)
{
	particleMemoryPool.push_back(particleData);
	Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(particleData);
}

void CS230::ParticleEmitter::Emit(int number, math::vec2 position, math::vec2 emitterVelocity, math::vec2 emitVector, double spread)
{
	for (int i = 0; i < number; i++)
	{
		if (particleMemoryPool[particleIndexToUse]->IsAlive() == true)
		{
			Engine::GetLogger().LogError("Overwriting a particle!");
		}
		emitVector *= GetRandom(0.5, 1.0);
		particleMemoryPool[particleIndexToUse]->Revive(position, math::TranslateMatrix(emitterVelocity) * math::RotateMatrix(GetRandom(-spread / 2, spread / 2)) * emitVector, lifetime);
		particleIndexToUse++;
	}
	if (particleIndexToUse >= particleMemoryPool.size())
		particleIndexToUse = 0;
}

//Particle
CS230::ParticleEmitter::Particle::Particle(const std::filesystem::path& spriteFile) : GameObject(math::vec2{}), life(0)
{
	AddGOComponent(new CS230::Sprite(spriteFile, this));
}

void CS230::ParticleEmitter::Particle::Revive(math::vec2 positions, math::vec2 velocitys, double lifes)
{
	SetPosition(positions);
	SetVelocity(velocitys);
	(*this).life = lifes;
	GetGOComponent<CS230::Sprite>()->PlayAnimation(0);
}

void CS230::ParticleEmitter::Particle::Update(double dt)
{
	if (IsAlive() == true)
	{
		life -= dt;
		GameObject::Update(dt);
	}
}

void CS230::ParticleEmitter::Particle::Draw(math::TransformMatrix matrix)
{
	if (IsAlive() == true)
	{
		GetGOComponent<CS230::Sprite>()->Draw(GetMatrix() * matrix);
	}
}

double CS230::ParticleEmitter::GetRandom(double min, double max)
{
	return min + rand() / (RAND_MAX / (max - min));
}