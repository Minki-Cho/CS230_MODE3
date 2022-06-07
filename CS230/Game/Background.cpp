/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Background.cpp
Project: CS230
Author: MINJAE YU
Creation date: 3/30/2022
-----------------------------------------------------------------*/
#include "Background.h" //Background
#include "../Engine/Camera.h" //camera
#include "../Engine/TransformMatrix.h" //matrix
#include "../Engine/Engine.h" //GetTextureManager

void Background::Add(const std::filesystem::path& texturePath, int level)
{
	backgrounds.push_back(ParallaxInfo{ Engine::GetTextureManager().Load(texturePath),level });
}

void Background::Unload()
{
	backgrounds.clear();
}

void Background::Draw(const CS230::Camera& camera)
{
	for (ParallaxInfo& info : backgrounds)
	{
		info.texturePtr->Draw(math::TranslateMatrix{ -math::vec2{camera.GetPosition().x / info.level,camera.GetPosition().y} });
	}
}

math::ivec2 Background::Size()
{
	for (int i = 0; i < backgrounds.size(); i++)
	{
		if (backgrounds[i].level == 1)
			return backgrounds[i].texturePtr->GetSize();
	}
	return math::ivec2{ 0,0 };
}