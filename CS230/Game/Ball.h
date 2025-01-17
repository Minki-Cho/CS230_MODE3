/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Ball.h
Project: CS230
Author: Kevin Wright
Creation date: 2/14/2021
-----------------------------------------------------------------*/
#pragma once
#include "..\Engine\\GameObject.h" //Game Object

namespace CS230
{
    class Camera;
}

class Ball : public CS230::GameObject
{
public:
    Ball(math::vec2 startPos);

    GameObjectType GetObjectType() override;
    std::string GetObjectTypeName() override;

    //collision
    bool CanCollideWith(GameObjectType objectBType) override;
    void ResolveCollision(GameObject* objectB) override;
private:
    class State_Bounce : public State
    {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, double dt) override;
        virtual void TestForExit(GameObject* object) override;
        std::string GetName() override { return "Bounce"; }
    };

    class State_Land : public State
    {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, double dt) override;
        virtual void TestForExit(GameObject* object) override;
        std::string GetName() override { return "Land"; }
    };

    State_Bounce stateBounce;
    State_Land stateLand;

    static constexpr double bounceVelocity{ 750 };
};