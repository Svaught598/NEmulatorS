#ifndef NES_GUI
#define NES_GUI



class System;

namespace GUI {

///////////////////////////////////////////////
// GUI Variables                             //
///////////////////////////////////////////////


// GUI State Variables
extern bool show_demo_window;
extern bool show_another_window;
extern float f;
extern int counter;
extern ImVec4 clear_color;

// GLFW & GL stuff
extern GLFWwindow* window;
extern char* glsl_version;

///////////////////////////////////////////////
// GUI  Functions                            //
///////////////////////////////////////////////


// Setup Functions (obviously hehe)
int SetupWindow();
int SetupGLEW();
int SetupImGui();

// General Purpose Functions
void NewFrame();
void Render();
void PollEvents();
void SwapBuffers();
int Cleanup();

// Functions that Build GUI
void ShowDemo();
void MainMenuBar(System* sys);
}

#endif