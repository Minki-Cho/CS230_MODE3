/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Arena.cpp
Project: CS230
Author: MINJAE YU, MINKI CHO
Creation date: 6/7/2022
-----------------------------------------------------------------*/
#include "Arena.h" //Arena
#include "../Engine/Camera.h" //camera
#include "../Engine/TransformMatrix.h" //matrix
#include "../Engine/Engine.h" //GetTextureManager

Arena::Arena(const std::filesystem::path& texturePath)
{
	texturePtr = Engine::GetTextureManager().Load(texturePath);
}

void Arena::Unload()
{
	delete texturePtr;
}

void Arena::Draw()
{
	//texturePtr->Draw(math::TranslateMatrix(Engine::GetWindow().GetSize() / 2));
	texturePtr->Draw(math::TranslateMatrix(math::vec2{}));
}

math::ivec2 Arena::Size()
{
	return texturePtr->GetSize();
}