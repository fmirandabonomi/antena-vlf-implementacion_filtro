#include "blocks.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
// #define STEP_STIMULUS // if not defined, delta stimulus

static void demoFilter(void);
static void demoNco(void);
enum { NSAMPLES = 1024, CS_GUARDA = 4 };

int main(int argc, char **argv)
{
    if (argc < 2 || !strcmp("filter", argv[1])) demoFilter();
    else if (argc > 1 && !strcmp("nco", argv[1])) demoNco();
    return 0;
}

static void demoFilter(void)
{
    static int32_t impulse_response[NSAMPLES] = {INT16_MAX << CS_GUARDA};
    static struct {
        struct ConjugatePolePair_s px1[1], px2[1];
        int32_t pz1[1], pz2[1];
        struct Comb_s cmb1[1], cmb2[1];
        int32_t memCmb1[16], memCmb2[16];
    } sta = {0};
    ConjugatePolePair_initRadFrec(sta.px1, 24.0 / 25.0, 3, 49);
    ConjugatePolePair_initRadFrec(sta.px2, 24.0 / 25.0, 3, 49);
    Comb_init(sta.cmb1, 14, sizeof(sta.memCmb1) / sizeof(*sta.memCmb1), sta.memCmb1);
    Comb_init(sta.cmb2, 14, sizeof(sta.memCmb2) / sizeof(*sta.memCmb2), sta.memCmb2);

    for (int i = 0; i < NSAMPLES; ++i) {
#ifdef STEP_STIMULUS
        int32_t aux = 1000 << CS_GUARDA;
#else
        int32_t aux = impulse_response[i];
#endif
        aux                 = ConjugatePolePair_step(sta.px1, aux);
        aux                 = ConjugatePolePair_step(sta.px2, aux);
        aux                 = poleAtFrecZero_step(sta.pz1, aux);
        aux                 = poleAtFrecZero_step(sta.pz2, aux);
        aux                 = Comb_step(sta.cmb1, aux);
        aux                 = Comb_step(sta.cmb2, aux);
        impulse_response[i] = (aux >> CS_GUARDA) >> 13;
    }
    for (int i = 0; i < NSAMPLES; ++i) {
        printf("%d\n", impulse_response[i]);
    }
}

static void demoNco(void)
{
    static struct Nco_s osc[1];
    Nco_init(osc, -30, 72000.0 / (6 * (12.5 + 41.5)));
    for (int i = 0; i < NSAMPLES; ++i) {
        const CplxI16 sample = Nco_sample(osc);
        printf("%hd%+hdj\n", sample.real, sample.imag);
    }
}

