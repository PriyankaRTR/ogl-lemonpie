// Dear ImGui: standalone example application for Win32 + OpenGL 3

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp
#include "vmath.h"
#pragma once

#include <windows.h>
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"


typedef struct
{
	float offsetX;
	float offsetY;
}mouseXYOffset;


class ImGui_Wrapper
{
private:

	HWND hwnd;
	ImGuiIO& io;
	// Our state ImGui
	bool show_demo_window;
	bool show_another_window;
	ImVec4 clear_color;
	vmath::mat4 matrixToDisplay;
	float inputCameraAngle;
	float mouseWheelOffset;
	ImVec2 mouseDragDelta;
	//mouseXYOffset mouseOffsets;

public:

	typedef struct
	{
		float offsetX;
		float offsetY;
	}mouseXYOffset;

	typedef struct
	{
		float posX;
		float posY;
	}mousePos;

	ImGui_Wrapper(HWND hwnd, ImGuiIO& newRef);
	~ImGui_Wrapper();

	bool setupImGui(void); // additional initial setup for my project
	void gameLoopUIUpdatesImGui(ImGuiContext* newCtx); // OTG UI I/O updates 
	bool cleanUpImGui(void);
	
	// write get/sets here
	void setMatrix(vmath::mat4& newMatrix);
	float getInputCameraAngle(void);
	float getDeltaTime(void);
	//void getDeltaMouse(mouseXYOffset &structMouseOffset);
	void getDeltaMouse(ImVec2& structMouseOffset);
	void getMousePos(mousePos& structMousePos); // not used since we are getting readyly calcualted delta
	float getDeltaMouseWheel(void);


};

