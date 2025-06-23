#ifndef BLOCKS_H
#define BLOCKS_H
#include <stdint.h>

#ifdef TRACE_EXEC
#include <stdio.h>
#endif

#define DELAY(delay, phase, mem, mem_size) \
    (mem)[(((unsigned)(phase) - ((unsigned)(delay) - 1U)) % (unsigned)(mem_size))]

#define SHIFT_IN(elem, phase, mem, mem_size) \
    do { \
        ++(phase); \
        (mem)[(unsigned)(phase) % (unsigned)(mem_size)] = (elem); \
    } while (0)

inline static int32_t pole_24r25_3f49(int32_t input, int32_t (*mem)[2], uint8_t *phase)
{
    // 30 bit fraccion
    int32_t a1     = -1910917036L;
    int32_t a2     = 989560464L;
    int32_t t1     = (-(int64_t)a1 * (int64_t)DELAY(1, *phase, *mem, 2)) >> 30;
    int32_t t2     = (-(int64_t)a2 * (int64_t)DELAY(2, *phase, *mem, 2)) >> 30;
    int32_t output = t1 + t2 + input;

#ifdef TRACE_EXEC
    printf("pd[%p]: input %d, output %d, input-1: %d, input-2: %d\n", (void *)mem, input, output, mem[(*phase) % 2],
           mem[(*phase - 1) % 2]);
#endif
    SHIFT_IN(output, *phase, *mem, 2);
    return output;
}

inline static int32_t pole_1r_0f(int32_t input, int32_t mem[1])
{
    int32_t output = input + *mem;
#ifdef TRACE_EXEC
    printf("pz[%p]: input %d, output %d, output-1: %d\n", (void *)mem, input, output, *mem);
#endif
    *mem = output;
    return output;
}

inline static int32_t comb_14d(int32_t input, int32_t (*mem)[16], uint8_t *phase)
{
    int32_t output = input - DELAY(14, *phase, *mem, 16);
#ifdef TRACE_EXEC
    printf("cmb[%p]: input: %d, output: %d\n", mem, input, output);
    for (int i = 0; i < 14; ++i) {
        printf("input-%d: %d ", i + 1, DELAY(i + 1, *phase, *mem, 16));
        printf("\n");
    }
#endif
    SHIFT_IN(input, *phase, *mem, 16);
    return output;
}
#endif // BLOCKS_H

