/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: ScreenShaker.h
Project: CS230
Author: Minki Cho, Minjae Yu
Creation date: 6/12/2022
-----------------------------------------------------------------*/
#pragma once
#include "..\Engine\Component.h" //Component inheritance
#include "..\Engine\GameObject.h"  //object

class CS230::GameObject;

class ScreenShake : public CS230::Component
{
public:
    ScreenShake(/*CS230::GameObject& object*/);
    void Update(double dt) override;
private:
    //CS230::GameObject& object;
    double        shake;
    static constexpr double  MaxAngle = 0.01;
    static constexpr double  MaxOffset = 10.0;
    double random_helper(double min, double max);
};