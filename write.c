#include "chunk.h"
#include <stdint.h>
#include <stdio.h>

void binaryPrint_char(char byte) {
    for (int i = 0; i < 8; i++) {
        printf("%d", !!((byte << i) & 0x80));
    }
    printf("\n");
}

void binaryPrint_uint16_t(uint16_t chunk) {
    for (int i = 0; i < 16; i++) {
        if (i == 8)
            printf(" ");
        printf("%d", !!((chunk << i) & 0x8000));
    }
    printf(" ");
}

void printOpcode(OpCode code, uint16_t chunk) {
    switch(code) {
        case OP_ADD:
            printADD(chunk);
            break;
        case OP_ADD_Imm5:
            printADD_Imm5(chunk);
            break;
        case OP_AND:
            printAND(chunk);
            break;
        case OP_AND_Imm5:
            printAND_Imm5(chunk);
            break;
        case OP_BR:
            printBR(chunk);
            break;
        case OP_JMP:
            printJMP(chunk);
            break;
        case OP_JSR:
            printJSR(chunk);
            break;
        case OP_JSRR:
            printJSRR(chunk);
            break;
        case OP_LD:
            printLD(chunk);
            break;
        case OP_LDI:
            printLDI(chunk);
            break;
        case OP_LDR:
            printLDR(chunk);
            break;
        case OP_LEA:
            printLEA(chunk);
            break;
        case OP_NOT:
            printLEA(chunk);
            break;
        case OP_RET:
            printRET(chunk);
            break;
        case OP_RTI:
            printRTI(chunk);
            break;
        case OP_ST:
            printST(chunk);
            break;
        case OP_STI:
            printSTI(chunk);
            break;
        case OP_STR:
            printSTR(chunk);
            break;
        case OP_TRAP:
            printTRAP(chunk);
            break;
        case reserved:
            printRESERVED(chunk);
            break;
    }
}
