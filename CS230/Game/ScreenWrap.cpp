/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: ScreenWrap.cpp
Project: CS230
Author: MINJAE YU
Creation date: 5/6/2022
-----------------------------------------------------------------*/
#include "ScreenWrap.h" //ScreenWrap
#include "..\Engine\Engine.h" //GetWindow

void ScreenWrap::Update(double)
{
	if (object.GetPosition().x + object.GetGOComponent<CS230::Sprite>()->GetFrameSize().x / static_cast<double>(2) < 0)
	{
		object.SetPosition(math::vec2{ Engine::GetWindow().GetSize().x + object.GetGOComponent<CS230::Sprite>()->GetFrameSize().x / static_cast<double>(2) ,object.GetPosition().y });
	}
	if (object.GetPosition().x - object.GetGOComponent<CS230::Sprite>()->GetFrameSize().x / static_cast<double>(2) > Engine::GetWindow().GetSize().x)
	{
		object.SetPosition(math::vec2{ -object.GetGOComponent<CS230::Sprite>()->GetFrameSize().x / static_cast<double>(2) ,object.GetPosition().y });
	}

	if (object.GetPosition().y + object.GetGOComponent<CS230::Sprite>()->GetFrameSize().y / static_cast<double>(2) < 0)
	{
		object.SetPosition(math::vec2{ object.GetPosition().x,Engine::GetWindow().GetSize().y + object.GetGOComponent<CS230::Sprite>()->GetFrameSize().y / static_cast<double>(2) });
	}
	if (object.GetPosition().y - object.GetGOComponent<CS230::Sprite>()->GetFrameSize().y / static_cast<double>(2) > Engine::GetWindow().GetSize().y)
	{
		object.SetPosition(math::vec2{ object.GetPosition().x,-object.GetGOComponent<CS230::Sprite>()->GetFrameSize().y / static_cast<double>(2) });
	}
}