#include "../inc/enigma.h"
#include <memory.h>

void _enigma_build_wheel(
    enigma_wheel_t *pWheel,
    uint32_t uWheelIndex,
    char *pSecret,
    uint32_t uSecretLength
) {

    uint32_t i, c;

    uint8_t x, bMap[32];

    memset(bMap, 0, sizeof bMap);

    for (i = 0, c = 0; i < 256; i++) {

        x = (pSecret[i % uSecretLength] * (i + c++ + uWheelIndex + 1) * uSecretLength) &
            0xFF;

        while (bMap[x >> 3] & (1 << (x & 7))) {

            x++;
        }

        bMap[x >> 3] |= (1 << (x & 7));

        pWheel->inbound[i] = x;
        pWheel->outbound[x] = i;
    }

}

void _enigma_build_reflect_board(
    uint8_t *pBoard,
    char *pSecret,
    uint32_t uSecretLength
) {

    uint32_t i, x;

    uint8_t bMap[32];

    memset(bMap, 0, sizeof bMap);

    for (i = 0; i < 256; i++) {

        if (bMap[i >> 3] & (1 << (i & 7))) {

            continue;
        }

        x = (pSecret[i % uSecretLength] * (i + 1) * uSecretLength) & 0xFF;

        while (bMap[x >> 3] & (1 << (x & 7))) {

            x++;
        }

        bMap[x >> 3] |= (1 << (x & 7));
        bMap[i >> 3] |= (1 << (i & 7));

        pBoard[i] = x;
        pBoard[x] = i;
    }
}

void enigma_rewind_context(enigma_context_t *ctx) {

    uint32_t i;

    for (i = 0; i < ctx->uWheels; i++) {

        ctx->pWheels[i].uOffset = 0;
    }
}

void enigma_init_context(
    enigma_context_t *ctx,
    uint32_t uWheels,
    char *pSecret,
    uint32_t uSecretLength
) {

    uint32_t i;

    memset(ctx, 0, ENIGMA_CALC_BUF_SIZE(uWheels));

    ctx->uWheels = uWheels;

    for (i = 0; i < uWheels; i++) {

        _enigma_build_wheel(
            ctx->pWheels + i,
            i,
            pSecret,
            uSecretLength
        );

        ctx->pWheels[i].uOffset = 0;
    }

    _enigma_build_reflect_board(
        ctx->pReflectBoard,
        pSecret,
        uSecretLength
    );
}

void enigma_exec(
    enigma_context_t *ctx,
    char *pOutput,
    char *pInput,
    uint32_t uBytes
) {

    uint32_t i, x, t;
    uint8_t c;

    for (x = 0; x < uBytes; x++) {

        c = pInput[x];

        for (i = 0; i < ctx->uWheels; i++) {

            c = ctx->pWheels[i].inbound[(c + ctx->pWheels[i].uOffset) & 0xFF];
        }

        c = ctx->pReflectBoard[c];

        for (i = 0; i < ctx->uWheels; i++) {

            c = (
                ctx->pWheels[ctx->uWheels - i - 1].outbound[c] -
                ctx->pWheels[ctx->uWheels - i - 1].uOffset
            ) & 0xFF;
        }

        pOutput[x] = c;

        for (i = 0; i < ctx->uWheels; i++) {

            if (ctx->pWheels[i].uOffset++ != 0xFF) {

                break;
            }
        }
    }
}
