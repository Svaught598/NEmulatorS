#include "../include/ppu.h"
#include "../include/bus.h"


// constructor
PPU::PPU(Bus& newBus, Logger& newLogger)
    : bus(&newBus), logger(newLogger)
{
    bus->connectPPU(*this);
}


void PPU::connectCart(Cart& newCart){
    cart = &newCart;
}


// writing to registers
void PPU::writeToRegisters(u8 reg, u8 value){
    switch (reg){
        case 0:
            PPUCTRL = value;
            break;
        case 1: ;
            PPUMASK = value;
            break;
        case 2:
            // PPUSTATUS is read only 
            break;
        case 3:
            OAMADDR = value;
            break;
        case 4: 
            OAMDATA = value;
            OAMADDR += 1;
            break;
        case 5:
            // TODO: implement writing to PPUSCROLL
            if (addressLatch) {
                break;
            } else {
                break;
            }
        case 6:
            if (addressLatch == 0) {
                TRAMADDR = ((value & 0x3F) << 8) | 0x00FF;
                addressLatch = 1;
                break;
            } else {
                TRAMADDR = (TRAMADDR & 0xFF00) | value;
                VRAMADDR = TRAMADDR;
                addressLatch = 0;
                break;
            }
        case 7:
            writeVRAM(VRAMADDR, value);
            u8 offset = (PPUCTRL & 0b100) ? 1 : 32;
            VRAMADDR += offset;
            break;
    }
}


// Reading from Registers
u8 PPU::readFromRegisters(u8 reg){
    switch (reg){
        case 0:
            // PPUCTRL is write only
            break;
        case 1:
            // PPUMASK is write only
            break;
        case 2:
            u8 reg = PPUSTATUS;
            PPUSTATUS &= 0x7F;
            return reg;
        case 3:
            // OAMADDR is write only
            break;
        case 4:
            u8 reg = OAMDATA;
            return reg;
        case 5:
            // PPUSCROLL is write only
            break;
        case 6:
            // PPUADDR is write only
            break;
        case 7:
            u8 value = readVRAM(VRAMADDR);
            u8 offset = (PPUCTRL & 0b100) ? 1 : 32;
            VRAMADDR += offset;
            return value;
            break;
    }
}


u8 PPU::read(u16 address){

}


void PPU::write(u16 address, u8 value){

}