/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name:
Project: CS230
Author:
Creation date: 6/7/2022
-----------------------------------------------------------------*/

#pragma once

#include "..\Engine\GameObject.h" //inheritance
#include "BumperCar.h"

class Apple : public CS230::GameObject
{
public:
	Apple();

	GameObjectType GetObjectType() override;
	std::string GetObjectTypeName() override;
	void ResolveCollision(GameObject* objectB) override;
	void Update(double dt) override;
private:
	//bool Isdead;
};