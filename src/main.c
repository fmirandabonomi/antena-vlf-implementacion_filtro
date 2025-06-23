#include "blocks.h"
#include <stdint.h>
#include <stdio.h>

int main(void)
{
    enum { NSAMPLES = 1024, CS_GUARDA = 4 };
    static int32_t impulse_response[NSAMPLES] = {INT16_MAX << CS_GUARDA};
    static struct {
        struct {
            int32_t px1[2];
            int32_t px2[2];
            int32_t pz1[1];
            int32_t pz2[1];
            int32_t cmb1[16];
            int32_t cmb2[16];
        } mem;
        struct {
            uint8_t px1[1];
            uint8_t px2[1];
            uint8_t cmb1[1];
            uint8_t cmb2[1];
        } ph;
    } sta;
    for (int i = 0; i < NSAMPLES; ++i) {
#if 1 // step
        int32_t aux = 1000 << CS_GUARDA;
#else
        int32_t aux = impulse_response[i];
#endif
        aux                 = pole_24r25_3f49(aux, &sta.mem.px1, sta.ph.px1);
        aux                 = pole_24r25_3f49(aux, &sta.mem.px2, sta.ph.px2);
        aux                 = pole_1r_0f(aux, sta.mem.pz1);
        aux                 = pole_1r_0f(aux, sta.mem.pz2);
        aux                 = comb_14d(aux, &sta.mem.cmb1, sta.ph.cmb1);
        aux                 = comb_14d(aux, &sta.mem.cmb2, sta.ph.cmb2);
        impulse_response[i] = (aux >> CS_GUARDA) >> 13;
    }
    for (int i = 0; i < NSAMPLES; ++i) {
        printf("%d\n", impulse_response[i]);
    }
    return 0;
}

