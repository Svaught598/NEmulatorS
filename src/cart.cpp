#include <iostream>
#include <fstream>

#include "../include/cart.h"



Cart::Cart(char* filepath){
    std::cout << "cartbegin\n";
    std::ifstream rom(filepath);

    u8 value;
    while (rom >> value){
        data.push_back(value);
    }

    for (auto element : data){
        std::cout << element << std::endl;
    }
    std::cout << "cartend";
}