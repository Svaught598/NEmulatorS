#ifndef NES_SYSTEM
#define NES_SYSTEM

#include <string>
#include <iostream>

#include <GL/glew.h>   
#include <GLFW/glfw3.h>
#include "../Imgui/imgui.h"
#include "../Imgui/imgui_impl_glfw.h"
#include "../Imgui/imgui_impl_opengl3.h"

#include "../include/6502.h"
#include "../include/gui.h"
#include "../include/cart.h"


class System
{
    /**
     * Singleton System Class
    */
public:

    System(std::string name){
        systemName = name;
        Bus* bus = new Bus();
        CPU* cpu = new CPU(bus);
    };

    void loadCart(char* filepath);
    void loadCart();

    void setImguiDemo(bool is_demo);
    void setTesting(bool is_testing);

    int mainLoop();
    
private:

    // NES components
    Bus* bus;
    CPU* cpu;
    Cart* cart;

    // State Variables
    std::string systemName;
    bool demo_mode = false;
    bool test_mode = false;
    bool cart_loaded = false;
    bool running = false;

    void setRunning(bool is_running);
    char* openFileSystem();

};

#endif