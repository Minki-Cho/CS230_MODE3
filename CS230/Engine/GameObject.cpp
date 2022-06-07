/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: GameObject.cpp
Project: CS230
Author: MINJAE YU
Creation date: 4/28/2022
-----------------------------------------------------------------*/
#include "GameObject.h" //GameObject
#include "Collision.h" //GetGOComponent
#include "ShowCollision.h" //GetGSComponent
#include "Engine.h" //GetGSComponent

CS230::GameObject::GameObject(math::vec2 position) : GameObject(position, 0, { 1, 1 }) {}

CS230::GameObject::GameObject(math::vec2 position, double rotation, math::vec2 scale)
    : velocity{ 0,0 }, position(position), updateMatrix(true),
    scale(scale), rotation(rotation), currState(&state_nothing) {}

CS230::GameObject::~GameObject()
{
    components.Clear();
}

void CS230::GameObject::Update(double dt)
{
    currState->Update(this, dt);
    if (velocity.x != 0 || velocity.y != 0)
    {
        UpdatePosition(velocity * dt);
    }
    UpdateGOComponents(dt);
    currState->TestForExit(this);
}

void CS230::GameObject::Draw(math::TransformMatrix cameraMatrix)
{
    Sprite* spritePtr = GetGOComponent<Sprite>();
    if (spritePtr != nullptr)
    {
        spritePtr->Draw(cameraMatrix * GetMatrix());
    }
    if (Engine::GetGSComponent<ShowCollision>() != nullptr)
    {
        if (Engine::GetGSComponent<ShowCollision>()->IsEnabled() == true)
        {
            if (GetGOComponent<Collision>() != nullptr)
                GetGOComponent<Collision>()->Draw(cameraMatrix);
        }
    }
}

const math::TransformMatrix& CS230::GameObject::GetMatrix()
{
    if (updateMatrix == true)
    {
        objectMatrix = math::TranslateMatrix(position) * math::RotateMatrix(rotation) * math::ScaleMatrix(scale);
        updateMatrix = false;
    }
    return objectMatrix;
}

const math::vec2& CS230::GameObject::GetPosition() const { return position; }

const math::vec2& CS230::GameObject::GetVelocity() const { return velocity; }

const math::vec2& CS230::GameObject::GetScale() const { return scale; }

double CS230::GameObject::GetRotation() const { return rotation; };

void CS230::GameObject::SetPosition(math::vec2 newPosition)
{
    position = newPosition;
    updateMatrix = true;
}

bool CS230::GameObject::GetDestroyed()
{
    return should_destroyed;
}

void CS230::GameObject::SetDestroyed(bool b)
{
     should_destroyed = b;
}

bool CS230::GameObject::CanCollideWith(GameObjectType)
{
    return false;
}

bool CS230::GameObject::DoesCollideWith(GameObject* objectB)
{
    if (GetGOComponent<Collision>() != nullptr && objectB->GetGOComponent<Collision>() != nullptr)
    {
        return GetGOComponent<Collision>()->DoesCollideWith(objectB);
    }
    return false;
}

bool CS230::GameObject::DoesCollideWith(math::vec2 point)
{
    if (GetGOComponent<Collision>() != nullptr)
    {
        return GetGOComponent<Collision>()->DoesCollideWith(point);
    }
    return false;
}

void CS230::GameObject::ResolveCollision(GameObject*)
{
    Engine::GetLogger().LogError("This function should not be called!");
}

void CS230::GameObject::UpdatePosition(math::vec2 adjustPosition)
{
    position += adjustPosition;
    updateMatrix = true;
}

void CS230::GameObject::SetVelocity(math::vec2 newVelocity)
{
    velocity = newVelocity;
}

void CS230::GameObject::UpdateVelocity(math::vec2 adjustPosition)
{
    velocity += adjustPosition;
    updateMatrix = true;
}

void CS230::GameObject::SetScale(math::vec2 newScale)
{
    scale = newScale;
}

void CS230::GameObject::SetRotation(double newRotationAmount)
{
    rotation = newRotationAmount;
}

void CS230::GameObject::UpdateRotation(double newRotationAmount)
{
    rotation += newRotationAmount;
    updateMatrix = true;
}

void CS230::GameObject::ChangeState(State* newState) {
    currState = newState;
    currState->Enter(this);
}