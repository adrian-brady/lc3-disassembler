#include <stdio.h>
#include <stdlib.h>

#include "chunk.h"
#include "write.h"

#define IN "biggest.obj"



int main() {
    FILE* fp = fopen(IN, "rb");
    if (!fp) {
        exit(1);
    }

    uint16_t chunk = 0;

    fread(&chunk, 2, 1, fp); // read ORIG value
    chunk = endian_swap(chunk);
    printf(".ORIG x%x\n", chunk);

    while (!feof(fp)) {
        size_t bytesRead = fread(&chunk, sizeof(chunk), 1, fp);
        // check if fread read not enough bytes
        if (bytesRead < sizeof(chunk)) {
            if (feof(fp)) {
                break;
            } else if (ferror(fp)) {
                break;
            }
        }
        chunk = endian_swap(chunk);
        OpCode code = getOpcode(chunk);
        printOpcode(code, chunk);
    }

    fclose(fp);
    printf(".END\n");
    return 0;
}
