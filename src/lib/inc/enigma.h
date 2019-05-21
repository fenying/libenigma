#ifndef ENIGMA_INC
#define ENIGMA_INC

#include <inttypes.h>

typedef struct {

    uint8_t inbound[256];

    uint8_t outbound[256];

    uint8_t uOffset;

    uint8_t uReserved[3];

} enigma_wheel_t;

typedef struct {

    uint8_t         pReflectBoard[256];

    uint32_t        uWheels;

    enigma_wheel_t  pWheels[1];

} enigma_context_t;

#define ENIGMA_CALC_BUF_SIZE(w) (0x104 + (w) * sizeof(enigma_wheel_t))

void enigma_init_context(
    enigma_context_t *ctx,
    uint32_t uWheels,
    char *pSecret,
    uint32_t uSecretLength
);

void enigma_rewind_context(enigma_context_t *ctx);

void enigma_exec(
    enigma_context_t *pContext,
    char *pOutput,
    char *pInput,
    uint32_t uBytes
);

#endif
