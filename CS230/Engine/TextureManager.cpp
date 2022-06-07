/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Project: CS230
Author: MINJAE YU
Creation date: 4/28/2022
-----------------------------------------------------------------*/
#include "TextureManager.h" //TextureManager
#include "Engine.h" //GetLogger
#include "Texture.h" //new Texture

CS230::Texture* CS230::TextureManager::Load(const std::filesystem::path& filePath)
{
	if (pathToTexture.count(filePath) == 0)
	{
		pathToTexture.insert(std::make_pair(filePath, new Texture{ filePath }));
	}
	return pathToTexture[filePath];
}

void CS230::TextureManager::Unload()
{
	Engine::GetLogger().LogEvent("Clear Textures");
	std::map<std::filesystem::path, Texture*>::iterator iter = pathToTexture.begin();
	for (; iter != pathToTexture.end(); iter++)
	{
		delete iter->second;
		iter->second = nullptr;
	}
	pathToTexture.clear();
}