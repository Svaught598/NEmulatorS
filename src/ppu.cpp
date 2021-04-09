#include "../include/ppu.h"
#include "../include/bus.h"


// constructor
PPU::PPU(Bus& newBus, Logger& newLogger)
    : bus(&newBus), logger(newLogger)
{
    bus->connectPPU(*this);
    initTexture(NT1_id);
    initTexture(NT2_id);
}


PPU::~PPU(){};


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
            write(VRAMADDR, value);
            u8 offset = (PPUCTRL & 0b100) ? 1 : 32;
            VRAMADDR += offset;
            break;
    }
}


// Reading from Registers
u8 PPU::readFromRegisters(u8 reg){

    u8 data;
    switch (reg){
        case 0:
            // PPUCTRL is write only
            break;
        case 1:
            // PPUMASK is write only
            break;
        case 2:
            data = PPUSTATUS;
            PPUSTATUS &= 0x7F;
            break;
        case 3:
            // OAMADDR is write only
            break;
        case 4:
            data = OAMDATA;
            break;
        case 5:
            // PPUSCROLL is write only
            break;
        case 6:
            // PPUADDR is write only
            break;
        case 7:
            data = read(VRAMADDR);
            VRAMADDR += (PPUCTRL & 0b100) ? 1 : 32;
            break;
    }
    return data;
}


u8 PPU::read(u16 address){
    // pattern tables
    if (address < 0x1000){
        return patterntable1[address];
    }
    else if (address < 0x2000){
        return patterntable2[address - 0x1000];
    }

    // nametables
    else if (address >= 0x2000 && address < 0x3EFF){
        address &= 0xFFF;
        if (address < 0x400){
            return nametable1[address & 0x3FF];
        }
        else if (address < 0x800){
            if (cart->mirroring == HORIZONTAL_MIRROR){
                return nametable1[address & 0x3FF];
            }
            else if (cart->mirroring == VERTICAL_MIRROR){
                return nametable2[address & 0x3FF];
            }
        }
        else if (address < 0xC00){
            if (cart->mirroring == HORIZONTAL_MIRROR){
                return nametable2[address & 0x3FF];
            }
            else if (cart->mirroring == VERTICAL_MIRROR){
                return nametable1[address & 0x3FF];
            }
        }
        else if (address < 0xFFF){
            return nametable2[address & 0x3FF];
        }
    }
    
    // Palette indices
    else if(address < 0x3FFF){
        address &= 0x1F;

        // Nesdev: Addresses $3F10/$3F14/$3F18/$3F1C are mirrors of $3F00/$3F04/$3F08/$3F0C
        address = (address == 0x10) ? 0x00 : address;
        address = (address == 0x14) ? 0x04 : address;
        address = (address == 0x18) ? 0x08 : address;
        address = (address == 0x1C) ? 0x0C : address;
        return palettetable[address];
    }
}


void PPU::write(u16 address, u8 value){
    //BRRRRRRRRRRR
}


void PPU::loadStuffFromCart(){
    for (int i = 0; i < 0x1000; i++){
        nametable1[i] = cart->readPPU(i);
        nametable2[i + 0x1000] = cart->readPPU(i + 0x1000);
    }
}


u32 PPU::readFromPalette(u8 pal, u8 index){
    u8 index = read(0x3000 + (pal << 2) + index);
    return palette[index & 0x3F];
}


GLuint PPU::renderNT1(){
    int temp;
    glBindTexture(GL_TEXTURE_2D, NT1_id);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 100, 100, GL_RGBA, GL_UNSIGNED_BYTE, static_cast<const GLvoid*>(nametable1));
    return NT1_id;
}


GLuint PPU::renderNT2(){
    int temp;
    glBindTexture(GL_TEXTURE_2D, NT2_id);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 100, 100, GL_RGBA, GL_UNSIGNED_BYTE, static_cast<const GLvoid*>(nametable2));
    return NT1_id;
}


void PPU::initTexture(GLuint &texture){
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
}