#include <iostream>
#include <fstream>
#include <vector>

#include "../include/cart.h"


// CONSTRUCTOR
Cart::Cart(char* filepath){
    // open file stream for rom & extract header data
    std::ifstream rom(filepath);
    getHeaderData(rom);
    getRomData(rom);
    getMapper();
}


Cart::~Cart(){
    
}


// reads from the cartridge
u8 Cart::read(u16 address){
    return 1;
}


// gets the header data from the rom stream
void Cart::getHeaderData(std::ifstream &ifs){
    char buffer[NES_HEADER_SIZE];
    ifs.read(buffer, NES_HEADER_SIZE);

    HeaderData(header);
    header.name[0] = buffer[0];
    header.name[1] = buffer[1];
    header.name[2] = buffer[2];
    header.name[3] = buffer[3];
    header.PrgRomSize = buffer[4];
    header.ChrRomSize = buffer[5];
    header.Mapper1 = buffer[6];
    header.Mapper2 = buffer[7];
    header.PrgRamSize = buffer[8];
    header.TVSystem1 = buffer[9];
    header.TVSystem2 = buffer[10];
    header.unused[0] = buffer[11];
    header.unused[1] = buffer[12];
    header.unused[2] = buffer[13];
    header.unused[3] = buffer[14];
    header.unused[4] = buffer[15];
}


// Gets the mapperID from the header data & creates a pointer to the needed mapper
void Cart::getMapper(){
    u8 mapperID = (header.Mapper1 & 0xF0) >> 4 | (header.Mapper2 & 0xF0);

    switch (mapperID){
        case 0: Mapper000(mapper); break;
        default: std::cout << "FUCK THAT ROM" << std::endl; break;
    }
}


// gets prgRom, chrRom, etc.. from the rom stream
void Cart::getRomData(std::ifstream &ifs){
    mirroring = (header.Mapper1 & 0x01) ? VERTICAL_MIRROR : HORIZONTAL_MIRROR;
    if (header.Mapper1 & 0x04) ifs.seekg(512, std::ios_base::cur);

    // prgRomSize = header->PrgRomSize * 16384;
    // prgRom.resize(prgRomSize);
    // ifs.read((char*)prgRom.data(), prgRom.size());

    // chrRomSize = header->ChrRomSize * 8192;
    // chrRom.resize(chrRomSize);
    // ifs.read((char*)chrRom.data(), chrRom.size());
}

