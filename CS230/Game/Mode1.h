/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Mode1.h
Project: CS230
Author: MINJAE YU
Creation date: 3/14/2022
-----------------------------------------------------------------*/
#pragma once
#include "..\Engine\GameState.h" //inheritance
#include "..\Engine\Input.h" //key input
#include "Background.h" //call background
#include "..\Engine\Camera.h" //get camera
#include "..\Engine\GameObjectManager.h" //Game Object

class Hero;

class Mode1 : public CS230::GameState
{
public:
	Mode1();
	void Load() override;
	void Update(double dt) override;
	void Unload() override;
	void Draw() override;

	std::string GetName() override { return "Mode1"; }

	static constexpr double gravity{ 1100 };
private:
#ifdef _DEBUG
	CS230::InputKey modeNext;
	CS230::InputKey modeReload;
#else
	CS230::InputKey modeNext;
#endif
	Background* backgroundsPtr;
	CS230::GameObjectManager* gameObjectManagerPtr;
	Hero* heroPtr;

	static constexpr double floor = 126.0f;

	int score;
	int lives;
	double timer;
	CS230::Texture livesTexture;
};