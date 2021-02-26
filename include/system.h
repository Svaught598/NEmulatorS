#ifndef NES_SYSTEM
#define NES_SYSTEM

#include <string>
#include <iostream>
#include <memory>

#include <GL/glew.h>   
#include <GLFW/glfw3.h>
#include "../Imgui/imgui.h"
#include "../Imgui/imgui_impl_glfw.h"
#include "../Imgui/imgui_impl_opengl3.h"

#include "../include/6502.h"
#include "../include/gui.h"
#include "../include/bus.h"
#include "../include/cart.h"
#include "../include/log.h"


class System
{
public:

    System(std::string name, Logger& newLogger);
    ~System(){};

    // tied to GUI controls
    void loadCart(char* filepath);
    void loadCart();

    // tied to command line arguments
    void setImguiDemo(bool isDemo);
    void setTesting(bool isTesting);

    // tied to `int main()`
    int mainLoop();
    
private:

    // NES components
    std::unique_ptr<Bus> bus;
    std::unique_ptr<CPU> cpu;
    std::unique_ptr<PPU> ppu;
    std::unique_ptr<Cart> cart;
    Logger& logger;

    // State Variables
    std::string systemName;
    bool demoMode = false;
    bool testMode = false;
    bool cartLoaded = false;
    bool running = false;

    void tick();
    void setRunning(bool isRunning);
    char* openFileSystem();

};

#endif