/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: BumperCar.h
Project: CS230
Author: MINJAE YU, MINKI CHO
Creation date: 6/7/2022
-----------------------------------------------------------------*/
#pragma once
#include "..\Engine\Sprite.h" //Sprite
#include "..\Engine\Input.h" //key input
#include "..\Engine\Vec2.h" //Vec2 variables
#include "..\Engine\\GameObject.h" //Game Object

class BumperCar : public CS230::GameObject
{
public:
    BumperCar(math::vec2 startPos);
    void Update(double dt) override;
    void Draw(math::TransformMatrix cameraMatrix) override;

    GameObjectType GetObjectType() override;
    std::string GetObjectTypeName() override;
    bool CanCollideWith(GameObjectType objectBType) override;
    void ResolveCollision(CS230::GameObject* objectB) override;

    bool IsDead() { return isDead; }
private:
    double velocity;
    static constexpr double increase_val{ 80 };

    CS230::InputKey rotateCounterKey;
    CS230::InputKey rotateClockKey;

    CS230::Sprite light;

    double rotate_speed{ 7 };

    bool isDead;
    bool isOut;
};