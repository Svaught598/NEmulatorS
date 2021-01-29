#include <iostream>

#include <boost/format.hpp>

#include "../include/bus.h"
#include "../include/6502.h"
#include "../include/cart.h"


Bus::Bus(){
    
}


Bus::~Bus(){
    
}


void Bus::connectCart(std::shared_ptr<Cart> newCart){
    cart = newCart;
}


void Bus::connectCPU(std::shared_ptr<CPU> newCpu){
    cpu = newCpu;
}


void Bus::connectLogger(std::shared_ptr<Logger> newLogger){
    logger = newLogger;
}


void Bus::write(u16 address, u8 data){
    if (address < 0x2000){
        mram[address] = data;
    }
}


u8 Bus::read(u16 address){
    if (address < 0x2000){
        return mram[address];
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
