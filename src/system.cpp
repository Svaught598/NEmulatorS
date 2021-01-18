#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <thread>

#include "../include/system.h"
#include "../include/gui.h"


///////////////////////////////////////////////
// Public Methods                            //
///////////////////////////////////////////////


// load cart from filepath (for testing)
void System::loadCart(char* filepath){
    cart = new Cart(filepath);
    cart_loaded = true;
}


// load cart from filesystem selection window
void System::loadCart(){
    char* filepath = openFileSystem();
    cart = new Cart(filepath);
    cart_loaded = true;
}


// Sets the flag for rendering the ImGui demo widgets
void System::setImguiDemo(bool is_demo){
    demo_mode = true;
}


// Loads Blarggs NesTest.nes & sets emulator to run
void System::setTesting(bool is_testing){
    if (is_testing){
        loadCart("./test/nestest.nes");
        setRunning(true);
    }
}


// Where the action happens!
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


///////////////////////////////////////////////
// Private methods                           //
///////////////////////////////////////////////


// sets the emulator `running` variable
void System::setRunning(bool is_running){
    running = is_running;
}


// opens a filesystem and returns chosen filepath
char* System::openFileSystem(){
    auto f = []()
    {
        char filename[1024];
        FILE *f = popen("zenity --file-selection", "r");
        fgets(filename, 1024, f);
        return filename;
    };

    // separate thread - Don't block emulation
    std::thread file_system(f);
    file_system.detach();
}