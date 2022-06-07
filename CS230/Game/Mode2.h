/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Mode2.h
Project: CS230
Author: MINJAE YU
Creation date: 3/14/2022
-----------------------------------------------------------------*/
#pragma once
#include "..\Engine\GameState.h" //inheritance
#include "..\Engine\Input.h" //key input
#include "..\Engine\GameObjectManager.h" //Game Object
#include "..\Engine\TransformMatrix.h" //Camera Matrix
#include "..\Engine\Texture.h" //scoreTexture

class Ship;

class Mode2 : public CS230::GameState
{
public:
	Mode2();
	void Load() override;
	void Update(double dt) override;
	void Unload() override;
	void Draw() override;

	std::string GetName() override { return "Mode2"; }
private:
#ifdef _DEBUG
	CS230::InputKey modeNext;
	CS230::InputKey modeReload;
#else
	CS230::InputKey modeNext;
	CS230::InputKey modeReload;
#endif
	Ship* shipPtr;
	CS230::GameObjectManager* gameObjectManagerPtr;
	math::TransformMatrix cameraMatrix;

	int score;
	CS230::Texture scoreTexture;
	CS230::Texture GameOverTexture;
	CS230::Texture RestartTexture;
};