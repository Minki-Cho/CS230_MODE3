/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Hero.h
Project: CS230
Author: Kevin Wright
Creation date: 2/11/2021
-----------------------------------------------------------------*/
#pragma once
#include "..\Engine\Sprite.h" //Sprite
#include "..\Engine\Input.h" //key input
#include "..\Engine\Vec2.h" //Vec2 variables
#include "..\Engine\GameObject.h" //Game Object

class Hero : public CS230::GameObject
{
public:
    Hero(math::vec2 startPos);
    void Update(double dt) override;
    void Draw(math::TransformMatrix displayMatrix);
    const math::vec2& GetPosition() const;

    bool IsDead() { return isDead; }

    //collision
    GameObjectType GetObjectType() override;
    std::string GetObjectTypeName() override;
    bool CanCollideWith(GameObjectType objectBType) override;
    void ResolveCollision(GameObject* objectB) override;
private:
    CS230::InputKey moveLeftKey;
    CS230::InputKey moveRightKey;
    CS230::InputKey moveUpKey;

    static constexpr double x_accel{ 500 };
    static constexpr double x_drag{ 750 };
    static constexpr double max_x_vel{ 500 };
    static constexpr double jumpVelocity{ 800 };

    static constexpr double hurtTime = 2;
    double hurtTimer;
    bool drawHero;
    double deadline{ -300 };

    class State_Idle : public State
    {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, double dt) override;
        virtual void TestForExit(GameObject* object) override;
        std::string GetName() override { return "Idle"; }
    };
    class State_Running : public State
    {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, double dt) override;
        virtual void TestForExit(GameObject* object) override;
        std::string GetName() override { return "Running"; }
    };
    class State_Skidding : public State
    {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, double dt) override;
        virtual void TestForExit(GameObject* object) override;
        std::string GetName() override { return "Skidding"; }
    };
    class State_Jumping : public State
    {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, double dt) override;
        virtual void TestForExit(GameObject* object) override;
        std::string GetName() override { return "Jumping"; }
    };
    class State_Falling : public State
    {
    public:
        virtual void Enter(GameObject* object) override;
        virtual void Update(GameObject* object, double dt) override;
        virtual void TestForExit(GameObject* object) override;
        std::string GetName() override { return "Falling"; }
    };
    State_Idle stateIdle;
    State_Running stateRunning;
    State_Skidding stateSkidding;
    State_Jumping stateJumping;
    State_Falling stateFalling;

    bool isDead;
    GameObject* standingOnObject;

    void UpdateXVelocity(double dt); //Change X velocity stuff
};