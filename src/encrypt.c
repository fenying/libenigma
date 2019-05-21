#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include "lib/inc/enigma.h"

#define ENIGMA_WHEEL_QUANTITY 5UL

void printHelp() {

    printf("USAGE: enigma <WHEELS-QUANTITY> <KEY> <INFILE> <OUTFILE>\n");
}

int main(int argc, char** argv) {

    enigma_context_t *ctx;

    uint8_t buf[8192];

    uint32_t t, len;

    FILE *pInFile, *pOutFile;

    pInFile = NULL;

    pOutFile = NULL;

    if (argc != 5) {

        printHelp();
        return 0;
    }

    t = atoi(argv[1]);

    if (!t) {

        printHelp();
        return 0;
    }

    ctx = (enigma_context_t *)malloc(ENIGMA_CALC_BUF_SIZE(t));

    if (!ctx) {

        printf("ERROR: Out of memory!\n");
        return 0;
    }

    enigma_init_context(ctx, t, argv[2], strlen(argv[2]));

    pInFile = fopen(argv[3], "rb");

    if (!pInFile) {

        printf("ERROR: Can not open file '%s'.\n", argv[3]);
        goto onError;
    }

    pOutFile = fopen(argv[4], "wb");

    if (!pOutFile) {

        printf("ERROR: Can not open file '%s'.\n", argv[4]);
        goto onError;
    }

    fseek(pInFile, 0, SEEK_END);

    t = ftell(pInFile);

    fseek(pInFile, 0, SEEK_SET);

    while (t > 0) {

        len = t >= sizeof(buf) ? sizeof(buf) : t;

        t -= len;

        if (!fread(buf, len, 1, pInFile)) {

            printf("ERROR: Failed to read data from file '%s'.\n", argv[3]);
            break;
        }

        enigma_exec(ctx, buf, buf, len);

        if (!fwrite(buf, len, 1, pOutFile)) {

            printf("ERROR: Failed to write data to file '%s'.\n", argv[4]);
            break;
        }
    }

onError:

    if (pOutFile) {

        fclose(pOutFile);
    }

    if (pInFile) {

        fclose(pInFile);
    }

    free(ctx);

    return 0;
}
