#ifndef _CORE_SENSOR_H
#define _CORE_SENSOR_H
#include "stdint.h"

/****Macro definition list ****/
//Capacitive current limitation
#define I_CAP_MAX_HI 10000
#define I_CAP_MAX_LO 9800
#define I_CAP_MAX    9700

//Low voltage overvoltage protection for capacitors
#define V_CAP_MAX     22000
#define V_CAP_MIN 		7000

//Determine whether to enter the protection condition
//#define PROTECT_CONDITION ((V_IN > 26000 || V_IN<17000) || (V_REF > 14000))
#define PROTECT_CONDITION ((V_IN<17000) || (V_REF > 14000))
//Exit time of the protected state, unit: 1e-4 s
#define  PROTECT_EXIT_TIME 1000

//TIMER FREQ
#define TIM7_FREQ 10000UL
#define TIM1_FREQ 100000UL
#define TIM2_FREQ 100UL


extern int32_t I_IN;
extern int32_t I_OUT;
extern int32_t I_CAP;
extern int32_t V_IN;
extern int32_t V_CAP;
extern int32_t V_HI;
extern int32_t V_LO;
extern int32_t V_REF;

extern int32_t P_real;
extern int32_t P_cap_real;
extern int32_t P_Target;
extern int32_t VLO_target;
extern int32_t pulse_width_mul8;
extern int32_t energy_1s_in_100nJ;

struct BadPWRContext
{
	int cnt_VIN_REVERSE_CURRENT;
	//NOT USEED
	int cnt_VOUT_OVERPOWER ;
	int cnt_I_CAP_PROTECT;
	int cnt_VIN_STAY_CURRENT;
};

typedef enum CAP_STATE_t
{
	CAP_INIT     = 0,
	CAP_VCAP     = 1,
	CAP_ICAP_IN  = 2,
	CAP_ICAP_OUT = 4,
	CAP_PIN      = 8,
	CAP_PROTECT  = 16,
	CAP_I_WORK_PROTECT  = 32,
}CAP_STATE_t;

#define ABS(x) ((x)<0?-(x):(x))
#define VAL_LIMIT(x,min,max) ((x)<(min)?(min):(x)>(max)?(max):(x))

#endif//_CORE_SENSOR_H
