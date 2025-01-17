/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Window.h
Project: CS230
Author: Kevin Wright
Creation date: 2/10/2021
-----------------------------------------------------------------*/
#pragma once
#include "Vec2.h" //Vec2 variables
#include <string> //std::string

namespace CS230
{
    class Window
    {
    public:
        void Init(std::string windowName);
        void Update();

        //vec2
        void Resize(int newWidth, int newHeight);
        math::ivec2 GetSize();
        void Clear(unsigned int color);
    private:
        math::ivec2 windowSize;
    };
}