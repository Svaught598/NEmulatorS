#include <string>
#include "6502.h"


class System
{
public:

    System(std::string name){
        systemName = name;
    };

private:

    std::string systemName;

};