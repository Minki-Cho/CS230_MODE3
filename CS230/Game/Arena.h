/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Arena.h
Project: CS230
Author: MINJAE YU, MINKI CHO
Creation date: 6/7/2022
-----------------------------------------------------------------*/
#pragma once
#include "../Engine/Texture.h" //use texture
#include "../Engine/Component.h" //Component inheritance

class Arena : public CS230::Component
{
public:
    Arena(const std::filesystem::path& texturePath);
    void Unload();
    void Draw();
    math::ivec2 Size();
private:
    CS230::Texture* texturePtr;
};