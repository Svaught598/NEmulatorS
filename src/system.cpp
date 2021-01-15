#include <stdlib.h>
#include "../include/system.h"
#include "../include/gui.h"



void System::setImguiDemo(){
    GUI::demo_mode= true;
}

///////////////////////////////////////////////
// Main Loop & Cleanup                       //
///////////////////////////////////////////////


int System::mainLoop(){

    // Setup Stuff
    if (GUI::SetupWindow())
        return 1;
    if (GUI::SetupGLEW())
        return 1;
    if (GUI::SetupImGui())
        return 1;

    // GLFW main loop
    while (!glfwWindowShouldClose(GUI::window))
    {
        GUI::PollEvents();
        GUI::NewFrame();
        GUI::ShowDemo();    // Demo Window (set `show_demo_window` in gui.cpp)
        GUI::MainMenuBar(); // Main GUI
        GUI::Render();
        GUI::SwapBuffers();
    }

    // Cleanup
    return GUI::Cleanup();
}
