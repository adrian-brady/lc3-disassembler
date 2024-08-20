#ifndef lc3_write_h
#define lc3_write_h

#include "chunk.h"

char* registerString(int id);
char* opcodeString(OpCode opcode);
char* conditionString(bool n, bool z, bool p);
char* offsetString(int length, uint16_t value);
void binaryPrint_char(char byte);
void binaryPrint_uint16_t(uint16_t chunk);
void printOpcode(OpCode code, uint16_t chunk);

#endif
