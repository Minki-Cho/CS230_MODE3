/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: main.cpp
Project: CS230
Author: Kevin Wright
Creation date: 2/10/2021
-----------------------------------------------------------------*/
#include <stdexcept>	//std::exception
#include <iostream>		//std::cerr

#include "Engine/Engine.h" //engine
#include "Game/Splash.h" //splash
#include "Game/Mode1.h" //mode1
#include "Game/Mode2.h" //mode2
#include "Game/MainMenu.h" //menu


int main(void)
{
	try
	{
		Engine& engine = Engine::Instance();
		engine.Init("Week FINAL - Minjae Yu, Minki Cho");
		Splash splash;
		Mode1 mode1;
		Mode2 mode2;
		MainMenu menu;

		engine.AddSpriteFont("assets/font.png");
		engine.AddSpriteFont("assets/font2.png");
		engine.GetGameStateManager().AddGameState(splash);
		engine.GetGameStateManager().AddGameState(menu);
		engine.GetGameStateManager().AddGameState(mode1);
		engine.GetGameStateManager().AddGameState(mode2);

		while (engine.HasGameEnded() == false)
		{
			engine.Update();
		}
		engine.Shutdown();
		return 0;
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << "\n";
		return -1;
	}
}