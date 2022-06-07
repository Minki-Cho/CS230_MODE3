/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Splash.cpp
Project: CS230
Author: Kevin Wright
Creation date: 2/10/2021
-----------------------------------------------------------------*/
#include "../Engine/Engine.h"	//GetGameStateManager
#include "Screens.h" //next state
#include "../Engine/Camera.h" //camera
#include "Splash.h" //Splash

Splash::Splash() : modeNext(CS230::InputKey::Keyboard::Enter), texturePtr(nullptr) {}

void Splash::Load()
{
	texturePtr = Engine::GetTextureManager().Load("Assets/DigiPen_BLACK_1024px.png");
}
void Splash::Update(double dt)
{
	//dt = 0.033...
	time_count += dt;
	if (time_count >= 3.0)
	{
		Engine::GetGameStateManager().SetNextState(static_cast<int>(Screens::MainMenu));
		time_count = 0;
	}
}

void Splash::Unload() {}

void Splash::Draw()
{
	Engine::GetWindow().Clear(0xFFFFFFFF);
	texturePtr->Draw(math::TranslateMatrix{ math::vec2{ ((Engine::GetWindow().GetSize() - texturePtr->GetSize()) / 2.0)} });
}