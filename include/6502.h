#ifndef NES_6502
#define NES_6502

#include <stdlib.h>
#include <functional>

#include "bus.h"
#include "typedefs.h"


class CPU
{
    /**
     * CPU class
    */
public:

    // constructor/destructor
    CPU(Bus* _bus){
        connectBus(_bus);
    };
    
    ~CPU(){
        delete bus;
    };

    // exposed attributes
    u64 cycles;

private:

    // Bus
    Bus* bus;

    // Working variables
    u8 OP;

    // Registers
    u16 PC;
    u8 SP;
    u8 A;
    u8 X;
    u8 Y;

    // Status Flags
    bool C;
    bool Z;
    bool I;
    bool D;
    bool B;
    bool U;
    bool V;
    bool N;

    // Interacting with Memory via Bus
    void connectBus(Bus* _bus);
    u8 read(u32 address);
    void write(u32 address, u8 value);

    // Exectuting opcodes & ticks 
    void execute();
    void fetch();

    ///////////////////////////////////////////////
    // Opcodes/AddressingModes/UtilityMethods    //
    ///////////////////////////////////////////////

    // Flag related methods
    void setCarry(bool val){C = val;}
    void setZero(bool val){Z = val;}
    void setInterrupt(bool val){I = val;}
    void setDecimal(bool val){D = val;}
    void setBreak(bool val){B = val;}
    void setUnused(bool val){U = val;}
    void setOverflow(bool val){V = val;}
    void setNegative(bool val){N = val;}

    // storing/loading flags
    u8 flagsToByte();
    void byteToFlags(u8 flags);
    void clearFlags();

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
};
#endif