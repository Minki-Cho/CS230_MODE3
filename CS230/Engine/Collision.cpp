/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Collision.cpp
Project: CS230
Author: MINJAE YU
Creation date: 5/9/2022
-----------------------------------------------------------------*/
#include "Collision.h" //Collision
#include <doodle/drawing.hpp> //draw_rectangle
#include "TransformMatrix.h" //TransformMatrix
#include "GameObject.h" //GameObject

void CS230::RectCollision::Draw(math::TransformMatrix cameraMatrix)
{
    doodle::no_fill();
    doodle::set_outline_width(2);
    doodle::set_outline_color(doodle::HexColor(0xFFFFFFFF));
    doodle::push_settings();
    math::rect2 displayRect = GetWorldCoorRect();
    doodle::apply_matrix(cameraMatrix[0][0], cameraMatrix[1][0], cameraMatrix[0][1], cameraMatrix[1][1], cameraMatrix[0][2], cameraMatrix[1][2]);
    doodle::draw_rectangle(displayRect.Left(), displayRect.Bottom(), displayRect.Size().x, displayRect.Size().y);
    doodle::pop_settings();
}

math::rect2 CS230::RectCollision::GetWorldCoorRect()
{
    return
    {
        objectPtr->GetMatrix() * static_cast<math::vec2>(rect.point1),
        objectPtr->GetMatrix() * static_cast<math::vec2>(rect.point2)
    };
}

bool CS230::RectCollision::DoesCollideWith(GameObject* objectB)
{
    if (objectB->GetGOComponent<Collision>() != nullptr && objectB->GetGOComponent<Collision>()->GetCollideType() == CollideType::Rect_Collide)
    {
        math::rect2 objectB_Rect = objectB->GetGOComponent<CS230::RectCollision>()->GetWorldCoorRect();
        if (GetWorldCoorRect().Right() > objectB_Rect.Left() && GetWorldCoorRect().Left() < objectB_Rect.Right())
        {
            if (GetWorldCoorRect().Top() > objectB_Rect.Bottom() && GetWorldCoorRect().Bottom() < objectB_Rect.Top())
            {
                return true;
            }
        }
    }
    return false;
}

bool CS230::RectCollision::DoesCollideWith(math::vec2 point)
{
    if (point.x >= GetWorldCoorRect().Left() && point.x <= GetWorldCoorRect().Right())
    {
        if (point.y >= GetWorldCoorRect().Bottom() && point.y <= GetWorldCoorRect().Top())
        {
            return true;
        }
    }
    return false;
}

void CS230::CircleCollision::Draw(math::TransformMatrix cameraMatrix)
{
    doodle::no_fill();
    doodle::set_outline_width(2);
    doodle::set_outline_color(doodle::HexColor(0xFFFFFFFF));
    doodle::push_settings();
    doodle::apply_matrix(cameraMatrix[0][0], cameraMatrix[1][0], cameraMatrix[0][1], cameraMatrix[1][1], cameraMatrix[0][2], cameraMatrix[1][2]);
    doodle::draw_ellipse(objectPtr->GetPosition().x, objectPtr->GetPosition().y, GetRadius() * 2);
    doodle::pop_settings();
}

double CS230::CircleCollision::GetRadius()
{
    return (math::ScaleMatrix(objectPtr->GetScale()) * math::vec2{ radius, 0 }).x;
}

bool CS230::CircleCollision::DoesCollideWith(GameObject* objectB)
{
    if (objectB->GetGOComponent<Collision>() != nullptr && objectB->GetGOComponent<Collision>()->GetCollideType() == CollideType::Circle_Collide)
    {
        double d_x = (objectPtr->GetPosition().x - objectB->GetPosition().x);
        double d_y = (objectPtr->GetPosition().y - objectB->GetPosition().y);
        double distance = (d_x * d_x) + (d_y * d_y);
        double d_r = GetRadius() + objectB->GetGOComponent<CS230::CircleCollision>()->GetRadius();
        if (distance < d_r * d_r)
        {
            return true;
        }
    }
    return false;
}

bool CS230::CircleCollision::DoesCollideWith(math::vec2 point)
{
    double d_x = (objectPtr->GetPosition().x - point.x);
    double d_y = (objectPtr->GetPosition().y - point.y);
    double distance = (d_x * d_x) + (d_y * d_y);
    if (distance <= GetRadius() * GetRadius())
    {
        return true;
    }
    return false;
}