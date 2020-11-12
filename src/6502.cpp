#include <stdlib.h>
#include "6502.h"




///////////////////////////////////////////////
// Interface with Bus                        //
///////////////////////////////////////////////


// Read value @ address from memory via bus
u8 CPU::read(u16 address){
    // TODO
}


// Write value @ address in memory via bus
void CPU::write(u16 address, u8 value){
    // TODO
}


///////////////////////////////////////////////
// Methods for Flags                         //
///////////////////////////////////////////////


// Convert flags in CPU to a byte
u8 CPU::flagsToByte(){
    u8 flags = 0;
    if (C) {flags += 0x01;}
    if (Z) {flags += 0x02;}
    if (I) {flags += 0x04;}
    if (D) {flags += 0x08;}
    if (B) {flags += 0x10;}
    if (U) {flags += 0x20;}
    if (V) {flags += 0x40;}
    if (N) {flags += 0x80;}
    return flags;
}


// Convert byte to CPU flag values
void CPU::byteToFlags(u8 flags){
    (flags & 0x01) ? setCarry(true) : setCarry(false);
    (flags & 0x02) ? setZero(true) : setZero(false);
    (flags & 0x04) ? setInterrupt(true) : setInterrupt(false);
    (flags & 0x08) ? setDecimal(true) : setDecimal(false);
    (flags & 0x10) ? setBreak(true) : setBreak(false);
    (flags & 0x20) ? setUnused(true) : setUnused(false);
    (flags & 0x40) ? setOverflow(true) : setOverflow(false);
    (flags & 0x80) ? setNegative(true) : setNegative(false);
}


// Sets all CPU flags to false (clears them)
void CPU::clearFlags(){
    setCarry(false);
    setZero(false);
    setInterrupt(false);
    setDecimal(false);
    setBreak(false);
    setUnused(false);
    setOverflow(false);
    setNegative(false);
}


///////////////////////////////////////////////
// Methods for Stack Stuff                   //
///////////////////////////////////////////////


// Pulls a byte off the stack
u8 CPU::pullStack(){
    SP++;
    u8 value = read(0x0100 + SP);
    return value;
}


// Pushes a byte onto the stack
void CPU::pushStack(u8 value){
    write(0x0100 + SP, value);
    SP--;
}


///////////////////////////////////////////////
// Interface with Bus                        //
///////////////////////////////////////////////


// Exectutes current opcode
void CPU::execute(){
    switch (OP) {
        // MULTIPLE ADDRESSING MODES
        case 0x69: ADC(_IMM); break;
        case 0x65: ADC(_ZPG); break;
        case 0x75: ADC(_ZPX); break;
        case 0x6D: ADC(_ZPY); break;
        case 0x7D: ADC(_ABX); break;
        case 0x79: ADC(_ABY); break;
        case 0x61: ADC(_IDX); break;
        case 0x71: ADC(_IDY); break;

        case 0x29: AND(_IMM); break;
        case 0x25: AND(_ZPG); break;
        case 0x35: AND(_ZPX); break;
        case 0x2D: AND(_ABS); break;
        case 0x3D: AND(_ABX); break;
        case 0x39: AND(_ABY); break;
        case 0x21: AND(_IDX); break;
        case 0x31: AND(_IDY); break;

        case 0x0A: ASL(_ACC); break;
        case 0x06: ASL(_ZPG); break;
        case 0x16: ASL(_ZPX); break;
        case 0x0E: ASL(_ABS); break;
        case 0x1E: ASL(_ABX); break;

        case 0x24: BIT(_ZPG); break;
        case 0x2C: BIT(_ABS); break;

        case 0xC9: CMP(_IMM); break;
        case 0xC5: CMP(_ZPG); break;
        case 0xD5: CMP(_ZPX); break;
        case 0xCD: CMP(_ABS); break;
        case 0xDD: CMP(_ABX); break;
        case 0xD9: CMP(_ABY); break;
        case 0xC1: CMP(_IDX); break;
        case 0xD1: CMP(_IDY); break;

        case 0xE0: CPX(_IMM); break;
        case 0xE4: CPX(_ZPG); break;
        case 0xEC: CPX(_ABS); break;

        case 0xC0: CPY(_IMM); break;
        case 0xC4: CPY(_ZPG); break;
        case 0xCC: CPY(_ABS); break;

        case 0xC6: DEC(_ZPG); break;
        case 0xD6: DEC(_ZPX); break;
        case 0xCE: DEC(_ABS); break;
        case 0xDE: DEC(_ABX); break;

        case 0x49: EOR(_IMM); break;
        case 0x45: EOR(_ZPG); break;
        case 0x55: EOR(_ZPX); break;
        case 0x4D: EOR(_ABS); break;
        case 0x5D: EOR(_ABX); break;
        case 0x59: EOR(_ABY); break;
        case 0x41: EOR(_IDX); break;
        case 0x51: EOR(_IDY); break;

        case 0xE6: INC(_ZPG); break;
        case 0xF6: INC(_ZPX); break;
        case 0xEE: INC(_ABS); break;
        case 0xFE: INC(_ABX); break;

        case 0x4C: JMP(_ABS); break;
        case 0x6C: JMP(_IND); break;

        case 0xA9: LDA(_IMM); break;
        case 0xA5: LDA(_ZPG); break;
        case 0xB5: LDA(_ZPX); break;
        case 0xAD: LDA(_ABS); break;
        case 0xBD: LDA(_ABX); break;
        case 0xB9: LDA(_ABY); break;
        case 0xA1: LDA(_IDX); break;
        case 0xB1: LDA(_IDY); break;

        case 0xA2: LDX(_IMM); break;
        case 0xA6: LDX(_ZPG); break;
        case 0xB6: LDX(_ZPY); break;
        case 0xAE: LDX(_ABS); break;
        case 0xBE: LDX(_ABY); break;

        case 0xA0: LDY(_IMM); break;       
        case 0xA4: LDY(_ZPG); break;
        case 0xB4: LDY(_ZPX); break;
        case 0xAC: LDY(_ABS); break;
        case 0xBC: LDY(_ABX); break;

        case 0x4A: LSR(_ACC); break;
        case 0x46: LSR(_ZPG); break;
        case 0x56: LSR(_ZPY); break;
        case 0x4E: LSR(_ABS); break;        
        case 0x5E: LSR(_ABX); break;

        case 0x09: ORA(_IMM); break;
        case 0x05: ORA(_ZPG); break;
        case 0x15: ORA(_ZPX); break;
        case 0x0D: ORA(_ABS); break;
        case 0x1D: ORA(_ABX); break;
        case 0x19: ORA(_ABY); break;
        case 0x01: ORA(_IDX); break;
        case 0x11: ORA(_IDY); break;

        case 0x2A: ROL(_ACC); break;
        case 0x26: ROL(_ZPG); break;
        case 0x36: ROL(_ZPX); break;
        case 0x2E: ROL(_ABS); break;
        case 0x3E: ROL(_ABX); break;

        case 0x6A: ROR(_ACC); break;
        case 0x66: ROR(_ZPG); break;
        case 0x76: ROR(_ZPX); break;
        case 0x6E: ROR(_ABS); break;
        case 0x7E: ROR(_ABX); break;

        case 0xE9: SBC(_IMM); break;
        case 0xE5: SBC(_ZPG); break;
        case 0xF5: SBC(_ZPX); break;
        case 0xED: SBC(_ABS); break;
        case 0xFD: SBC(_ABX); break;
        case 0xF9: SBC(_ABY); break;
        case 0xE1: SBC(_IDX); break;
        case 0xF1: SBC(_IDY); break;

        case 0x85: STA(_ZPG); break;
        case 0x95: STA(_ZPX); break;
        case 0x8D: STA(_ABS); break;
        case 0x9D: STA(_ABX); break;
        case 0x99: STA(_ABY); break;
        case 0x81: STA(_IDX); break;
        case 0x91: STA(_IDY); break;

        case 0x86: STX(_ZPG); break;
        case 0x96: STX(_ZPY); break;
        case 0x8E: STX(_ABS); break;

        case 0x84: STY(_ZPG); break;
        case 0x94: STY(_ZPX); break;
        case 0x8C: STY(_ABS); break;

        // BRANCHING
        case 0x90: BCC(_REL); break;
        case 0xB0: BCS(_REL); break;
        case 0xF0: BEQ(_REL); break;
        case 0x30: BMI(_REL); break;
        case 0xD0: BNE(_REL); break;
        case 0x10: BPL(_REL); break;
        case 0x50: BVC(_REL); break;
        case 0x70: BVS(_REL); break;

        // INTERRUPTS
        case 0x00: BRK(); break;
        case 0x20: JSR(); break;
        case 0x40: RTI(); break;
        case 0x60: RTS(); break;

        // IMPLIED INSTRUCTIONS
        case 0x18: CLC(); break;
        case 0xD8: CLD(); break;
        case 0x58: CLI(); break;
        case 0xB8: CLV(); break;
        case 0xCA: DEX(); break;
        case 0x88: DEY(); break;
        case 0xE8: INX(); break;
        case 0xC8: INY(); break;
        case 0xEA: NOP(); break;
        case 0x48: PHA(); break;
        case 0x08: PHP(); break;
        case 0x68: PLA(); break;
        case 0x28: PLP(); break;
        case 0x38: SEC(); break;
        case 0xF8: SED(); break;
        case 0x78: SEI(); break;
        case 0xAA: TAX(); break;
        case 0xA8: TAY(); break;
        case 0xBA: TSX(); break;
        case 0x8A: TXA(); break;
        case 0x9A: TXS(); break;
        case 0x98: TYA(); break;
    }
}


// fetchs opcode at PC address
void CPU::fetch(){
    OP = read(PC);
}


///////////////////////////////////////////////
// Addressing Modes Funcs                    //
///////////////////////////////////////////////


// Immediate
u16 CPU::IMM(){
    // TODO
}

// Relative
u16 CPU::REL(){
    // TODO
}

// Accumulator
u16 CPU::ACC(){
    // TODO
}

// Zero Page
u16 CPU::ZPG(){
    // TODO
}

// Zero Page X
u16 CPU::ZPX(){
    // TODO
}

// Zero Page Y
u16 CPU::ZPY(){
    // TODO
}

// Absolute
u16 CPU::ABS(){
    // TODO
}

// Absolute X
u16 CPU::ABX(){
    // TODO
}

// Absolute Y
u16 CPU::ABY(){
    // TODO
}


// Indirect
u16 CPU::IND(){
    // TODO
}


// Indirect X
u16 CPU::IDX(){
    // TODO
}


// Indirect Y
u16 CPU::IDY(){
    // TODO
}

///////////////////////////////////////////////
// Multiple Addr. Modes Instructions         //
///////////////////////////////////////////////


// Or (With Accumulator)
void CPU::ORA(CPU::AMode mode){
    u16 address = (this->*modeTable[mode])();
    u8 M = read(address);

    A = A | M;
    if (A == 0) {setZero(true);}
    if (A & 0x80) {setNegative(true);}
};


// And (With Accumulator)
void CPU::AND(CPU::AMode mode){
    u16 address = (this->*modeTable[mode])();
    u8 M = read(address);

    A = A & M;
    if (A == 0) {setZero(true);}
    if (A & 0x80) {setNegative(true);}
}


// Exclusive Or
void CPU::EOR(CPU::AMode mode){
    u16 address = (this->*modeTable[mode])();
    u8 M = read(address);

    A = A ^ M;
    if (A == 0) {setZero(true);}
    if (A & 0x80) {setNegative(true);}
}


// Add with Carry
void CPU::ADC(CPU::AMode mode){
    u8 address = (this->*modeTable[mode])();
    u8 M = read(address);
    // TODO
}


// Store in Accumulator
void CPU::STA(CPU::AMode mode){
    u16 address = (this->*modeTable[mode])();
    write(address, A);
}


// Load to Accumulator
void CPU::LDA(CPU::AMode mode){
    u16 address = (this->*modeTable[mode])();
    u8 M = read(address);

    A = M;
    if (A == 0) {setZero(true);}
    if (A & 0x80) {setNegative(true);}
}


// Compare
void CPU::CMP(CPU::AMode mode){
    u16 address = (this->*modeTable[mode])();
    u8 M = read(address);

    u8 result = A - M;
    if (A >= M) {setCarry(true);}
    if (A == M) {setZero(true);}
    if (result & 0x80) {setNegative(true);}
}


// Subtract With Carry
void CPU::SBC(CPU::AMode mode){
    u16 address = (this->*modeTable[mode])();
    u8 M = read(address);
    // TODO
}


// Arithmetic Shift Left
void CPU::ASL(CPU::AMode mode){
    u16 address = (this->*modeTable[mode])();
    u8 M = read(address);
    // TODO
}


// Rotate Left
void CPU::ROL(CPU::AMode mode){
    u16 address = (this->*modeTable[mode])();
    u8 M = read(address);

    bool carry = M & 0x80;
    if (C) {M = (M << 1) + 1;}
    if (A == 0) {setZero(true);}
    (carry) ? setCarry(true) : setCarry(false);
}


// Logical Shift Right
void CPU::LSR(CPU::AMode mode){
    u16 address = (this->*modeTable[mode])();
    u8 M = read(address);
    // TODO
}


// Rotate Right
void CPU::ROR(CPU::AMode mode){
    u16 address = (this->*modeTable[mode])();
    u8 M = read(address);
    // TODO
}


// Store X Register
void CPU::STX(CPU::AMode mode){
    u16 address = (this->*modeTable[mode])();
    write(address, X);
}


// Load X Register
void CPU::LDX(CPU::AMode mode){
    u16 address = (this->*modeTable[mode])();
    u8 M = read(address);

    X = M;
    if (X == 0) {setZero(true);}
    if (X & 0x80) {setNegative(true);}
}


// Decrement Memory
void CPU::DEC(CPU::AMode mode){
    u16 address = (this->*modeTable[mode])();
    u8 M = read(address);
    
    M = M - 1;
    if (M == 0) {setZero(true);}
    if (M & 0x80) {setNegative(true);}
    write(address, M);
}


// Increment Memory
void CPU::INC(CPU::AMode mode){
    u16 address = (this->*modeTable[mode])();
    u8 M = read(address);
    
    M = M + 1;
    if (M == 0) {setZero(true);}
    if (M & 0x80) {setNegative(true);}
    write(address, M);
}


// Bit Test
void CPU::BIT(CPU::AMode mode){
    u16 address = (this->*modeTable[mode])();
    u8 M = read(address);

    (M & 0x80) ? setNegative(true) : setNegative(false);
    (M & 0x40) ? setOverflow(true) : setOverflow(false);
    if (A & M == 0) {setZero(true);}
}


// Jump
void CPU::JMP(CPU::AMode mode){
    u16 address = (this->*modeTable[mode])();
    u8 M = read(address);

    PC = M;
}


// Store Y Register
void CPU::STY(CPU::AMode mode){
    u16 address = (this->*modeTable[mode])();
    write(address, Y);
}


// Load Y Register
void CPU::LDY(CPU::AMode mode){
    u16 address = (this->*modeTable[mode])();
    u8 M = read(address);

    Y = M;
    if (Y == 0) {setZero(true);}
    if (Y & 0x80) {setNegative(true);}
}


// Compare Y Register
void CPU::CPY(CPU::AMode mode){
    u16 address = (this->*modeTable[mode])();
    u8 M = read(address);
    
    u8 result = Y - M;
    if (Y >= M) {setCarry(true);}
    if (Y == M) {setZero(true);}
    if (result & 0x80) {setNegative(true);}
}


// Compare X Register
void CPU::CPX(CPU::AMode mode){
    u16 address = (this->*modeTable[mode])();
    u8 M = read(address);
    
    u8 result = X - M;
    if (X >= M) {setCarry(true);}
    if (X == M) {setZero(true);}
    if (result & 0x80) {setNegative(true);}
}


///////////////////////////////////////////////
// Branching Instructions                    //
///////////////////////////////////////////////


// Branch if Positive
void CPU::BPL(CPU::AMode mode){
    u16 address = (this->*modeTable[mode])();
    u8 M = read(address);

    if (!N) {
        PC += M;
    }
}


// Branch if Minus
void CPU::BMI(CPU::AMode mode){
    u16 address = (this->*modeTable[mode])();
    u8 M = read(address);

    if (N) {
        PC += M;
    }
}


// Branch if Overflow Clear
void CPU::BVC(CPU::AMode mode){
    u16 address = (this->*modeTable[mode])();
    u8 M = read(address);

    if (!V) {
        PC += M;
    }
}


// Branch if Overflow Set
void CPU::BVS(CPU::AMode mode){
    u16 address = (this->*modeTable[mode])();
    u8 M = read(address);

    if (V) {
        PC += M;
    }
}


// Branch if Carry Clear
void CPU::BCC(CPU::AMode mode){
    u16 address = (this->*modeTable[mode])();
    u8 M = read(address);

    if (!C) {
        PC += M;
    }
}


// Branch if Carry Set
void CPU::BCS(CPU::AMode mode){
    u16 address = (this->*modeTable[mode])();
    u8 M = read(address);

    if (C) {
        PC += M;
    }
}


// Branch if Not Equal
void CPU::BNE(CPU::AMode mode){
    u16 address = (this->*modeTable[mode])();
    u8 M = read(address);

    if (!Z) {
        PC += M;
    }
}


// Branch if Equal
void CPU::BEQ(CPU::AMode mode){
    u16 address = (this->*modeTable[mode])();
    u8 M = read(address);

    if (Z) {
        PC += M;
    }
}

///////////////////////////////////////////////
// Interrupt Instructions                    //
///////////////////////////////////////////////


// Force Interrupt
void CPU::BRK(){
    //TODO
}


// Jump to Subroutine
void CPU::JSR(){
    //TODO
}


// Return from Interrupt
void CPU::RTI(){
    //TODO
}


// Return from Subroutine
void CPU::RTS(){
}

///////////////////////////////////////////////
// Single Byte Instructions                  //
///////////////////////////////////////////////


// Push Processor Status
void CPU::PHP(){
    //TODO
}


// Pull Processor Status
void CPU::PLP(){
    //TODO
}


// Push Accumulator
void CPU::PHA(){
    //TODO
}


// Pull Accumulator
void CPU::PLA(){
    //TODO
}


// Decrement Y Register
void CPU::DEY(){
    Y = Y - 1;
    if (Y == 0) {setZero(true);}
    if (Y & 0x80) {setNegative(true);}
}


// Transfer Accumulator to Y
void CPU::TAY(){
    Y = A;
    if (Y == 0) {setZero(true);}
    if (Y & 0x80) {setNegative(true);}
}


// Increment Y
void CPU::INY(){
    Y = Y + 1;
    if (Y == 0) {setZero(true);}
    if (Y & 0x80) {setNegative(true);}
}


// Increment X
void CPU::INX(){
    X = X + 1;
    if (X == 0) {setZero(true);}
    if (X & 0x80) {setNegative(true);}
}


// Clear Carry Flag
void CPU::CLC(){
    setCarry(false);
}


// Set Carry Flag
void CPU::SEC(){
    setCarry(true);
}


// Clear Interrupt Disable
void CPU::CLI(){
    setInterrupt(false);
}


// Set Interrupt Disable
void CPU::SEI(){
    setInterrupt(true);
}


// Transfer Y to Accumulator
void CPU::TYA(){
    A = Y;
    if (Y == 0) {setZero(true);}
    if (Y & 0x80) {setNegative(true);}
}


// Clear Overflow Flag
void CPU::CLV(){
    setOverflow(false);
}


// Clear Decimal Mode
void CPU::CLD(){
    setDecimal(false);
}


// Set Decimal Flag
void CPU::SED(){
    setDecimal(true);
}


// Transfer X to Accumulator
void CPU::TXA(){
    A = X;
    if (A == 0) {setZero(true);}
    if (A & 0x80) {setNegative(true);}
}


// Transfer X to Stack Pointer
void CPU::TXS(){
    SP = X;
}


// Transfer Accumulator to X
void CPU::TAX(){
    X = A;
    if (X == 0) {setZero(true);}
    if (X & 0x80) {setNegative(true);}
}


// Transfer Stack Pointer to X
void CPU::TSX(){
    X = SP;
    if (X == 0) {setZero(true);}
    if (X & 0x80) {setNegative(true);}
}


// Decrement X Register
void CPU::DEX(){
    X = X - 1;
    if (X == 0) {setZero(true);}
    if (X & 0x80) {setNegative(true);}
}


// No Operation
void CPU::NOP(){
    setOverflow(false);
}