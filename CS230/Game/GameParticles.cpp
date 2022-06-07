/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: GameParticles.cpp
Project: CS230
Author: MINJAE YU
Creation date: 5/26/2022
-----------------------------------------------------------------*/
#include "GameParticles.h" //GameParticles

//SmokeEmitter
SmokeEmitter::SmokeEmitter() : ParticleEmitter(1.0)
{
	for (int i = 0; i < NumParticles; i++)
	{
		ParticleEmitter::AddParticle(new SmokeParticle{ "Assets/Smoke.spt" });
	}
}

//HitEmitter
HitEmitter::HitEmitter() : ParticleEmitter(1.0)
{
	for (int i = 0; i < NumParticles; i++)
	{
		ParticleEmitter::AddParticle(new HitParticle{ "Assets/hit.spt" });
	}
}

//MeteorBitEmitter
MeteorBitEmitter::MeteorBitEmitter() : ParticleEmitter(1.5)
{
	for (int i = 0; i < NumParticles; i++)
	{
		ParticleEmitter::AddParticle(new MeteorBit{ "Assets/MeteorBit.spt" });
	}
}