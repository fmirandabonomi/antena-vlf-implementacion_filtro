#ifndef BLOCKS_H
#define BLOCKS_H
#include <math.h>
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

#ifndef PI
#define PI 3.14159265358979323846
#endif
#ifndef AMPLITUDE
#define AMPLITUDE INT16_MAX
#endif
#ifndef NCO_GUARD_BITS
#define NCO_GUARD_BITS 3
#endif
#define NCO_STATE enum { COEF_REAL, COEF_IMAG, STATE_REAL, STATE_IMAG }

inline static void nco_init(int32_t (*mem)[4], double outFreq, double sampFreq)
{
    NCO_STATE;
    (*mem)[COEF_REAL]  = (int32_t)((1 << 30) * cos(-2 * PI * outFreq / sampFreq));
    (*mem)[COEF_IMAG]  = (int32_t)((1 << 30) * sin(-2 * PI * outFreq / sampFreq));
    (*mem)[STATE_REAL] = (int32_t)INT16_MAX << NCO_GUARD_BITS;
    (*mem)[STATE_IMAG] = 0;
}
#define MUL_I64(a, b) ((int64_t)(a) * (int64_t)(b))

inline static void nco_sample(int32_t (*mem)[4], int16_t (*cplx)[2])
{
    NCO_STATE;
    int32_t sta_r = (*mem)[STATE_REAL] >> NCO_GUARD_BITS;
    int32_t sta_i = (*mem)[STATE_IMAG] >> NCO_GUARD_BITS;
    if (!sta_i || sta_r > AMPLITUDE || sta_r < -AMPLITUDE) {
        sta_r              = sta_r >= 0 ? AMPLITUDE : -AMPLITUDE;
        sta_i              = 0;
        (*mem)[STATE_REAL] = sta_r << NCO_GUARD_BITS;
        (*mem)[STATE_IMAG] = 0;
    } else if (!sta_r || sta_i > AMPLITUDE || sta_r < -AMPLITUDE) {
        sta_r              = 0;
        sta_i              = sta_i >= 0 ? AMPLITUDE : -AMPLITUDE;
        (*mem)[STATE_REAL] = 0;
        (*mem)[STATE_IMAG] = sta_i << NCO_GUARD_BITS;
    }
    (*cplx)[0] = sta_r;
    (*cplx)[1] = sta_i;
    (*mem)[STATE_REAL] =
        (int32_t)((MUL_I64((*mem)[COEF_REAL], (*mem)[STATE_REAL]) - MUL_I64((*mem)[COEF_IMAG], (*mem)[STATE_IMAG]))
                  >> 30);
    (*mem)[STATE_IMAG] =
        (int32_t)((MUL_I64((*mem)[COEF_IMAG], (*mem)[STATE_REAL]) + MUL_I64((*mem)[COEF_REAL], (*mem)[STATE_IMAG]))
                  >> 30);
}
#undef NCO_STATE
#undef MUL_I64

#endif // BLOCKS_H

