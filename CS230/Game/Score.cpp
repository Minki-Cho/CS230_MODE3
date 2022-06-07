/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Score.cpp
Project: CS230
Author: MINJAE YU
Creation date: 5/6/2022
-----------------------------------------------------------------*/
#include "Score.h" //Score
#include "..\Engine\Engine.h" //GetWindow
#include "Fonts.h" //Font

Score::Score(int startingScore, Fonts fontToUse) : score(startingScore), fontToUse(fontToUse)
{}

void Score::AddScore(int newPoints)
{
	score += newPoints;
}

void Score::Draw(math::ivec2 location)
{
	RenderText();
	scoreTexture.Draw(math::TranslateMatrix(math::ivec2{ location.x, -scoreTexture.GetSize().y + location.y }));
}

void Score::RenderText()
{
	std::string scoreString = "Score: " + std::to_string(score / 100) + std::to_string((score % 100) / 10) + std::to_string(score % 10);
	scoreTexture = Engine::GetSpriteFont(static_cast<int>(fontToUse)).DrawTextToTexture(scoreString, 0xFFFFFFFF, true);
}