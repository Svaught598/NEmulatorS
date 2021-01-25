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
    /**
     * Bus class
    */

public:

    Bus();
    ~Bus();

    void connectCart(std::shared_ptr<Cart> newCart);
    void connectCPU(std::shared_ptr<CPU> newCpu);
    void connectLogger(std::shared_ptr<Logger> newLogger);

    void write(u16 address, u8 data);
    u8 read(u16 address);

private:

    std::shared_ptr<Cart> cart;
    std::shared_ptr<CPU> cpu;
    std::shared_ptr<Logger> logger;

    u8 mram[0x2000];
};

#endif