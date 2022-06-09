/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Mode3.cpp
Project: CS230
Author: MINJAE YU, MINKI CHO
Creation date: 6/7/2022
-----------------------------------------------------------------*/
#include "Mode3.h" //Mode2
#include "../Engine/Engine.h" //GetGameStateManager
#include "BumperCar.h" //new BumperCar
#include "Meteor.h" //new Meteor
#include "Fonts.h" //Load Fonts
#include "Screens.h" //Go to MainMenu
#include "Score.h" //score
#include "../Engine/ShowCollision.h" //collision
#include "GameParticles.h" //HitEmitter, MeteorBitEmitter
#include "Arena.h" //EnemyShip
#include "Apple.h"
#include "EnemyCar.h"
#include "EnemyCarType.h"

Mode3::Mode3() : gameObjectManagerPtr(nullptr), bumpercarPtr(nullptr),
#ifdef _DEBUG
modeNext(CS230::InputKey::Keyboard::Escape), modeReload(CS230::InputKey::Keyboard::R), score(0)
#else
modeNext(CS230::InputKey::Keyboard::Escape), modeReload(CS230::InputKey::Keyboard::R)
#endif
{}

void Mode3::Load()
{
	score = 0;
	AddGSComponent(new Score(score, Fonts::Font1));
	std::string gameoverString = "Game Over";
	GameOverTexture = Engine::GetSpriteFont(static_cast<int>(Fonts::Font2)).DrawTextToTexture(gameoverString, 0xFFFFFFFF, true);
	std::string restartString = "Press r to restart";
	RestartTexture = Engine::GetSpriteFont(static_cast<int>(Fonts::Font2)).DrawTextToTexture(restartString, 0xFFFFFFFF, true);

	gameObjectManagerPtr = new CS230::GameObjectManager();
	bumpercarPtr = new BumperCar(math::vec2{ Engine::GetWindow().GetSize().x / 2.0, Engine::GetWindow().GetSize().y / 2.0 });
	gameObjectManagerPtr->Add(bumpercarPtr);
	gameObjectManagerPtr->Add(new Apple());
	gameObjectManagerPtr->Add(new EnemyCar(bumpercarPtr, { 300,0 }, { 1000, 300 }, EnemyCarType::UpDown));
	gameObjectManagerPtr->Add(new EnemyCar(bumpercarPtr, EnemyCarType::Chasing));
	AddGSComponent(gameObjectManagerPtr);
	AddGSComponent(new Arena{ "Assets/Final/map.png" });

	//Emitters
	AddGSComponent(new HitEmitter());
	AddGSComponent(new MeteorBitEmitter());

#ifdef _DEBUG
	//collision
	AddGSComponent(new ShowCollision(CS230::InputKey::Keyboard::Tilde));
#endif
}

void Mode3::Update(double dt)
{
#ifdef _DEBUG
	if (modeReload.IsKeyReleased())
	{
		Engine::GetGameStateManager().ReloadState();
	}
	GetGSComponent<ShowCollision>()->Update(dt);
#else
	if (shipPtr->IsDead() == true)
	{
		if (modeReload.IsKeyReleased())
		{
			Engine::GetGameStateManager().ReloadState();
		}
	}
#endif
	if (modeNext.IsKeyReleased())
	{
		Engine::GetGameStateManager().SetNextState(static_cast<int>(Screens::MainMenu));
	}

	GetGSComponent<CS230::GameObjectManager>()->Update(dt);
}

void Mode3::Draw()
{
	Engine::GetWindow().Clear(0x97CAFF);
	GetGSComponent<Arena>()->Draw();

	GetGSComponent<CS230::GameObjectManager>()->DrawAll(cameraMatrix);

	math::ivec2 winSize = Engine::GetWindow().GetSize();
	GetGSComponent<Score>()->Draw(math::ivec2{ 10,  winSize.y - 5 });

	if (bumpercarPtr->IsDead() == true)
	{
		GameOverTexture.Draw(math::TranslateMatrix{ math::ivec2{(winSize.x - GameOverTexture.GetSize().x) / 2,winSize.y / 2} });
		RestartTexture.Draw(math::TranslateMatrix{ math::ivec2{(winSize.x - RestartTexture.GetSize().x) / 2,winSize.y / 3} });
	}
}

void Mode3::Unload()
{
	ClearGSComponent();
}