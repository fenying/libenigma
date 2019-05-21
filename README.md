# C Library for Enigma

A simple C implementation of Enigma.

## Build

```sh
./make.sh
```

## CLI Usage

```sh
enigma <WHEELS> <KEY> <INFILE> <OUTFILE>
```

- `WHEELS` Specify how many wheels the Enigma should use.
- `KEY` Specify the key to initialize the Enigma.
- `INFILE` Specify the path to the input file.
- `OUTFILE` Specify the path to the output file.

## API Usage

```c
#include <enigma.h>

#define ENIGMA_WHEELS   10

#define ENIGMA_KEY      "this is the key"

void example() {

    enigma_context_t *ctx = (enigma_context_t *)malloc(
        // Calculate the space for 10 wheels Enigma.
        ENIGMA_CALC_BUF_SIZE(ENIGMA_WHEELS)
    );

    char buf[] = "Hello world!";

    enigma_init_context(
        ctx,
        ENIGMA_WHEELS,
        ENIGMA_KEY,
        sizeof(ENIGMA_KEY)
    );

    // Encryption.
    enigma_exec(
        ctx,
        buf,        // Output buffer
        buf,        // Input buffer
        sizeof(buf) // The bytes length of input buffer.
    );

    // Reset the cursor of context, for reusing.
    enigma_rewind_context(ctx);

    /**
     * Decryption, just run again enigma_exe.
     *
     * NOTICE: Run enigma_rewind_context to reset the context.
     */
    enigma_exec(
        ctx,
        buf,        // Output buffer
        buf,        // Input buffer
        sizeof(buf) // The bytes length of input buffer.
    );

    free(ctx);
}
```

## LICENSE

This project is published under [MIT](./LICENSE) license.
