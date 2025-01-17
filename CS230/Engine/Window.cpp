/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Window.cpp
Project: CS230
Author: Kevin Wright
Creation date: 2/10/2021
-----------------------------------------------------------------*/
#include <doodle/window.hpp> //doodle Window
#include <doodle/drawing.hpp> // set_frame_of_reference, clear_background
#include "Window.h" //Window
#include "Engine.h"	// GetWindow

void CS230::Window::Init(std::string windowName)
{
    doodle::create_window(windowName, 1440, 810);
    doodle::set_frame_of_reference(doodle::FrameOfReference::RightHanded_OriginBottomLeft);
}
void CS230::Window::Update()
{
    doodle::update_window();
}

void CS230::Window::Resize(int newWidth, int newHeight)
{
    windowSize.x = newWidth;
    windowSize.y = newHeight;
    Engine::GetLogger().LogEvent("Window Resized");
}

math::ivec2 CS230::Window::GetSize()
{
    return windowSize;
}

void CS230::Window::Clear(unsigned int color)
{
    doodle::clear_background(doodle::HexColor{ color });
}

void on_window_resized(int new_width, int new_height) {
    Engine::GetWindow().Resize(new_width, new_height);
}