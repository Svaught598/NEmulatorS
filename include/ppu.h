#ifndef NES_PPU
#define NES_PPU

#include "typedefs.h"
#include "log.h"


// circular include if I #include "bus.h"
// so I'm just going with a declaration
class Bus;


class PPU
{
public:

    PPU(Bus& newBus, Logger& newLogger);
    ~PPU();

    void tick();
    void writeToRegisters(u8 reg, u8 value);
    u8 readFromRegisters(u8 reg);
    void write(u8 address, u8 value);
    u8 read(u8 address);

private:

    Bus* bus = nullptr;
    Logger& logger;

    // registers
    u8 PPUCTRL;
    u8 PPUMASK;
    u8 PPUSTATUS;
    u8 OAMADDR;
    u8 OAMDATA;
    u8 PPUSCROLL;
    u16 VRAMADDR;
    u16 TRAMADDR;
    u8 PPUDATA;
    u8 OAMDMA;

    // memory tables & such
    u8 oam[0xFF];
    u8 nametable1[0x400];
    u8 nametable2[0x400];
    u8 patterntable1[0x1000];
    u8 patterntable2[0x1000];
    u8 palettetable[0x20];

    // other helping variables
    bool addressLatch;

    void writeVRAM(u8 address, u8 value);
    u8 readVRAM(u8 address);
};

#endif                