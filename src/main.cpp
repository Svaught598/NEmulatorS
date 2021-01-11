#include <iostream>

#include "../include/system.h"



int main(){

    // Mainloop
    System nes = System("NES");
    return nes.mainLoop();
}