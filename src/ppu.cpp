#include "../include/ppu.h"
#include "../include/bus.h"


// constructor
PPU::PPU(Bus& newBus, Logger& newLogger)
    : bus(&newBus), logger(newLogger){}