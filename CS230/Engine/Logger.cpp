/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Logger.cpp
Project: CS230
Author: MINJAE YU
Creation date: 3/8/2022
-----------------------------------------------------------------*/
#include <iostream> // cout.rdbuf
#include "Logger.h" // logger

CS230::Logger::Logger(Logger::Severity severity, bool useConsole, std::chrono::system_clock::time_point start_time)
	: minLevel(severity), outStream("Trace.log"), startTime(start_time)
{
	if (useConsole == true)
	{
		outStream.set_rdbuf(std::cout.rdbuf());
	}
}

CS230::Logger::~Logger()
{
	outStream.flush();
	outStream.close();
}

void CS230::Logger::Log(CS230::Logger::Severity severity, std::string message)
{
	if (severity >= minLevel)
	{
		switch (severity)
		{
		case Severity::Verbose:
			outStream.precision(4);
			outStream << '[' << std::fixed << GetSecondsSinceStart() << "]\t";
			outStream << "Verb" << '\t' << message << '\n';
			break;
		case Severity::Debug:
			outStream.precision(4);
			outStream << '[' << std::fixed << GetSecondsSinceStart() << "]\t";
			outStream << "Debug" << '\t' << message << '\n';
			break;
		case Severity::Event:
			outStream.precision(4);
			outStream << '[' << std::fixed << GetSecondsSinceStart() << "]\t";
			outStream << "Event" << '\t' << message << '\n';
			break;
		case Severity::Error:
			outStream.precision(4);
			outStream << '[' << std::fixed << GetSecondsSinceStart() << "]\t";
			outStream << "Error" << '\t' << message << '\n';
			break;
		}
	}
}

double CS230::Logger::GetSecondsSinceStart()
{
	return std::chrono::duration<double>(std::chrono::system_clock::now() - startTime).count();
}