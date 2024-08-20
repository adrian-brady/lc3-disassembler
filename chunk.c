#include "chunk.h"
#include "write.h"
#include <stdio.h>

uint16_t endian_swap(uint16_t x) {
    return (x>>8)|(x<<8);
}

OpCode getOpcode(uint16_t chunk) {
   uint16_t chunk_op = (chunk>>12);
    switch (chunk_op) {
        case 1: // 0001
            if ((chunk & (uint16_t)0x20)) {
                return OP_ADD_Imm5;
            } else {
                return OP_ADD;
            }
        case 5: // 0101
            if ((chunk & (uint16_t)0x20)) {
                return OP_AND_Imm5;
            } else {
                return OP_AND;
            }
        case 0: // 0000
            return OP_BR;
        case 12: //1100
            if (((chunk & 0x1C0)>>6) == 7) {
                return OP_RET;
            } else {
                return OP_JMP;
            }
        case 4: // 0100
            if ((chunk & (uint16_t)0x800)) {
                return OP_JSR;
            } else {
                return OP_JSRR;
            }
        case 2: //0010
            return OP_LD;
        case 10: // 1010
            return OP_LDI;
        case 6: // 0110
            return OP_LDR;
        case 14: // 1110
            return OP_LEA;
        case 9: // 1001
            return OP_NOT;
        case 8: // 1000
            return OP_RTI;
        case 3: // 0011
            return OP_ST;
        case 11: // 1011
            return OP_STI;
        case 7: // 0111
            return OP_STR;
        case 15: // 1111
            return OP_TRAP;
        default: // 1101
            return reserved;
    }
}

/* ADD Operation
 * 0001  |  ***  |  ***  |  0  |  00  |  ***
 *  OP      DR      SR1     I            SR2
 */
void printADD(uint16_t chunk) {
    uint8_t DR, SR1, SR2;
    DR = ((chunk & 0xE00)>>9);
    SR1 = ((chunk & 0x1C0)>>6);
    SR2 = ((chunk & 0x7));
    printf("ADD R%d,R%d,R%d\n", DR, SR1, SR2);
}

/* ADD Imm5 Operation
 * 0001  |  ***  |  ***  |  1  |  *****
 *  OP      DR      SR1     I     imm5
 */
void printADD_Imm5(uint16_t chunk) {
    uint8_t DR, SR1, imm5;
    DR = ((chunk & 0xE00)>>9);
    SR1 = ((chunk & 0x1C0)>>6);
    imm5 = (chunk & 0x1F);
    printf("ADD R%d,R%d,#%d\n", DR, SR1, imm5);
}

/* AND Operation
 * 0101  |  ***  |  ***  |  0  |  00  |  ***
 *  OP      DR      SR1     I            SR2
 */
void printAND(uint16_t chunk) {
    uint8_t DR, SR1, SR2;
    DR = ((chunk & 0xE00)>>9);
    SR1 = ((chunk & 0x1C0)>>6);
    SR2 = ((chunk & 0x7));
    printf("AND R%d,R%d,R%d\n", DR, SR1, SR2);
}

/* AND Imm5
 * 0101  |  ***  |  ***  |  1  |  *****
 *  OP      DR      SR1     I     imm5
 */
void printAND_Imm5(uint16_t chunk) {
    uint8_t DR, SR1, imm5;
    DR = ((chunk & 0xE00)>>9);
    SR1 = ((chunk & 0x1C0)>>6);
    imm5 = (chunk & 0x1F);
    printf("AND R%d,R%d,#%d\n", DR, SR1, imm5);
}

/* Branch Operation
 * 0000  |  ***  | *********
 *  OP      NZP     offest9
 */
void printBR(uint16_t chunk) {
    uint8_t N, Z, P;
    uint16_t PCoffset9;

    N = ((chunk & 0x800)>>11);
    Z = ((chunk & 0x400)>>10);
    P = ((chunk & 0x200)>>9);
    PCoffset9 = ((chunk & 0x1FF));
    printf("BR");
    if (N == 1 && Z == 1 && P == 1) {
        // print nothing.
    } else {
        if (N == 1) printf("n");
        if (Z == 1) printf("z");
        if (P == 1) printf("p");
    }

    printf(" x%x\n", PCoffset9);
}

/* Jump Operation
 * 1100  |  000  |  ***  | 000000
 *  OP             BaseR
 */
void printJMP(uint16_t chunk) {
    uint8_t BaseR = ((chunk & 0x1C0)>>6);
    printf("JMP R%d\n", BaseR);
}

/* Jump Subroutine Operation
 * 0101  |  ***  |  ***  |  1  |  *****
 *  OP      DR      SR1     I     imm5
 */
void printJSR(uint16_t chunk) {
    uint16_t PCoffset11 = (chunk & 0x7FF);
    printf("JSR %x\n", PCoffset11);
}

/* JSRR Operation
 * 0101  |  ***  |  ***  |  1  |  *****
 *  OP      DR      SR1     I     imm5
 */
void printJSRR(uint16_t chunk) {
    uint8_t BaseR = (chunk & 0x1C0)>>6;
    printf("JSRR R%d\n", BaseR);
}

void printLD(uint16_t chunk) {
    uint8_t DR = ((chunk & 0xE00)>>9);
    uint16_t PCoffset9 = (chunk & 0x1FF);
    printf("LD R%d,x%x\n", DR, PCoffset9);
}

void printLDI(uint16_t chunk) {
    uint8_t DR = ((chunk & 0xE00)>>9);
    uint16_t PCoffset9 = (chunk & 0x1FF);
    printf("LDI R%d,x%x\n", DR, PCoffset9);
}

void printLDR(uint16_t chunk) {
    uint8_t DR, BaseR, Offset6;
    DR = ((chunk & 0xE00)>>9);
    BaseR = ((chunk & 0x1C0)>>6);
    Offset6 = (chunk & 0x3F);
    printf("LDR R%d,R%d,#%d\n", DR, BaseR, Offset6);
}

void printLEA(uint16_t chunk) {
    uint8_t DR = ((chunk & 0xE00)>>9);
    uint16_t PCoffset9 = (chunk & 0x1FF);
    printf("LEA R%d,x%x\n", DR, PCoffset9);
}

void printNOT(uint16_t chunk) {
    uint8_t DR, SR;
    DR = ((chunk & 0xE00)>>9);
    SR = ((chunk & 0x1C0)>>6);
    printf("NOT R%d,R%d\n", DR, SR);
}

void printRET(uint16_t chunk) {
    printf("RET\n");
}

void printRTI(uint16_t chunk) {
    printf("RTI\n");
}

void printST(uint16_t chunk) {
    uint8_t SR = ((chunk & 0xE00)>>9);
    uint16_t PCoffset9 = (chunk & 0x1FF);
    printf("ST R%d,x%x\n", SR, PCoffset9);
}

void printSTI(uint16_t chunk) {
    uint8_t SR = ((chunk & 0xE00)>>9);
    uint16_t PCoffset9 = (chunk & 0x1FF);
    printf("STI R%d,x%x\n", SR, PCoffset9);
}

void printSTR(uint16_t chunk) {
    uint8_t DR, BaseR, Offset6;
    DR = ((chunk & 0xE00)>>9);
    BaseR = ((chunk & 0x1C0)>>6);
    Offset6 = (chunk & 0x3F);
    printf("STR R%d,R%d,#%d\n", DR, BaseR, Offset6);
}

void printTRAP(uint16_t chunk) {
    uint8_t vect = (chunk & 0xFF);
    switch (vect) {
        case 32:
            printf("GETC\n");
            break;
        case 33:
            printf("OUT\n");
            break;
        case 34:
            printf("PUTS\n");
            break;
        case 35:
            printf("IN\n");
            break;
        case 36:
            printf("PUTSP\n");
            break;
        case 37:
            printf("HALT\n");
            break;
        default:
            printf("TRAP x%x\n", vect);
            break;
    }
}

void printRESERVED(uint16_t chunk) {
    printf("RESERVED\n");
}
