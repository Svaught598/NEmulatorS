#ifndef NES_MAPPERS
#define NES_MAPPERS

#include "typedefs.h"


class BasicMapper
{
public:

    BasicMapper(u8 &_numPrgBanks, u8 &_numChrBanks);
    ~BasicMapper();

    virtual u16 getMappedAddress(u16 &address);

protected:

    u8 numPrgBanks = 0;
    u8 numChrBanks = 0;

};


class Mapper000: public BasicMapper
{
public:

    Mapper000(u8 &_numPrgBanks, u8 &_numChrBanks);
    ~Mapper000();

    u16 getMappedAddress(u16 &address);

private:

};

#endif