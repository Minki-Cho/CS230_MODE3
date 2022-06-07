/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Meteor.h
Project: CS230
Author: Kevin Wright
Creation date: 2/15/2021
-----------------------------------------------------------------*/
#pragma once
#include "..\Engine\GameObject.h" //inheritance

class Meteor : public CS230::GameObject
{
public:
	Meteor();
	Meteor(Meteor& original);
	void Update(double dt) override;

	GameObjectType GetObjectType() override;
	std::string GetObjectTypeName() override;

	void ResolveCollision(GameObject* objectB) override;
private:
	int health;
	int size;

	constexpr static double PI{ 3.141592 };
	const double velocity_MIN{ -100.0 };
	const double velocity_MAX{ 100.0 };

	const double scale_MIN{ 0.75 };
	const double scale_MAX{ 1.0 };

	template<typename Type>
	Type scoped_random(double min, double max)
	{
		return static_cast<Type>(min + (rand() / (RAND_MAX / (max - min))));
	}
};