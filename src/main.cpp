#include <iostream>
#include <string>

#include "../include/system.h"


int printUsage(){
    std::cout << \
    "So this is an emulator with a GUI, so most controls are set through menu\n"
    "Options in said GUI. since command line arguments are especially conducive\n"
    "to flagging the program to run in different 'modes',\n"
    "that is what it is used for here\n"
    "\n"
    "These are the flags:\n"
    "   --demo, -d              enables ImGui demo window\n"
    "   --help, -h              shows this message!\n";
    return 1;
}


int main(int argc, char *argv[]){

    System nes = System("NES");

    if(argc == 2){ 
        std::string argument = argv[1];
        if (argument == "--demo" || argument == "-d"){
            std::cout << "ImGui demo mode set";
            nes.setImguiDemo(true);
        }
        else if (argument == "--test" || argument == "-t"){
            std::cout << "Running Blargs tests";
            nes.setTesting(true);
        }
        else if (argument == "--help" || argument == "-h"){
            return printUsage();
        }
        else{
            return printUsage();
        }
    }
    else if(argc > 2) {
        return printUsage();
    }

    // enter the main emulation loop
    return nes.mainLoop();
}