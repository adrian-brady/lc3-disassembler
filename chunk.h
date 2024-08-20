#ifndef lc3_chunk_h
#define lc3_chunk_h

#include "common.h"

typedef enum {
    OP_ADD,
    OP_ADD_Imm5,
    OP_AND,
    OP_AND_Imm5,
    OP_BR,
    OP_JMP,
    OP_JSR,
    OP_JSRR,
    OP_LD,
    OP_LDI,
    OP_LDR,
    OP_LEA,
    OP_NOT,
    OP_RET,
    OP_RTI,
    OP_ST,
    OP_STI,
    OP_STR,
    OP_TRAP,
    reserved
} OpCode;

typedef struct {
    uint16_t instruction;
} Chunk;

uint16_t endian_swap(uint16_t chunk);

OpCode getOpcode(uint16_t chunk);

void printADD_Imm5(uint16_t chunk);
void printADD(uint16_t chunk);
void printAND_Imm5(uint16_t chunk);
void printAND(uint16_t chunk);
void printBR(uint16_t chunk);
void printJMP(uint16_t chunk);
void printJSR(uint16_t chunk);
void printJSRR(uint16_t chunk);
void printLD(uint16_t chunk);
void printLDI(uint16_t chunk);
void printLDR(uint16_t chunk);
void printLEA(uint16_t chunk);
void printNOT(uint16_t chunk);
void printRET(uint16_t chunk);
void printRTI(uint16_t chunk);
void printST(uint16_t chunk);
void printSTI(uint16_t chunk);
void printSTR(uint16_t chunk);
void printTRAP(uint16_t chunk);
void printRESERVED(uint16_t chunk);

#endif
