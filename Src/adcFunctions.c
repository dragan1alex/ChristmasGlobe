
#include "main.h"
#include "cmsis_os.h"
#include "globalVariables.h"
#include "mainOperatingFunctions.h"
#include "tasks.h"
#include "clock.h"

/* ADC conversion complete flag */
volatile uint8_t convCpt = 0;

/* Raw ADC variables */
static uint32_t adc[5];
static uint32_t VCCmV, currentuA;
static float currentmA, vccRes,vcc,current;
static uint32_t ran[5] = {ADC_CHANNEL_0, ADC_CHANNEL_6, ADC_CHANNEL_7, ADC_CHANNEL_VREFINT, ADC_CHANNEL_TEMPSENSOR};
uint8_t debugRunCurrent = 0;

/* Delay between ADC data acquisition */
uint16_t adcDelay = 500;

/* Derivates from ADC values */
float currentAvg, tca;
uint8_t acc=0;
uint8_t isRunning = 1;
uint16_t lightThreshold = 200;

void config_ext_channel_ADC(uint32_t channel, uint8_t val)
{
  ADC_ChannelConfTypeDef sConfig;

  sConfig.Channel = channel;

  if(1 == val)
  {
    sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  }
  else
  {
    sConfig.Rank = ADC_RANK_NONE;
  }

  HAL_ADC_ConfigChannel(&hadc, &sConfig);
}

uint32_t r_single_ext_channel_ADC(uint32_t channel)
{
  uint32_t digital_result;

  config_ext_channel_ADC(channel, 1);

  HAL_ADCEx_Calibration_Start(&hadc, ADC_SINGLE_ENDED);

  HAL_ADC_Start(&hadc);
  HAL_ADC_PollForConversion(&hadc, 1000);
  digital_result = HAL_ADC_GetValue(&hadc);
  HAL_ADC_Stop(&hadc);

  config_ext_channel_ADC(channel, 0);

  return digital_result;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	convCpt = 1;
	return;
}

void getADC()
{
	while(1)
	{
		enableSensors();
		vTaskDelay(3);
		//enter time-critical block of code, disable the RTOS and run bare-metal
		vTaskSuspendAll();
		if(debugRunCurrent == 1)
			setAll(0);
		for(int i=0;i<5;i++)
		{
			r_single_ext_channel_ADC(ran[i]); //to mitigate the "adc being stupid" silicon bug; read the stupid errata
			adc[i] = r_single_ext_channel_ADC(ran[i]);
		}
		//adc conversions done, re-enter RTOS
		xTaskResumeAll();
		disableSensors();
		//do something with the collected data
		vTaskResume(xTaskGetHandle("calc"));
		//sleep
		vTaskSuspend(null);
	}
}

void checkADC()
{
	while(1)
	{
		vTaskResume(xTaskGetHandle("adc"));
		vTaskDelay(adcDelay);
	}
}

void calcValues()
{
	while(1)
	{
		VCCmV = 1165 * VREFINT_CAL / adc[4];
		vcc = VCCmV / 1000.0;
		vccRes = vcc * (float)(4096 - adc[2])/4095.0;\
		current = vccRes / 10;
		currentmA = current * 1000;
		currentuA = currentmA * 1000;
		tca += currentuA;
		acc ++;
		uint8_t maxSamples = 20;
		if(acc >= maxSamples)
		{
			acc = 0;
			currentAvg = tca / maxSamples;
			tca = 0;
		}
		if(vcc < 2.8)
		{
			f1 = 10;
		}
		brightnessMultiplier = adc[1]* 0.00041514 + 0.3;
		//check the ambient light level
		if(adc[0] > (lightThreshold * 12/ 10) && isRunning == 1)
		{ //light
			isRunning= 0;
			suspendModes();
			setAll(0);
			ChangeSystemClock(2);
			adcDelay = 4000;
		}
		else if(adc[0] <= lightThreshold && isRunning == 0)
		{
			isRunning = 1;
			checkModes();
			adcDelay = 500;
		}
		else if(isRunning == 0)
		{
			statusLed(1);
			vTaskDelay(75);
			statusLed(0);
		}
		vTaskSuspend(null);
	}
}
