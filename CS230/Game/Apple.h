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
	bool CanCollideWith(GameObjectType objectBType) override;
	void ResolveCollision(GameObject* objectB) override;
	void Update(double dt) override;
private:
	//bool Isdead;
	double random_helper(double min, double max){return min + rand() / (RAND_MAX / (max - min));}
	math::vec2 random_val;
};