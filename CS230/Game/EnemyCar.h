#pragma once
#include "..\Engine\GameObject.h" //GameObject inheritance

class EnemyCar : public CS230::GameObject
{
public:
    EnemyCar(GameObject* player);
    void Update(double dt);
    void Draw(math::TransformMatrix displayMatrix);

    GameObjectType GetObjectType() override;
    std::string GetObjectTypeName() override;
    bool CanCollideWith(GameObjectType collideAgainstType) override;
    void ResolveCollision(GameObject* collidedWith);

private:
    GameObject* player;

    static constexpr double accel = 50;
    static constexpr double drag = 1;
    static constexpr double rotationRate = 3;

    const double PI{ 3.141592 };
    double GetRandom(double min, double max);
};