/*
 * adcFunctions.h
 *
 *  Created on: Dec 12, 2019
 *      Author: Andrei
 */

#ifndef ADCFUNCTIONS_H_
#define ADCFUNCTIONS_H_

/* ADC conversion complete flag */
volatile uint8_t convCpt;
/* Raw ADC variables */
uint32_t adc[5];
uint32_t VCCmV, currentuA;
float currentmA, vccRes,vcc,current;
uint32_t ran[5];
uint8_t debugRunCurrent;
/* Delay between ADC data acquisition */
uint16_t adcDelay;
/* Derivates from ADC values */
float currentAvg, tca;
uint8_t acc;
uint8_t isRunning;
uint16_t lightThreshold;

void config_ext_channel_ADC(uint32_t channel, uint8_t val);
uint32_t r_single_ext_channel_ADC(uint32_t channel);
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);
void getADC();
void checkADC();
void calcValues();


#endif /* ADCFUNCTIONS_H_ */
