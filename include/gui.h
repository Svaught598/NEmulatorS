#ifndef NES_GUI
#define NES_GUI

class CPU;
class PPU;
class System;

class GUI
{
public:

    GUI(){};

    GLFWwindow* window = nullptr;

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
    void CPUDebugWindow(CPU &cpu);
    void PpuDebugWindow(PPU &ppu);

protected:

    bool show_demo_window = true;
    bool show_another_window = true;
    bool show_debug_window = true;
    int switchDebugTabs= 0;

    float f = 0.0f;
    int counter = 0;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    char* glsl_version = "#version 130";

};

#endif