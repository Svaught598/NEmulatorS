#include <stdlib.h>
#include <memory>
#include <iomanip>
#include <iostream>

#include <boost/format.hpp>

#include "../include/6502.h"
#include "../include/bus.h"

// We define an address outside of 
// memory for accumulator addressing/reading/writing
#define ACCUMULATOR_ADDRESS 0xFFFFFFFF

// address of IRQ interrupt vector in memory
#define IRQ_INTERRUPT 0xFFFE


// constructor
CPU::CPU(Bus& newBus, Logger& newLogger)
    : PC(0xFFFC), prevPC(0), SP(0xFD), A(0), X(0), Y(0), P(0x24), cycles(0)
    , error1(0), error2(0)
    , bus(&newBus), logger(newLogger){}

// fetch & execute opcode
void CPU::tick(){
    fetch();
    logState();
    execute();
    error1 = read(0x02);
    error2 = read(0x03);
}


void CPU::logState(){
    boost::format fmt = boost::format(                              
        "%1$#04x  %2$#04x         A:%3$#04X  X:%4$#04X  Y:%5$#04X  P:%6$#04X  SP:%7$#04X"
    ) % (int) PC % (int) OP % (int) A % (int) X % (int) Y % (int) P % (int) SP;
    //fmt.modify_item(1, std::group(std::setw(4), std::setfill('0')));
    logger \
        << PC << "  " << OP << "  " << mnemonic[OP] << "     "
        << "A:" << A << " "
        << "X:" << X << " "
        << "Y:" << Y << " "
        << "P:" << P << " "
        << "SP:" << SP << " " << Logger::logType::LOG_ENDLINE;
}


///////////////////////////////////////////////
// Interface with Bus                        //
///////////////////////////////////////////////


// Read value at address from memory via bus
u8 CPU::read(u32 address){
    if (address == ACCUMULATOR_ADDRESS){
        return A;
    }
    else {
        u16 trueAddress = (u16) address;
        return bus->read(address);
    }
}


// Write value @ address in memory via bus
void CPU::write(u32 address, u8 value){
    if (address == ACCUMULATOR_ADDRESS){
        A = value;
    }
    else {
        u16 trueAddress = (u16) address;
        bus->write(address, value);
    }
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
// GIANT SWITCH/CASE FOR INSTRUCTIONS        //
///////////////////////////////////////////////


// Exectutes current opcode
void CPU::execute(){
    switch (OP) {
        // MULTIPLE ADDRESSING MODES
        case 0x69: ADC(_IMM); break;
        case 0x65: ADC(_ZPG); break;
        case 0x75: ADC(_ZPX); break;
        case 0x6D: ADC(_ABS); break;
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

        default: NOP(); break;
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
u32 CPU::IMM(){
    u16 temp = PC + 1;
    PC += 2;
    return temp;
}

// Accumulator
u32 CPU::ACC(){
    PC += 1;
    return ACCUMULATOR_ADDRESS;
}

// Relative
u32 CPU::REL(){
    s32 address = PC;
    s8 offset = read(PC + 1);
    address += offset + 2;
    return (u16) address;
}

// Zero Page
u32 CPU::ZPG(){
    u8 address = read(PC + 1);
    PC += 2;
    return (u16) address;
}

// Zero Page X
u32 CPU::ZPX(){
    u32 address = read(PC + 1);
    address = (address + X) & 0xFF;
    PC += 2;
    return address;
}

// Zero Page Y
u32 CPU::ZPY(){
    u32 address = read(PC + 1);
    address = (address + Y) & 0xFF;
    PC += 2;
    return address;
}

// Absolute
u32 CPU::ABS(){
    u16 LSN = read(PC + 1);
    u16 MSN = read(PC + 2);
    u32 address = LSN + (MSN << 8);
    PC += 3;
    return address;
}

// Absolute X
u32 CPU::ABX(){
    u32 address = ABS();
    return address + X;
}

// Absolute Y
u32 CPU::ABY(){
    u32 address = ABS();
    return address + Y;
}

// Indirect
u32 CPU::IND(){
    u32 address = ABS();
    u16 LSN = read(address);
    u16 MSN = read(address + 1);
    address = (LSN + (MSN << 8)) & 0xFF;
    PC += 2;
    return address;
}

// Indirect X
u32 CPU::IDX(){
    u32 address = read(PC + 1);
    address = (address + X) & 0xFF;
    u16 LSN = read(address);
    u16 MSN = read(address + 1);
    address = LSN + (MSN << 8);
    PC += 2;
    return address;
}

// Indirect Y
u32 CPU::IDY(){
    u16 temp = read(PC + 1);
    u16 LSN = read(temp) + Y;
    u16 MSN = read(temp + 1);
    u32 address = LSN + (MSN << 8);
    PC += 2;
    return address;
}


///////////////////////////////////////////////
// Multiple Addr. Modes Instructions         //
///////////////////////////////////////////////


// Or (With Accumulator)
void CPU::ORA(CPU::AMode mode){
    u32 address = getAddress(mode);
    u8 M = read(address);

    A = A | M;
    setZero(A == 0);
    setNegative(A & 0x80);
};


// And (With Accumulator)
void CPU::AND(CPU::AMode mode){
    u32 address = getAddress(mode);
    u8 M = read(address);

    A = A & M;
    setZero(A == 0);
    setNegative(A & 0x80);
}


// Exclusive Or
void CPU::EOR(CPU::AMode mode){
    u32 address = getAddress(mode);
    u8 M = read(address);

    A = A ^ M;
    setZero(A == 0);
    setNegative(A & 0x80);
}


// Add with Carry
void CPU::ADC(CPU::AMode mode){
    u32 address = getAddress(mode);
    u8 M = read(address);
    
    u8 C = P & 0x01;
    u16 result = A+M+C;
    setZero((result & 0xFF) == 0);
    setCarry(result & 0x100);
    setNegative(result & 0x80);
    setOverflow(!((M^A) & 0x80) && ((M^result) & 0x80));
    A = (u8) result & 0xFF;
}


// Store Accumulator
void CPU::STA(CPU::AMode mode){
    u32 address = getAddress(mode);
    write(address, A);
}


// Load to Accumulator
void CPU::LDA(CPU::AMode mode){
    u32 address = getAddress(mode);
    u8 M = read(address);

    A = M;
    setZero(A == 0);
    setNegative(A & 0x80);
}


// Compare
void CPU::CMP(CPU::AMode mode){
    u32 address = getAddress(mode);
    u8 M = read(address);

    u8 result = A - M;
    setCarry(A >= M);
    setZero(A == M);
    setNegative(result & 0x80);
}


// Subtract With Carry
void CPU::SBC(CPU::AMode mode){
    u32 address = getAddress(mode);
    u8 M = read(address) ^ 0xFF;
    
    u8 C = P & 0x01;
    u16 result = A+M+C;
    setZero((result & 0xFF) == 0);
    setCarry(result & 0x100);
    setNegative(result & 0x80);
    setOverflow((result ^ A) & ((result ^ M) & 0x80));
    A = (u8) result & 0xFF;
}


// Arithmetic Shift Left
void CPU::ASL(CPU::AMode mode){
    u32 address = getAddress(mode);
    u8 M = read(address);

    setCarry(M & 0x80);
    M = M << 1;
    setZero(M == 0);
    setNegative(M & 0x80);
    write(address, M);
}


// Rotate Left
void CPU::ROL(CPU::AMode mode){
    u32 address = getAddress(mode);
    u8 M = read(address);

    u8 C = P & 0x01;
    bool carry = M & 0x80;
    M = (M << 1) + C;
    setZero(M == 0);
    setCarry(carry);
    setNegative(M & 0x80);
    write(address, M);
}


// Logical Shift Right
void CPU::LSR(CPU::AMode mode){
    u32 address = getAddress(mode);
    u8 M = read(address);

    bool carry = M & 1;
    M = M >> 1;
    setCarry(carry);
    setZero(M == 0);
    setNegative(M & 0x80);
    write(address, M);
}


// Rotate Right
void CPU::ROR(CPU::AMode mode){
    u32 address = getAddress(mode);
    u8 M = read(address);
    
    u8 C = P & 0x01;
    bool carry = M & 1;
    M = (M >> 1) + (C << 7);
    setCarry(carry);
    setZero(M == 0);
    setNegative(M & 0x80);
    write(address, M);
}


// Store X Register
void CPU::STX(CPU::AMode mode){
    u32 address = getAddress(mode);
    write(address, X);
}


// Load X Register
void CPU::LDX(CPU::AMode mode){
    u32 address = getAddress(mode);
    u8 M = read(address);

    X = M;
    setZero(X == 0);
    setNegative(X & 0x80);
}


// Decrement Memory
void CPU::DEC(CPU::AMode mode){
    u32 address = getAddress(mode);
    u8 M = read(address);
    
    M = M - 1;
    setZero(M == 0);
    setNegative(M & 0x80);
    write(address, M);
}


// Increment Memory
void CPU::INC(CPU::AMode mode){
    u32 address = getAddress(mode);
    u8 M = read(address);
    
    M = M + 1;
    setZero(M == 0);
    setNegative(M & 0x80);
    write(address, M);
}


// Bit Test
void CPU::BIT(CPU::AMode mode){
    u32 address = getAddress(mode);
    u8 M = read(address);
    u8 result = M & A;

    setNegative(M & 0x80);
    setOverflow(M & 0x40);
    setZero((result) == 0);
}


// Jump
void CPU::JMP(CPU::AMode mode){
    u32 address = getAddress(mode);
    PC = address;
}


// Store Y Register
void CPU::STY(CPU::AMode mode){
    u32 address = getAddress(mode);
    write(address, Y);
}


// Load Y Register
void CPU::LDY(CPU::AMode mode){
    u32 address = getAddress(mode);
    u8 M = read(address);

    Y = M;
    setZero(Y == 0);
    setNegative(Y & 0x80);
}


// Compare Y Register
void CPU::CPY(CPU::AMode mode){
    u32 address = getAddress(mode);
    u8 M = read(address);
    
    u8 result = Y - M;
    setCarry(Y >= M);
    setZero(Y == M);
    setNegative(result & 0x80);
}


// Compare X Register
void CPU::CPX(CPU::AMode mode){
    u32 address = getAddress(mode);
    u8 M = read(address);
    
    u8 result = X - M;
    setCarry(X >= M);
    setZero(result == 0);
    setNegative(result & 0x80);
}


///////////////////////////////////////////////
// Branching Instructions                    //
///////////////////////////////////////////////


// Branch if Positive
void CPU::BPL(CPU::AMode mode){
    if (!(P & 0x80)) {
        PC = getAddress(mode);
    }
    else {
        PC += 2;
    }
}


// Branch if Minus
void CPU::BMI(CPU::AMode mode){
    if (P & 0x80) {
        PC = getAddress(mode);
    }
    else {
        PC += 2;
    }
}


// Branch if Overflow Clear
void CPU::BVC(CPU::AMode mode){
    if (!(P & 0x40)) {
        PC = getAddress(mode);
    }
    else {
        PC += 2;
    }
}


// Branch if Overflow Set
void CPU::BVS(CPU::AMode mode){
    if (P & 0x40) {
        PC = getAddress(mode);
    }
    else {
        PC += 2;
    }
}


// Branch if Carry Clear
void CPU::BCC(CPU::AMode mode){
    if (!(P & 0x01)) {
        PC = getAddress(mode);
    }
    else {
        PC += 2;
    }
}


// Branch if Carry Set
void CPU::BCS(CPU::AMode mode){
    if (P & 0x01) {
        PC = getAddress(mode);
    }
    else {
        PC += 2;
    }
}


// Branch if Not Equal
void CPU::BNE(CPU::AMode mode){
    if (!(P & 0x02)) {
        PC = getAddress(mode);
    }
    else {
        PC += 2;
    }
}


// Branch if Equal
void CPU::BEQ(CPU::AMode mode){
    if (P & 0x02) {
        PC = getAddress(mode);
    }
    else {
        PC += 2;
    }
}

///////////////////////////////////////////////
// Interrupt Instructions                    //
///////////////////////////////////////////////


// Force Interrupt
void CPU::BRK(){
    // push stuff onto the stack
    pushStack(PC & 0x00FF);
    pushStack((PC & 0xFF00) >> 8);
    pushStack(P | 0x30);
    PC = IRQ_INTERRUPT;
}


// Jump to Subroutine
void CPU::JSR(){
    AMode mode = _ABS;
    u32 address = getAddress(mode);
    pushStack(((PC-1) & 0xFF00) >> 8);
    pushStack((PC-1) & 0x00FF);
    PC = (u16) address;
}


// Return from Interrupt
void CPU::RTI(){
    P = pullStack() & ~0x10 | 0x20;
    PC = pullStack() + (pullStack() << 8);
}


// Return from Subroutine
void CPU::RTS(){
    PC = pullStack() + (pullStack() << 8) + 1;
}

///////////////////////////////////////////////
// Single Byte Instructions                  //
///////////////////////////////////////////////


// Push Processor Status
void CPU::PHP(){
    pushStack(P | 0x30);
    PC += 1;
}


// Pull Processor Status
void CPU::PLP(){
    P = pullStack() & ~0x10 | 0x20;
    PC += 1;
}


// Push Accumulator
void CPU::PHA(){
    pushStack(A);
    PC += 1;
}


// Pull Accumulator
void CPU::PLA(){
    A = pullStack();
    setZero(A == 0);
    setNegative(A & 0x80);
    PC += 1;
}


// Decrement Y Register
void CPU::DEY(){
    Y = Y - 1;
    setZero(Y == 0);
    setNegative(Y & 0x80);
    PC += 1;
}


// Transfer Accumulator to Y
void CPU::TAY(){
    Y = A;
    setZero(Y == 0);
    setNegative(Y & 0x80);
    PC += 1;
}


// Increment Y
void CPU::INY(){
    Y = Y + 1;
    setZero(Y == 0);
    setNegative(Y & 0x80);
    PC += 1;
}


// Increment X
void CPU::INX(){
    X = X + 1;
    setZero(X == 0);
    setNegative(X & 0x80);
    PC += 1;
}


// Clear Carry Flag
void CPU::CLC(){
    setCarry(false);
    PC += 1;
}


// Set Carry Flag
void CPU::SEC(){
    setCarry(true);
    PC += 1;
}


// Clear Interrupt Disable
void CPU::CLI(){
    setInterrupt(false);
    PC += 1;
}


// Set Interrupt Disable
void CPU::SEI(){
    setInterrupt(true);
    PC += 1;
}


// Transfer Y to Accumulator
void CPU::TYA(){
    A = Y;
    setZero(Y == 0);
    setNegative(Y & 0x80);
    PC += 1;
}


// Clear Overflow Flag
void CPU::CLV(){
    setOverflow(false);
    PC += 1;
}


// Clear Decimal Mode
void CPU::CLD(){
    setDecimal(false);
    PC += 1;
}


// Set Decimal Flag
void CPU::SED(){
    setDecimal(true);
    PC += 1;
}


// Transfer X to Accumulator
void CPU::TXA(){
    A = X;
    setZero(A == 0);
    setNegative(A & 0x80);
    PC += 1;
}


// Transfer X to Stack Pointer
void CPU::TXS(){
    SP = X;
    PC += 1;
}


// Transfer Accumulator to X
void CPU::TAX(){
    X = A;
    setZero(X == 0);
    setNegative(X & 0x80);
    PC += 1;
}


// Transfer Stack Pointer to X
void CPU::TSX(){
    X = SP;
    setZero(X == 0);
    setNegative(X & 0x80);
    PC += 1;
}


// Decrement X Register
void CPU::DEX(){
    X = X - 1;
    setZero(X == 0);
    setNegative(X & 0x80);
    PC += 1;
}


// No Operation
void CPU::NOP(){
    PC += 1;
}