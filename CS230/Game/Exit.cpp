/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Exit.cpp
Project: CS230
Author: MINJAE YU
Creation date: 5/19/2022
-----------------------------------------------------------------*/
#include "Exit.h" //Exit
#include "../Engine/Rect.h" //irect2
#include "../Engine/Engine.h" //GetGameStateManager
#include "Screens.h" //MainMenu
#include "../Engine/Collision.h" //RectCollision

Exit::Exit(math::irect2 rect) : GameObject(math::vec2{ rect.point1 })
{
	AddGOComponent(new CS230::RectCollision(math::irect2{ rect.Size() }, this));
}

void Exit::ResolveCollision(GameObject* objectA)
{
	if (objectA->GetObjectType() == GameObjectType::Hero)
	{
		Engine::GetGameStateManager().SetNextState(static_cast<int>(Screens::MainMenu));
	}
}