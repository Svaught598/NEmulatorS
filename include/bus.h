#ifndef NES_BUS
#define NES_BUS

#include <iostream>
#include <memory>

#include "typedefs.h"
#include "6502.h"
#include "cart.h"
#include "log.h"


class Bus
{
public:

    Bus(Logger& newLogger);
    ~Bus();

    void connectCart(Cart& newCart);
    void connectCPU(CPU& newCpu);
    void connectPPU(PPU& newPpu);
    void write(u16 address, u8 data);
    u8 read(u16 address);

private:

    Cart* cart = nullptr;
    CPU* cpu = nullptr;
    PPU* ppu = nullptr;
    Logger& logger;
    u8 mram[0x800];

};

#endif