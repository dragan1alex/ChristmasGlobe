#include "main.h"

#define null NULL
#define VREFINT_CAL *((uint16_t*)0x1FF80078)

float brightnessMultiplier;
extern ADC_HandleTypeDef hadc;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim21;
extern TIM_HandleTypeDef htim22;
uint32_t f1, f2;   // f1/f2
