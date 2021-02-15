#ifndef NES_6502
#define NES_6502

#include <functional>
#include <memory>
#include <unordered_map>
#include <string>

#include "typedefs.h"
#include "log.h"


// circular include if I #include "bus.h"
// so I'm just going with a declaration
class Bus;


class CPU
{
    /**
     * CPU class
    */
public:

    // constructor/destructor
    CPU(Bus& newBus, Logger& newLogger);
    ~CPU(){};

    
    void tick();
    void connectLogger(Logger& newLogger);
    u64 cycles;
    u8 OP;
    u16 PC;
    u16 prevPC;
    u8 SP;
    u8 A;
    u8 X;
    u8 Y;
    u8 P;

private:

    Bus* bus = nullptr;
    Logger& logger;

    u8 read(u32 address);
    void write(u32 address, u8 value);

    void execute();
    void fetch();

    void reset();   // https://www.pagetable.com/?p=410

    void logState();

    ///////////////////////////////////////////////
    // Opcodes/AddressingModes/UtilityMethods    //
    ///////////////////////////////////////////////

    // Flag related methods
    void setCarry(bool val)    {P = (val) ? P | 0x01 : P & ~0x01;}
    void setZero(bool val)     {P = (val) ? P | 0x02 : P & ~0x02;}
    void setInterrupt(bool val){P = (val) ? P | 0x04 : P & ~0x04;}
    void setDecimal(bool val)  {P = (val) ? P | 0x08 : P & ~0x08;}
    void setBreak(bool val)    {P = (val) ? P | 0x10 : P & ~0x10;}
    void setUnused(bool val)   {P = (val) ? P | 0x20 : P & ~0x20;}
    void setOverflow(bool val) {P = (val) ? P | 0x40 : P & ~0x40;}
    void setNegative(bool val) {P = (val) ? P | 0x80 : P & ~0x80;}
    void clearFlags(){P = 0;}

    // Pushing/Pulling to Stack
    u8 pullStack();
    void pushStack(u8 value);

    // Addressing modes
    u32 IMM(); 
    u32 REL(); 
    u32 ZPG(); 
    u32 ZPX(); 
    u32 ZPY(); 
    u32 ABS(); 
    u32 ABX(); 
    u32 ABY();
    u32 IND();
    u32 IDX();
    u32 IDY();
    u32 ACC();

    enum AMode {
        _IMM = 0, 
        _REL = 1, 
        _ZPG = 2, 
        _ZPX = 3, 
        _ZPY = 4, 
        _ABS = 5, 
        _ABX = 6, 
        _ABY = 7,
        _IND = 8,
        _IDX = 9,
        _IDY = 10,
        _ACC = 11,
    };

    u32 getAddress(AMode &mode){
        switch (mode) {
            case _IMM: return IMM();
            case _REL: return REL();
            case _ZPG: return ZPG();
            case _ZPX: return ZPX();
            case _ZPY: return ZPY();
            case _ABS: return ABS();
            case _ABX: return ABX();
            case _ABY: return ABY();
            case _IND: return IND();
            case _IDX: return IDX();
            case _IDY: return IDY();
            case _ACC: return ACC();
            default: return 0;
        }
    };

    // Instructions
    // Variety of Addressing Modes
    void ORA(AMode mode); 
    void AND(AMode mode); 
    void EOR(AMode mode); 
    void ADC(AMode mode);
    void STA(AMode mode); 
    void LDA(AMode mode); 
    void CMP(AMode mode); 
    void SBC(AMode mode);
    void ASL(AMode mode); 
    void ROL(AMode mode); 
    void LSR(AMode mode); 
    void ROR(AMode mode);
    void STX(AMode mode); 
    void LDX(AMode mode); 
    void DEC(AMode mode); 
    void INC(AMode mode);
    void BIT(AMode mode); 
    void JMP(AMode mode); 
    void STY(AMode mode); 
    void LDY(AMode mode); 
    void CPY(AMode mode); 
    void CPX(AMode mode);

    // Branching Instructions
    // Addressing mode is Relative
    void BPL(AMode mode); 
    void BMI(AMode mode); 
    void BVC(AMode mode); 
    void BVS(AMode mode);
    void BCC(AMode mode); 
    void BCS(AMode mode); 
    void BNE(AMode mode); 
    void BEQ(AMode mode);

    // Interrupt/Subroutine Instructions
    // Addressing Mode is Implied -> No argument
    void BRK(); 
    void JSR(); 
    void RTI(); 
    void RTS();

    // Other Single-Byte Instructions
    // Addressing Mode is Implied -> No argument
    void PHP(); 
    void PLP(); 
    void PHA(); 
    void PLA();
    void DEY(); 
    void TAY(); 
    void INY(); 
    void INX();
    void CLC(); 
    void SEC(); 
    void CLI(); 
    void SEI();
    void TYA(); 
    void CLV(); 
    void CLD(); 
    void SED();
    void TXA(); 
    void TXS(); 
    void TAX(); 
    void TSX();
    void DEX(); 
    void NOP();


    std::unordered_map<u8, std::string> mnemonic = {
        {0x69, "ADC"},
        {0x65, "ADC"},
        {0x75, "ADC"},
        {0x6D, "ADC"},
        {0x7D, "ADC"},
        {0x79, "ADC"},
        {0x61, "ADC"},
        {0x71, "ADC"},

        {0x29, "AND"},
        {0x25, "AND"},
        {0x35, "AND"},
        {0x2D, "AND"},
        {0x3D, "AND"},
        {0x39, "AND"},
        {0x21, "AND"},
        {0x31, "AND"},

        {0x0A, "ASL"},
        {0x06, "ASL"},
        {0x16, "ASL"},
        {0x0E, "ASL"},
        {0x1E, "ASL"},

        {0x24, "BIT"},
        {0x2C, "BIT"},

        {0xC9, "CMP"},
        {0xC5, "CMP"},
        {0xD5, "CMP"},
        {0xCD, "CMP"},
        {0xDD, "CMP"},
        {0xD9, "CMP"},
        {0xC1, "CMP"},
        {0xD1, "CMP"},

        {0xE0, "CPX"},
        {0xE4, "CPX"},
        {0xEC, "CPX"},

        {0xC0, "CPY"},
        {0xC4, "CPY"},
        {0xCC, "CPY"},

        {0xC6, "DEC"},
        {0xD6, "DEC"},
        {0xCE, "DEC"},
        {0xDE, "DEC"},

        {0x49, "EOR"},
        {0x45, "EOR"},
        {0x55, "EOR"},
        {0x4D, "EOR"},
        {0x5D, "EOR"},
        {0x59, "EOR"},
        {0x41, "EOR"},
        {0x51, "EOR"},

        {0xE6, "INC"},
        {0xF6, "INC"},
        {0xEE, "INC"},
        {0xFE, "INC"},

        {0x4C, "JMP"},
        {0x6C, "JMP"},

        {0xA9, "LDA"},
        {0xA5, "LDA"},
        {0xB5, "LDA"},
        {0xAD, "LDA"},
        {0xBD, "LDA"},
        {0xB9, "LDA"},
        {0xA1, "LDA"},
        {0xB1, "LDA"},

        {0xA2, "LDX"},
        {0xA6, "LDX"},
        {0xB6, "LDX"},
        {0xAE, "LDX"},
        {0xBE, "LDX"},

        {0xA0, "LDY"},
        {0xA4, "LDY"},
        {0xB4, "LDY"},
        {0xAC, "LDY"},
        {0xBC, "LDY"},

        {0x4A, "LSR"},
        {0x46, "LSR"},
        {0x56, "LSR"},
        {0x4E, "LSR"},
        {0x5E, "LSR"},

        {0x09, "ORA"},
        {0x05, "ORA"},
        {0x15, "ORA"},
        {0x0D, "ORA"},
        {0x1D, "ORA"},
        {0x19, "ORA"},
        {0x01, "ORA"},
        {0x11, "ORA"},

        {0x2A, "ROL"},
        {0x26, "ROL"},
        {0x36, "ROL"},
        {0x2E, "ROL"},
        {0x3E, "ROL"},

        {0x6A, "ROR"},
        {0x66, "ROR"},
        {0x76, "ROR"},
        {0x6E, "ROR"},
        {0x7E, "ROR"},

        {0xE9, "SBC"},
        {0xE5, "SBC"},
        {0xF5, "SBC"},
        {0xED, "SBC"},
        {0xFD, "SBC"},
        {0xF9, "SBC"},
        {0xE1, "SBC"},
        {0xF1, "SBC"},

        {0x85, "STA"},
        {0x95, "STA"},
        {0x8D, "STA"},
        {0x9D, "STA"},
        {0x99, "STA"},
        {0x81, "STA"},
        {0x91, "STA"},

        {0x86, "STX"},
        {0x96, "STX"},
        {0x8E, "STX"},

        {0x84, "STY"},
        {0x94, "STY"},
        {0x8C, "STY"},

        // BRANCHING
        {0x90, "BCC"},
        {0xB0, "BCS"},
        {0xF0, "BEQ"},
        {0x30, "BMI"},
        {0xD0, "BNE"},
        {0x10, "BPL"},
        {0x50, "BVC"},
        {0x70, "BVS"},


        // INTERRUPTS
        {0x00, "BRK"},
        {0x20, "JSR"},
        {0x40, "RTI"},
        {0x60, "RTS"},

        // IMPLIED INSTRUCTIONS
        {0x18, "CLC"},
        {0xD8, "CLD"},
        {0x58, "CLI"},
        {0xB8, "CLV"},
        {0xCA, "DEX"},
        {0x88, "DEY"},
        {0xE8, "INX"},
        {0xC8, "INY"},
        {0xEA, "NOP"},
        {0x48, "PHA"},
        {0x08, "PHP"},
        {0x68, "PLA"},
        {0x28, "PLP"},
        {0x38, "SEC"},
        {0xF8, "SED"},
        {0x78, "SEI"},
        {0xAA, "TAX"},
        {0xA8, "TAY"},
        {0xBA, "TSX"},
        {0x8A, "TXA"},
        {0x9A, "TXS"},
        {0x98, "TYA"}
    };
};


#endif