/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: TreeStump.cpp
Project: CS230
Author: Kevin Wright
Creation date: 4/28/2022
-----------------------------------------------------------------*/
#include "TreeStump.h" //TreeStump
#include "GameObjectTypes.h" //return GameObjectTypes

TreeStump::TreeStump(math::vec2 pos, int size) : GameObject(pos)
{
	switch (size)
	{
	case 1:
		AddGOComponent(new CS230::Sprite("Assets/treeStump1.spt", this));
		break;
	case 2:
		AddGOComponent(new CS230::Sprite("Assets/treeStump2.spt", this));
		break;
	case 3:
		AddGOComponent(new CS230::Sprite("Assets/treeStump3.spt", this));
		break;
	case 5:
		AddGOComponent(new CS230::Sprite("Assets/treeStump5.spt", this));
		break;
	}
}

GameObjectType TreeStump::GetObjectType()
{
	return GameObjectType::TreeStump;
}

std::string TreeStump::GetObjectTypeName()
{
	return "TreeStump";
}