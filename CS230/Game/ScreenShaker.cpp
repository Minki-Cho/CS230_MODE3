/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: ScreenShaker.cpp
Project: CS230
Author: Minki Cho, Minjae Yu
Creation date: 6/12/2022
-----------------------------------------------------------------*/
#include "ScreenShaker.h"
#include <doodle/drawing.hpp> // apply_translate, apply_rotate
using namespace doodle;
ScreenShake::ScreenShake() : shake(1.0)
{
}

void ScreenShake::Update(double dt)
{
    const double offsetX = MaxOffset * shake * random_helper(-1.0, 1.0);
    const double offsetY = MaxOffset * shake * random_helper(-1.0, 1.0);
    const double angle = MaxAngle * shake * random_helper(-1.0, 1.0);
    apply_translate(offsetX, offsetY);
    apply_rotate(angle);
    shake -= dt;
    shake = std::clamp(shake, 0.0, 1.0);
}

double ScreenShake::random_helper(double min, double max)
{
    return min + rand() / (RAND_MAX / (max - min));
}
