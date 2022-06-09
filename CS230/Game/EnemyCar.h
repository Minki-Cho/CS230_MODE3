#pragma once
#include "..\Engine\GameObject.h" //GameObject inheritance
#include "EnemyCarType.h"

class EnemyCar : public CS230::GameObject
{
public:
    EnemyCar(GameObject* player, math::vec2 position, std::vector<double> moveableNodes, EnemyCarType type);
    EnemyCar(GameObject* player, EnemyCarType type);
    void Update(double dt);
    //void Draw(math::TransformMatrix displayMatrix);

    GameObjectType GetObjectType() override;
    std::string GetObjectTypeName() override;
    bool CanCollideWith(GameObjectType collideAgainstType) override;
    void ResolveCollision(GameObject* collidedWith);

private:
    GameObject* player;
    math::vec2 position;
    bool changeDir;
    static constexpr double accel = 70;
    static constexpr double drag = 0.1;
    static constexpr double rotationRate = 50;

    const double PI{ 3.141592 };
    std::vector<double> moveableNodes;
    math::vec2 speed = { 200,200 };
    EnemyCarType type;
};