#include <iostream>

#include <boost/format.hpp>

#include "../include/bus.h"
#include "../include/6502.h"
#include "../include/cart.h"


Bus::Bus(Logger& newLogger)
    : logger(newLogger){}


Bus::~Bus(){}


void Bus::connectCart(Cart& newCart){
    cart = &newCart;
}


void Bus::connectCPU(CPU& newCpu){
    cpu = &newCpu;
}


void Bus::write(u16 address, u8 data){
    if (address < 0x2000){
        mram[address & 0x07FF] = data;
        if (address == 0x0300){
            std::cout << "write 0x0300: " << std::hex << (int) data << std::endl;
        }
    }
}


u8 Bus::read(u16 address){
    if (address < 0x2000){
        if (address == 0x0300){
            std::cout << "read 0x0300: " << std::hex << (int) mram[0x0300] << std::endl;
        }


        return mram[address & 0x07FF];
    }
    else if (address < 0x4020){
        // TODO: PPU memory, mirrored every 8
        // function call might look like:
        // ppu->read(address & 0x7)
    }
    else if (address < 0x6000){
        // Expansion ROM? idk
    }
    else {
        return cart->read(address);
    }
}
