/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Mode1.cpp
Project: CS230
Author: MINJAE YU
Creation date: 3/14/2022
-----------------------------------------------------------------*/
#include "Mode1.h" //Mode1
#include "../Engine/Engine.h" //GetGameStateManager
#include "Screens.h" //next state
#include "Hero.h" //new Hero
#include "Ball.h" //new Ball
#include "TreeStump.h" //new TreeStump
#include "Bunny.h" //new Bunny
#include "Fonts.h" //Load Fonts
#include "Score.h" //score
#include "Timer.h" //timer
#include "../Engine/ShowCollision.h" //collision
#include "Floor.h" //new Floor
#include "Exit.h" //new Exit
#include "GameParticles.h" //SmokeEmitter

Mode1::Mode1() : heroPtr(nullptr), gameObjectManagerPtr(nullptr), backgroundsPtr(nullptr), score(0), lives(3), timer(0),
#ifdef _DEBUG
	modeNext(CS230::InputKey::Keyboard::Escape), modeReload(CS230::InputKey::Keyboard::R)
#else
	modeNext(CS230::InputKey::Keyboard::Escape)
#endif
{}

void Mode1::Load()
{
	score = 0;
	timer = 60;

	//Fonts
	AddGSComponent(new Score(score, Fonts::Font1));
	std::string livesString = "Lives: " + std::to_string(lives);
	livesTexture = Engine::GetSpriteFont(static_cast<int>(Fonts::Font1)).DrawTextToTexture(livesString, 0xFFFFFFFF, true);
	AddGSComponent(new Timer(timer));

	//gameObjects
	gameObjectManagerPtr = new CS230::GameObjectManager();
	gameObjectManagerPtr->Add(new Ball({ 600, Mode1::floor }));
	gameObjectManagerPtr->Add(new Ball({ 2700, Mode1::floor }));
	gameObjectManagerPtr->Add(new Ball({ 4800, Mode1::floor }));
	gameObjectManagerPtr->Add(new TreeStump({ 300, Mode1::floor }, 3));
	gameObjectManagerPtr->Add(new TreeStump({ 1200, Mode1::floor }, 2));
	gameObjectManagerPtr->Add(new TreeStump({ 2200, Mode1::floor }, 1));
	gameObjectManagerPtr->Add(new TreeStump({ 2800, Mode1::floor }, 5));
	gameObjectManagerPtr->Add(new TreeStump({ 5100, Mode1::floor }, 5));
	gameObjectManagerPtr->Add(new Floor({ {0, 0}, {1471, static_cast<int>(Mode1::floor)} }));
	gameObjectManagerPtr->Add(new Floor({ {1602, 0}, {4262, static_cast<int>(Mode1::floor)} }));
	gameObjectManagerPtr->Add(new Floor({ {4551, 0}, {5760, static_cast<int>(Mode1::floor)} }));
	gameObjectManagerPtr->Add(new Exit({ {5550, static_cast<int>(Mode1::floor)}, {5760, 683} }));
	AddGSComponent(gameObjectManagerPtr);
	heroPtr = new Hero({ 100, Mode1::floor - 1 });
	gameObjectManagerPtr->Add(heroPtr);
	gameObjectManagerPtr->Add(new Bunny({ 1000, floor }, { 674, 1132 }, heroPtr));
	gameObjectManagerPtr->Add(new Bunny({ 2000, floor }, { 1635, 2135 }, heroPtr));
	gameObjectManagerPtr->Add(new Bunny({ 3200, floor }, { 2860, 4250 }, heroPtr));
	gameObjectManagerPtr->Add(new Bunny({ 3800, floor }, { 2860, 4250 }, heroPtr));

	//SmokeEmitter
	AddGSComponent(new SmokeEmitter());

	//backgrounds
	backgroundsPtr = new Background();
	backgroundsPtr->Add("Assets/clouds.png", 4);
	backgroundsPtr->Add("Assets/Mountains.png", 2);
	backgroundsPtr->Add("Assets/foreground.png", 1);
	AddGSComponent(backgroundsPtr);

	//camera
	CS230::Camera* cameraPtr = new CS230::Camera({ { 0.15 * Engine::GetWindow().GetSize().x, 0 }, {0.35 * Engine::GetWindow().GetSize().x, 0 } });
	AddGSComponent(cameraPtr);
	cameraPtr->SetExtent({ { 0,0 }, { GetGSComponent<Background>()->Size() } });
#ifdef _DEBUG
	//collision
	AddGSComponent(new ShowCollision(CS230::InputKey::Keyboard::Tilde));
#endif
}

void Mode1::Update(double dt)
{
#ifdef _DEBUG
	if (modeReload.IsKeyReleased()) {
		Engine::GetGameStateManager().ReloadState();
	}
	GetGSComponent<ShowCollision>()->Update(dt);
#endif
	if (modeNext.IsKeyReleased()) {
		Engine::GetGameStateManager().SetNextState(static_cast<int>(Screens::MainMenu));
	}
	GetGSComponent<CS230::GameObjectManager>()->Update(dt);
	GetGSComponent<CS230::Camera>()->Update(heroPtr->GetPosition());

	//Timer Update
	GetGSComponent<Timer>()->Update(dt);
	if (GetGSComponent<Timer>()->hasEnded() == true)
	{
		lives--;
		if (lives == 0)
		{
			lives = 3;
			Engine::GetGameStateManager().SetNextState(static_cast<int>(Screens::MainMenu));
		}
		else
		{
			Engine::GetGameStateManager().ReloadState();
		}
	}

	//isDead
	if (heroPtr->IsDead() == true)
	{
		lives--;
		if (lives == 0)
		{
			lives = 3;
			Engine::GetGameStateManager().SetNextState(static_cast<int>(Screens::MainMenu));
		}
		else
		{
			Engine::GetGameStateManager().ReloadState();
		}
	}
}

void Mode1::Draw()
{
	Engine::GetWindow().Clear(0x97CAFF);
	CS230::Camera* cameraPtr = GetGSComponent<CS230::Camera>();
	math::TransformMatrix cameraMatrix = cameraPtr->GetMatrix();
	//background
	GetGSComponent<Background>()->Draw(*cameraPtr);
	//objects
	GetGSComponent<CS230::GameObjectManager>()->DrawAll(cameraMatrix);

	math::ivec2 winSize = Engine::GetWindow().GetSize();
	GetGSComponent<Score>()->Draw(math::ivec2{ 10,  winSize.y - 5 });
	livesTexture.Draw(math::TranslateMatrix(math::ivec2{ (winSize.x - livesTexture.GetSize().x) / 2, winSize.y - livesTexture.GetSize().y - 5 }));
	GetGSComponent<Timer>()->Draw(math::ivec2{ winSize.x - 10, winSize.y - 5 });
}

void Mode1::Unload()
{
	ClearGSComponent();
}