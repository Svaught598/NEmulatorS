#include <stdlib.h>
#include <iostream>
#include <thread>

#include "../include/system.h"
#include "../include/gui.h"



void System::setImguiDemo(){
    demo_mode = true;
}

void System::openFileSystem(){
    // function for opening filesystem
    auto f = []()
    {
        char filename[1024];
        FILE *f = popen("zenity --file-selection", "r");
        fgets(filename, 1024, f);
        std::cout << filename;
    };

    // opens filesystem in separate thread to not interfere
    // with emulation
    std::thread file_system(f);
    file_system.detach();
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

        // Demo Window (set by argument flag `--demo, -d`)
        if (demo_mode)
        {
            GUI::ShowDemo();    
        }

        // Main GUI
        {
            GUI::MainMenuBar(this); 
        }
        GUI::Render();
        GUI::SwapBuffers();
    }

    // Cleanup
    return GUI::Cleanup();
}
