#include "../include/mappers.h"


BasicMapper::BasicMapper(u8 &_numPrgBanks, u8 &_numChrBanks)
    : numPrgBanks(_numPrgBanks), numChrBanks(_numChrBanks){}


/* Mapper 000 */


Mapper000::Mapper000(u8 &_numPrgBanks, u8 &_numChrBanks)
    : BasicMapper(_numPrgBanks, _numPrgBanks){}


u16 Mapper000::getMappedAddress(u16 &address){
    if (numPrgBanks > 1){
        // Masking w/ 7FFF removes 15th bit: drops 8000 to 0
        address = address & 0x7FFF;
    } else {
        // Masking w/ 3FFF mirrors bank 1: C000-FFFF effectively mirrors 8000-BFFF
        address = address & 0x3FFF;
    }
}
