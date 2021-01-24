#include <iostream>
#include <fstream>
#include <thread>

#include "../include/system.h"
#include "../include/gui.h"


System::System(std::string name)
{
    systemName = name;
    bus = std::make_shared<Bus>();
    cpu = std::make_shared<CPU>(bus);
}


///////////////////////////////////////////////
// Public Methods                            //
///////////////////////////////////////////////


// load cart from filepath (for testing)
void System::loadCart(char* filepath){
    cart = std::make_shared<Cart>(filepath);
    bus->insertCart(cart);
    cartLoaded = true;
}


// load cart from filesystem selection window
void System::loadCart(){
    char* filepath = openFileSystem();
    cart = std::make_shared<Cart>(filepath);
    bus->insertCart(cart);
    cartLoaded = true;
    delete filepath;
}


// Sets the flag for rendering the ImGui demo widgets
void System::setImguiDemo(bool isDemo){
    demoMode = isDemo;
}


// Loads Blarggs NesTest.nes & sets emulator to run
void System::setTesting(bool isTesting){
    if (isTesting){
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
        if (demoMode)
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
void System::setRunning(bool isRunning){
    running = isRunning;
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