/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Ship.h
Project: CS230
Author: MINJAE YU
Creation date: 3/21/2022
-----------------------------------------------------------------*/
#pragma once
#include "..\Engine\Sprite.h" //Sprite
#include "..\Engine\Input.h" //key input
#include "..\Engine\Vec2.h" //Vec2 variables
#include "..\Engine\\GameObject.h" //Game Object

class Ship : public CS230::GameObject
{
public:
    Ship(math::vec2 startPos);
    void Update(double dt) override;
    void Draw(math::TransformMatrix cameraMatrix) override;

    GameObjectType GetObjectType() override;
    std::string GetObjectTypeName() override;
    bool CanCollideWith(GameObjectType objectBType) override;
    void ResolveCollision(CS230::GameObject* objectB) override;

    bool IsDead() { return isDead; }
private:
    static constexpr double accel{ 400 };
    static constexpr double drag{ 1.0f };

    CS230::InputKey rotateCounterKey;
    CS230::InputKey rotateClockKey;
    CS230::InputKey accelerateKey;
    CS230::InputKey accelerateReverseKey;
    CS230::InputKey laserKey;

    double rotate_speed{ 7 };

    CS230::Sprite flame1;
    CS230::Sprite flame2;

    bool is_Accelerating{ false };

    bool isDead;
};