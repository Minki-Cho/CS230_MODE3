/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: MainMenu.cpp
Project: CS230
Author: MINJAE YU
Creation date: 5/2/2022
-----------------------------------------------------------------*/
#include "MainMenu.h" //MainMenu
#include "../Engine/Engine.h" //GetSpriteFont
#include "Fonts.h" //Fonts
#include "Screens.h" //Select Mode

MainMenu::MainMenu()
	: upKey(CS230::InputKey::Keyboard::Up), downKey(CS230::InputKey::Keyboard::Down), selectKey(CS230::InputKey::Keyboard::Enter), selectedIndex(0)
{}

void MainMenu::Load()
{
	title = Engine::GetSpriteFont(static_cast<int>(Fonts::Font2)).DrawTextToTexture("CS230 Project", 0xFFFFFFFF, true);
	for (OptionData& data : optionsData)
	{
		RenderOption(data, false);
	}
	selectedIndex = static_cast<int>(Options::Mode1);
	RenderOption(optionsData[selectedIndex], true);
}

void MainMenu::Update(double)
{
	if (upKey.IsKeyReleased() == true)
	{
		if (selectedIndex != static_cast<int>(Options::Mode1))
		{
			RenderOption(optionsData[selectedIndex], false);
			selectedIndex--;
		}
	}
	if (downKey.IsKeyReleased() == true)
	{
		if (selectedIndex != static_cast<int>(Options::Quit))
		{
			RenderOption(optionsData[selectedIndex], false);
			selectedIndex++;
		}
	}
	RenderOption(optionsData[selectedIndex], true);

	if (selectKey.IsKeyReleased() == true)
	{
		switch(selectedIndex)
		{
		case 0:
			Engine::GetGameStateManager().SetNextState(static_cast<int>(Screens::Mode1));
			break;
		case 1:
			Engine::GetGameStateManager().SetNextState(static_cast<int>(Screens::Mode2));
			break;
		case 2:
			Engine::GetGameStateManager().Shutdown();
			break;
		}
	}
}

void MainMenu::Unload() {}

void MainMenu::Draw()
{
	Engine::GetWindow().Clear(0x97CAFF);
	math::ivec2 winSize = Engine::GetWindow().GetSize();
	title.Draw(math::TranslateMatrix{ math::ivec2{winSize.x / 2 - title.GetSize().x, (winSize.y + title.GetSize().y) / 2} } *math::ScaleMatrix{ math::ivec2{2,2} });

	for (int i = 0; i < static_cast<int>(Options::Count); i++)
	{
		optionsData[i].texture.Draw(math::TranslateMatrix{ math::ivec2{(winSize.x - optionsData[i].texture.GetSize().x) / 2, winSize.y / 2 - optionsData[i].texture.GetSize().y - (i * optionsData[i].texture.GetSize().y)} });
	}
}

void MainMenu::RenderOption(OptionData& data, bool isHighlighted)
{
	data.texture = Engine::GetSpriteFont(static_cast<int>(Fonts::Font1)).DrawTextToTexture(
		data.text, (isHighlighted == true) ? MainMenu::onColor : MainMenu::offColor, true);
}

MainMenu::OptionData MainMenu::optionsData[static_cast<int>(MainMenu::Options::Count)] =
{
	{"Side Scroller", {0.5, 0.45}, {} },
	{"Space Shooter", {0.5, 0.35}, {} },
	{"Quit", {0.5, 0.25}, {} },
};