#ifndef NES_CART
#define NES_CART

#include <iostream>
#include <fstream>
#include <vector>
#include <memory>

#include "typedefs.h"
#include "mappers.h"
#include "log.h"

#define NES_HEADER_SIZE 0x10
#define VERTICAL_MIRROR true
#define HORIZONTAL_MIRROR false


class Cart
{
public:

    Cart(Logger& newLogger);
    Cart(char *filename, Logger& newLogger);
    ~Cart();

    u8 read(u16 address);
    void write(u16 address, u8 data);

private:

    std::unique_ptr<BasicMapper> mapper;
    Logger& logger;
    u32 prgRomSize;
    u32 chrRomSize;
    std::vector<u8> header;
    std::vector<u8> prgRom;
    std::vector<u8> chrRom;
    bool mirroring;
    
    void getHeaderData(std::ifstream &ifs);
    void getRomData(std::ifstream &ifs);
    void getMapper();
    void printHeader();
};

#endif