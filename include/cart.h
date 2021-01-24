#ifndef NES_CART
#define NES_CART

#include <iostream>
#include <fstream>
#include <vector>
#include <memory>

#include "typedefs.h"
#include "mappers.h"

#define NES_HEADER_SIZE 16
#define VERTICAL_MIRROR true
#define HORIZONTAL_MIRROR false

struct HeaderData
{
    /**
     * iNES Header Data Struct
    */
public:

    u8 name[4];   // 0-3: Constant $4E $45 $53 $1A ("NES" followed by MS-DOS end-of-file)
    u8 PrgRomSize;  // 4:   16kB chunks
    u8 ChrRomSize;  // 5:   8kB chunks
    u8 Mapper1;     // 6:   Mapper, Mirroring, Battery, Trainer
    u8 Mapper2;     // 7:   Mapper, VS/Playchoice, NES 2.0
    u8 PrgRamSize;  // 8:   PRG-RAM size (rarely used extension)
    u8 TVSystem1;   // 9:   TV system (rarely used extension)
    u8 TVSystem2;   // 10:  TV system, PRG-RAM presence (unofficial, rarely used extension)
    u8 unused[5];
    // 11-15: Unused padding (should be filled with zero,
    // but some rippers put their name across bytes 7-15)
};

class Cart
{
    /**
     * Cartridge class
    */
public:

    Cart();
    Cart(char *filename);
    ~Cart();

    u8 read(u16 address);

private:

    HeaderData header;
    BasicMapper mapper;
    u32 prgRomSize;
    u32 chrRomSize;
    std::vector<u8> prgRom;
    std::vector<u8> chrRom;
    bool mirroring;
    
    
    void getHeaderData(std::ifstream &ifs);
    void getRomData(std::ifstream &ifs);
    void getMapper();

};

#endif