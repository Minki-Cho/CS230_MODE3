/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Camera.cpp
Project: CS230
Author: MINJAE YU
Creation date: 3/29/2022
-----------------------------------------------------------------*/
#include "Camera.h" //Camera
#include "..\Engine\Engine.h" //GetWindow

CS230::Camera::Camera(math::rect2 movableRange) : movableRange(movableRange),
extent(math::irect2{}), position(math::vec2{ 0,0 })
{}

void CS230::Camera::SetPosition(math::vec2 newPosition)
{
	position = newPosition;
}

const math::vec2& CS230::Camera::GetPosition() const
{
	return position;
}

void CS230::Camera::SetExtent(math::irect2 newExtent)
{
	extent = newExtent;
}

void CS230::Camera::Update(const math::vec2& followObjPos)
{
	//outside of movable
	//camera from 0 to movableRange
	if (followObjPos.x < position.x + movableRange.Left())
	{
		position.x = followObjPos.x - movableRange.Left();
	}
	else if (followObjPos.x > position.x + movableRange.Right())
	{
		position.x = followObjPos.x - movableRange.Right();
	}
	//outside of extent
	if (position.x < extent.Left())
	{
		position.x = extent.Left();
	}
	if (position.x + Engine::GetWindow().GetSize().x > extent.Right())
	{
		position.x = extent.Right() - Engine::GetWindow().GetSize().x;
	}
	if (position.y < extent.Bottom())
	{
		position.y = extent.Bottom();
	}
	if (position.y > extent.Top())
	{
		position.y = extent.Top();
	}
}

math::TransformMatrix CS230::Camera::GetMatrix()
{
	math::TranslateMatrix cameraMatrix(-position);
	return cameraMatrix;
}