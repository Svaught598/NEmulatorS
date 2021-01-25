#include <iostream>
#include <string>

#include "../include/system.h"
#include "../include/log.h"


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

    std::shared_ptr<Logger> logger = std::make_shared<Logger>();
    System nes = System("NES", logger);

    if(argc == 2){ 
        std::string argument = argv[1];
        if (argument == "--demo" || argument == "-d"){
            *logger << Logger::logType::LOG_INFO << "ImGui demo mode set";
            nes.setImguiDemo(true);
        }
        else if (argument == "--test" || argument == "-t"){
            *logger << Logger::logType::LOG_INFO << "Running Blargs tests";
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