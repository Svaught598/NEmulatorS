#ifndef NES_CART
#define NES_CART

#include <iostream>
#include <fstream>
#include <vector>

#include "typedefs.h"


class Cart
{
    /**
     * Cartridge class
    */
public:

    Cart(char *filename);
    ~Cart();

private:

    std::vector<u8> data;
    

};





#endif