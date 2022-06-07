/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Floor.cpp
Project: CS230
Author: MINJAE YU
Creation date: 5/19/2022
-----------------------------------------------------------------*/
#include "Floor.h" //Floor
#include "../Engine/Rect.h" //irect2
#include "../Engine/Collision.h" //RectCollision

Floor::Floor(math::irect2 rect) : GameObject(math::vec2{ rect.point1 })
{
	AddGOComponent(new CS230::RectCollision(math::irect2{ rect.Size() }, this));
}