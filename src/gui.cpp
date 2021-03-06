#include <iostream>

#include <GL/glew.h>   
#include <GLFW/glfw3.h>
#include "../Imgui/imgui.h"
#include "../Imgui/imgui_impl_glfw.h"
#include "../Imgui/imgui_impl_opengl3.h"

#include "../include/gui.h"
#include "../include/system.h"


///////////////////////////////////////////////
// GUI  Functions                            //
///////////////////////////////////////////////

// error handling
static void glfw_error_callback(int error, const char* description){
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}


///////////////////////////////////////////////
// Setup Functions                           //
///////////////////////////////////////////////

int GUI::SetupWindow(){
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) return 1;

    // GL 3.0 + GLSL 130
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only

    // Create window with graphics context
    GUI::window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
    if (GUI::window == NULL)
        return 1;
    glfwMakeContextCurrent(GUI::window);
    glfwSwapInterval(1); // Enable vsync
    return 0;
}


int GUI::SetupGLEW(){
    // Initialize OpenGL loader
    bool err = glewInit() != GLEW_OK;
    if (err){
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return 1;
    }
    return 0;
}


int GUI::SetupImGui(){
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    //ImGui::StyleColorsDark();
    ImGui::StyleColorsClassic();
    ImGui_ImplGlfw_InitForOpenGL(GUI::window, true);
    ImGui_ImplOpenGL3_Init(GUI::glsl_version);
    return 0;
}


///////////////////////////////////////////////
// General GUI Functions                     //
///////////////////////////////////////////////


void GUI::NewFrame(){
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}


void GUI::Render(){
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void GUI::PollEvents(){
    glfwPollEvents();
}


void GUI::SwapBuffers(){
    glfwSwapBuffers(GUI::window);
}


int GUI::Cleanup(){
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(GUI::window);
    glfwTerminate();
    return 0;
}


///////////////////////////////////////////////
// Functions that build GUI                  //
///////////////////////////////////////////////

void GUI::ShowDemo(){
    {
        // Create a window called "Hello, world!" and append into it.
        ImGui::Begin("Hello, world!");

        // Display some text (you can use a format strings too)
        ImGui::Text("This is some useful text.");

        // Edit bools storing our window open/close state
        ImGui::Checkbox("Demo Window", &show_demo_window);                  
        ImGui::Checkbox("Another Window", &show_another_window);

        // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);

        // Edit 3 floats representing a color
        ImGui::ColorEdit3("clear color", (float*)&clear_color); 

        // Buttons return true when clicked
        // (most widgets return true when edited/activated)
        if (ImGui::Button("Button"))                            
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }

    // // 2. Show a simple window
    if (show_demo_window){
        ImGui::ShowDemoWindow(&show_demo_window);
    }

    // // 3. Show another simple window.
    // if (show_another_window)
    // {
    //     // Pass a pointer to our bool variable
    //     // (the window will have a closing button that will clear the bool when clicked)
    //     ImGui::Begin("Another Window", &show_another_window);   
    //     ImGui::Text("Hello from another window!");
    //     if (ImGui::Button("Close Me"))
    //         show_another_window = false;
    //     ImGui::End();
    // }
}


void GUI::MainMenuBar(System* sys){
    if (ImGui::BeginMainMenuBar()){
        if (ImGui::BeginMenu("Library")){
            if (ImGui::MenuItem("Load Rom")){
                sys->loadCart();
            }
            if (ImGui::BeginMenu("Save")){
                if (ImGui::MenuItem("State 1")){
                    // TODO: save state 1 callback function
                }
                if (ImGui::MenuItem("State 2")){
                    // TODO: save state 2 callback function
                }
                if (ImGui::MenuItem("State 3")){
                    // TODO: save state 3 callback function
                }
                ImGui::EndMenu();
            }
            if (ImGui:: BeginMenu("Load")){
                if (ImGui::MenuItem("State 1")){
                    // TODO: load state 1 callback function
                }
                if (ImGui::MenuItem("State 2")){
                    // TODO: load state 2 callback function
                }
                if (ImGui::MenuItem("State 3")){
                    // TODO: load state 3 callback function
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Settings")){
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}


void GUI::CPUDebugWindow(CPU &cpu){
    ImGui::Begin("CPU Debug Window", &show_debug_window); 
    {
        ImGui::Text("Last Executed Opcode %x", cpu.OP);
        ImGui::Text("PC = %x", cpu.PC);  
        ImGui::Text("SP = %x", cpu.SP);
        ImGui::Text("A = %x", cpu.A);
        ImGui::Text("X = %x", cpu.X);
        ImGui::Text("Y = %x", cpu.Y);
        ImGui::Text("02h: %x", cpu.error1);
        ImGui::Text("03h: %x", cpu.error2);
        if (ImGui::Button("Step CPU")) cpu.tick();
    }
    ImGui::End();
}


void GUI::PpuDebugWindow(PPU &ppu){
    ImGui::Begin("PPU Debug Window", &show_debug_window);
    {
        ImGui::Image((ImTextureID)(ppu.renderNT1()), ImVec2(80 * 2.f, 72 * 2.f));
        ImGui::Image((ImTextureID)(ppu.renderNT2()), ImVec2(80 * 2.f, 72 * 2.f));
    }
    ImGui::End();
    
}