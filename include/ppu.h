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

private:

    Bus* bus = nullptr;
    Logger& logger;

};

#endif