/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: GameObjectManager.cpp
Project: CS230
Author: MINJAE YU
Creation date: 4/27/2022
-----------------------------------------------------------------*/
#include "GameObjectManager.h" //GameObjectManager
#include "GameObject.h" //objects
#include "TransformMatrix.h" //Draw
#include "Engine.h" //Getlogger
#include "Collision.h" //CollideTest

CS230::GameObjectManager::~GameObjectManager()
{
	for (GameObject* objects : gameObjects)
	{
		delete objects;
		objects = nullptr;
	}
	gameObjects.clear();
}

void CS230::GameObjectManager::Add(GameObject* obj)
{
	gameObjects.push_back(obj);
}

void CS230::GameObjectManager::Update(double dt)
{
	std::list<GameObject*> DestroyList;
	for (GameObject* objects : gameObjects)
	{
		objects->Update(dt);
		if (objects->GetDestroyed() == true)
		{
			DestroyList.push_back(objects);
		}
	}
	for (GameObject* destroyObject : DestroyList)
	{
		gameObjects.remove(destroyObject);
		delete destroyObject;
	}
}

void CS230::GameObjectManager::DrawAll(math::TransformMatrix& cameraMatrix)
{
	for (GameObject* objects : gameObjects)
	{
		objects->Draw(cameraMatrix);
	}
}

void CS230::GameObjectManager::CollideTest()
{
	for (GameObject* objectA : gameObjects)
	{
		for (GameObject* objectB : gameObjects)
		{
			if (objectA->GetGOComponent<CS230::Collision>() != nullptr && objectB->GetGOComponent<CS230::Collision>() != nullptr)
			{
				if (objectA->CanCollideWith(objectB->GetObjectType()))
				{
					if (objectA->DoesCollideWith(objectB) == true)
					{
						if(objectA->GetObjectTypeName() != objectB->GetObjectTypeName())
						Engine::GetLogger().LogEvent("Collision Detected: " + objectA->GetObjectTypeName() + " and " + objectB->GetObjectTypeName());
						objectA->ResolveCollision(objectB);
					}
				}
			}
		}
	}
}