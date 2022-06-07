/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Engine.h
Purpose: This is the service provider for our games engine services
Project: CS230
Author: MINJAE YU
Creation date: 3/13/2022
-----------------------------------------------------------------*/
#include "Engine.h" //Engine

Engine::Engine() : frameCount(0), lastTick(std::chrono::system_clock::now()),
#ifdef _DEBUG				
	logger(CS230::Logger::Severity::Debug, true, lastTick)
#else 						
	logger(CS230::Logger::Severity::Event, false, lastTick)
#endif
{}

Engine::~Engine() {}

void Engine::Init(std::string windowName)
{
	logger.LogEvent("Engine Init");
	window.Init(windowName);
	fpsCalcTime = lastTick;
	srand(static_cast<unsigned int>(time(NULL)));
	GetLogger().LogEvent("Seed = " + std::to_string(rand()));
}

void Engine::Shutdown()
{
	logger.LogEvent("Engine Shutdown");
}

void Engine::Update()
{
	std::chrono::system_clock::time_point now{ std::chrono::system_clock::now() };
	double dt = std::chrono::duration<double>(now - lastTick).count();

	//33.33milliseconds
	if (dt >= 1 / Engine::Target_FPS)
	{
		Engine::logger.LogVerbose("Engine Update");
		lastTick = now;

		if (++frameCount > Engine::FPS_IntervalFrameCount)
		{
			double averageFrameRate = frameCount / std::chrono::duration<double>(now - fpsCalcTime).count();
			Engine::logger.LogEvent("FPS: " + std::to_string(averageFrameRate));
			frameCount = 0;
			fpsCalcTime = now;
		}
		gameStateManager.Update(dt);
		input.Update();
		window.Update();
	}
}

bool Engine::HasGameEnded()
{
	return gameStateManager.HasGameEnded();
}

void Engine::AddSpriteFont(const std::filesystem::path& fileName)
{
	fonts.push_back(fileName);
}