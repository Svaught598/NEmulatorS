#include <iostream>
#include <fstream>
#include <thread>

#include "../include/system.h"
#include "../include/gui.h"
#include "../include/ppu.h"


System::System(std::string name, Logger& newLogger)
    : systemName(name), logger(newLogger)
{
    bus = std::make_unique<Bus>(logger);
    cpu = std::make_unique<CPU>(*bus, logger);
    ppu = std::make_unique<PPU>(*bus, logger);
    logger << Logger::logType::LOG_INFO
        << "System initialized!"
        << Logger::logType::LOG_ENDLINE;
}


///////////////////////////////////////////////
// Public Methods                            //
///////////////////////////////////////////////


// load cart from filepath (for testing)
void System::loadCart(char* filepath){
    cart = std::make_unique<Cart>(filepath, logger);
    bus->connectCart(*cart);
    ppu->connectCart(*cart);
    cartLoaded = true;
}


// load cart from filesystem selection window
void System::loadCart(){
    char* filepath = openFileSystem();
    cart = std::make_unique<Cart>(filepath, logger);

    bus->connectCart(*cart);
    ppu->connectCart(*cart);
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
        cpu->PC = 0xC000;
        setRunning(true);
    }
}


// Where the action happens!
int System::mainLoop(){

    // Setup Stuff
    GUI *gui = new GUI();
    if (gui->SetupWindow())
        return 1;
    if (gui->SetupGLEW())
        return 1;
    if (gui->SetupImGui())
        return 1;

    // GLFW main loop
    while (!glfwWindowShouldClose(gui->window))
    {   
        // GUI Events
        gui->PollEvents();
        gui->NewFrame();

        // System Events
        if (cartLoaded) 
            cpu->tick();

        // Demo Window (set by argument flag `--demo, -d`)
        if (demoMode)
        {
            gui->ShowDemo();
        }

        // Main GUI
        {   
            gui->MainMenuBar(this);

            if (cartLoaded)
                gui->CPUDebugWindow(*cpu);
                gui->PpuDebugWindow(*ppu);
        }
        gui->Render();
        gui->SwapBuffers();
    }

    // Cleanup
    return gui->Cleanup();
}


///////////////////////////////////////////////
// Private methods                           //
///////////////////////////////////////////////


void System::tick(){
    cpu->tick();
}


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