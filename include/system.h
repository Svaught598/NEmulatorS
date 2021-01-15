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



class System
{
public:

    System(std::string name){
        systemName = name;
        Bus* bus = new Bus();
        CPU* cpu = new CPU(bus);
    };
    
    bool running = true;

    void openFileSystem();
    void setImguiDemo();
    int mainLoop();
    void step();

private:

    std::string systemName;
    bool demo_mode = false;
    Bus* bus;
    CPU* cpu;

};

#endif