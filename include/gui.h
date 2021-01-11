#pragma once


namespace GUI {

// GUI State Variables
static bool show_demo_window;
static bool show_another_window;
static float f;
static int counter;
ImVec4 clear_color;


// GUI functions
void SetupImGui(GLFWwindow *window, const char *glsl_version);

void NewFrame();
void ShowDemo();
void ShowExampleAppMainMenuBar();
void Render(GLFWwindow *window);
}
