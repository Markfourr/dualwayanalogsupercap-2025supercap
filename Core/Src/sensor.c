#include "sensor.h"

int32_t I_IN;
int32_t I_OUT;
int32_t I_CAP;
int32_t V_IN;
int32_t V_CAP;
int32_t V_HI;
int32_t V_LO;
int32_t V_REF;



//P_real IS (V_IN * I_IN)  >> 10;
int32_t P_real = 0;
int32_t P_cap_real = 0;
int32_t P_Target = 50000;
int32_t VLO_target = 15000;
int32_t pulse_width_mul8 = 840<<3;

int32_t energy_1s_in_100nJ;
