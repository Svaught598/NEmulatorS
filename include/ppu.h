#ifndef NES_PPU
#define NES_PPU

#include "typedefs.h"
#include "log.h"
#include "cart.h"

#include <GL/glew.h>


// circular include if I #include "bus.h"
// so I'm just going with a declaration
class Bus;


class PPU
{
public:

    PPU(Bus& newBus, Logger& newLogger);
    ~PPU();

    void connectCart(Cart& newCart);
    void tick();
    void writeToRegisters(u8 reg, u8 value);
    u8 readFromRegisters(u8 reg);
    void write(u16 address, u8 value);
    u8 read(u16 address);

    // temporary functions for hacking away :D
    GLuint renderNT1();
    GLuint renderNT2();
    void loadStuffFromCart();

private:

    Bus* bus = nullptr;
    Cart* cart = nullptr;
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
    u32 palette[64] = {
		0x7C7C7C, 0x0000FC, 0x0000BC, 0x4428BC, 0x940084, 0xA80020, 0xA81000, 0x881400,
        0x503000, 0x007800, 0x006800, 0x005800, 0x004058, 0x000000, 0x000000, 0x000000,
		0xBCBCBC, 0x0078F8, 0x0058F8, 0x6844FC, 0xD800CC, 0xE40058, 0xF83800, 0xE45C10,
        0xAC7C00, 0x00B800, 0x00A800, 0x00A844, 0x008888, 0x000000, 0x000000, 0x000000,
		0xF8F8F8, 0x3CBCFC, 0x6888FC, 0x9878F8, 0xF878F8, 0xF85898, 0xF87858, 0xFCA044,
        0xF8B800, 0xB8F818, 0x58D854, 0x58F898, 0x00E8D8, 0x787878, 0x000000, 0x000000,
		0xFCFCFC, 0xA4E4FC, 0xB8B8F8, 0xD8B8F8, 0xF8B8F8, 0xF8A4C0, 0xF0D0B0, 0xFCE0A8,
        0xF8D878, 0xD8F878, 0xB8F8B8, 0xB8F8D8, 0x00FCFC, 0xF8D8F8, 0x000000, 0x000000
    };


    // other helping variables
    bool addressLatch;

    void writeVRAM(u8 address, u8 value);
    u8 readVRAM(u8 address);
    u32 readFromPalette(u8 pal, u8 index);

    // GL Specific stuff
    GLuint NT1_id;
    GLuint NT2_id;
    void initTexture(GLuint &texture);
    void updTextureNT1();
};

#endif                