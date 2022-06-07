/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Timer.cpp
Project: CS230
Author: MINJAE YU
Creation date: 5/6/2022
-----------------------------------------------------------------*/
#include "Timer.h" //Timer
#include "..\Engine\Engine.h" //GetWindow
#include "Fonts.h" //Font

Timer::Timer(double time) : timer(time) {}

void Timer::Update(double dt)
{
	timer -= dt;
	std::string timerString = "Time: " + std::to_string(static_cast<int>(timer));
	timeTexture = Engine::GetSpriteFont(static_cast<int>(Fonts::Font1)).DrawTextToTexture(timerString, 0xFFFFFFFF, true);
}

void Timer::Draw(math::ivec2 location)
{
	math::ivec2 winSize = Engine::GetWindow().GetSize();
	timeTexture.Draw(math::TranslateMatrix(math::ivec2{ -timeTexture.GetSize() + location }));
}

bool Timer::hasEnded()
{
	if (timer <= 0)
	{
		return true;
	}
	return false;
}