/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Mode2.cpp
Project: CS230
Author: MINJAE YU
Creation date: 3/14/2022
-----------------------------------------------------------------*/
#include "Mode2.h" //Mode2
#include "../Engine/Engine.h" //GetGameStateManager
#include "Ship.h" //new Ship
#include "Meteor.h" //new Meteor
#include "Fonts.h" //Load Fonts
#include "Screens.h" //Go to MainMenu
#include "Score.h" //score
#include "../Engine/ShowCollision.h" //collision
#include "GameParticles.h" //HitEmitter, MeteorBitEmitter
#include "EnemyShip.h" //EnemyShip

Mode2::Mode2() : gameObjectManagerPtr(nullptr), shipPtr(nullptr),
#ifdef _DEBUG
	modeNext(CS230::InputKey::Keyboard::Escape), modeReload(CS230::InputKey::Keyboard::R), score(0)
#else
	modeNext(CS230::InputKey::Keyboard::Escape), modeReload(CS230::InputKey::Keyboard::R)
#endif
{}

void Mode2::Load()
{
	score = 0;
	AddGSComponent(new Score(score, Fonts::Font1));
	std::string gameoverString = "Game Over";
	GameOverTexture = Engine::GetSpriteFont(static_cast<int>(Fonts::Font2)).DrawTextToTexture(gameoverString, 0xFFFFFFFF, true);
	std::string restartString = "Press r to restart";
	RestartTexture = Engine::GetSpriteFont(static_cast<int>(Fonts::Font2)).DrawTextToTexture(restartString, 0xFFFFFFFF, true);

	gameObjectManagerPtr = new CS230::GameObjectManager();
	gameObjectManagerPtr->Add(new Meteor());
	gameObjectManagerPtr->Add(new Meteor());
	gameObjectManagerPtr->Add(new Meteor());
	gameObjectManagerPtr->Add(new Meteor());
	gameObjectManagerPtr->Add(new Meteor());
	shipPtr = new Ship(math::vec2{ Engine::GetWindow().GetSize().x / 2.0, Engine::GetWindow().GetSize().y / 2.0 });
	gameObjectManagerPtr->Add(shipPtr);
	gameObjectManagerPtr->Add(new EnemyShip(shipPtr));
	AddGSComponent(gameObjectManagerPtr);

	//Emitters
	AddGSComponent(new HitEmitter());
	AddGSComponent(new MeteorBitEmitter());

#ifdef _DEBUG
	//collision
	AddGSComponent(new ShowCollision(CS230::InputKey::Keyboard::Tilde));
#endif
}

void Mode2::Update(double dt)
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

void Mode2::Draw()
{
	Engine::GetWindow().Clear(0x000000FF);
	GetGSComponent<CS230::GameObjectManager>()->DrawAll(cameraMatrix);

	math::ivec2 winSize = Engine::GetWindow().GetSize();
	GetGSComponent<Score>()->Draw(math::ivec2{ 10,  winSize.y - 5 });

	if (shipPtr->IsDead() == true)
	{
		GameOverTexture.Draw(math::TranslateMatrix{ math::ivec2{(winSize.x - GameOverTexture.GetSize().x) / 2,winSize.y / 2} });
		RestartTexture.Draw(math::TranslateMatrix{ math::ivec2{(winSize.x - RestartTexture.GetSize().x) / 2,winSize.y / 3} });
	}
}

void Mode2::Unload()
{
	ClearGSComponent();
}