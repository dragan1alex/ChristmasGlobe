/*
 * tasks.h
 *
 *  Created on: Dec 12, 2019
 *      Author: Andrei
 */

#include "cmsis_os.h"
#include "main.h"
#include "clock.h"
#include "globalVariables.h"
#include "mainOperatingFunctions.h"
#include "stdlib.h"

uint8_t mode = 4, maxModes = 5;
int fadeDelay = 2;
uint8_t minPWM = 0;

 void fadeLedsOut(){
	for(;;){
		decreaseAll(minPWM);
		vTaskDelay((int)(fadeDelay/brightnessMultiplier));
	}

}

 void suspendModes(){
	 vTaskSuspend(xTaskGetHandle("fadeLedsOut"));
	 vTaskSuspend(xTaskGetHandle("rotate"));
	 vTaskSuspend(xTaskGetHandle("breathe"));
	 vTaskSuspend(xTaskGetHandle("singleColors"));
	 vTaskSuspend(xTaskGetHandle("loop"));
	 vTaskSuspend(xTaskGetHandle("randomFadeOut"));

}

 void suspendAll(){
	 vTaskSuspend(xTaskGetHandle("chkBtn"));
	 vTaskSuspend(xTaskGetHandle("adc"));
	 vTaskSuspend(xTaskGetHandle("calc"));
	 suspendModes();
 }

 uint32_t convDelay(uint32_t d){
	 return (uint32_t)( (float)d/(float)brightnessMultiplier);
 }

 void rotate(){
	 while(1){
		 for(uint8_t i=1;i<=6;i++)
		 {
			 for(uint8_t j=1;j<=50;j++)
			 {
				 setLed(i, 2*(uint8_t)(j*brightnessMultiplier));
				 vTaskDelay(convDelay(1));
			 }
			 if(i%3==0)
				 vTaskDelay(55);
		 }
	 }

 }

 void breathe()
 {
	 while(1)
	 {
		 while(getLed(1) > 1) vTaskDelay(10);
		 vTaskDelay(1000);
		 //vTaskSuspend(xTaskGetHandle("fadeLedsOut"));
		 for(uint8_t i=1;i<=50;i++)
		 {
			 setLed(3, (uint8_t)(i*2*brightnessMultiplier));
			 setLed(4, (uint8_t)(i*2*brightnessMultiplier));
			 vTaskDelay(convDelay(2));
		 }
		 for(uint8_t i=1;i<=50;i++)
		 {
			 setLed(2, (uint8_t)(i*2*brightnessMultiplier));
			 setLed(5, (uint8_t)(i*2*brightnessMultiplier));
			 vTaskDelay(convDelay(2));
		 }
		 for(uint8_t i=1;i<=50;i++)
		 {
			 setLed(1, (uint8_t)(i*2*brightnessMultiplier));
			 setLed(6, (uint8_t)(i*2*brightnessMultiplier));
			 vTaskDelay(convDelay(2));
		 }
		 vTaskResume(xTaskGetHandle("fadeLedsOut"));
	 }
 }

 void singleColors()
 {
	 while(1)
	 {
		 while(getLed(3) > (20*brightnessMultiplier)) vTaskDelay(1);
		 for(uint8_t i=1;i<=33;i++)
		 {
			 setLed(1, i*3*brightnessMultiplier);
			 setLed(4, i*3*brightnessMultiplier);
			 vTaskDelay(convDelay(2));
		 }
		 while(getLed(1) > (20*brightnessMultiplier)) vTaskDelay(1);
		 for(uint8_t i=1;i<=33;i++)
		 {
			 setLed(2, i*3*brightnessMultiplier);
			 setLed(5, i*3*brightnessMultiplier);
			 vTaskDelay(convDelay(2));
		 }
		 while(getLed(2) > (20*brightnessMultiplier)) vTaskDelay(1);
		 for(uint8_t i=1;i<=33;i++)
		 {
			 setLed(3, i*3*brightnessMultiplier);
			 setLed(6, i*3*brightnessMultiplier);
			 vTaskDelay(convDelay(2));
		 }
	 }
 }

 void loop()
 {
	 while(1)
	 {
		 for(uint8_t i=0;i<=5;i++)
		 {
			 for(uint8_t j=0;j<=5;j++)
			 {
				 for(uint8_t k=1;k<=50;k++)
				 {
					 setLed((i+j) % 6 + 1, 2 * k * brightnessMultiplier);
					 vTaskDelay(1);
				 }
			 }
			 vTaskDelay(50);
			 for(uint8_t j=1;j<=6;j++)
			 {
				 for(uint8_t k=50;k>=1;k--)
				 {
					 setLed((i+j) % 6 + 1, 2 * k * brightnessMultiplier);
					 vTaskDelay(2);
				 }
			 }
			 vTaskDelay(500);
		 }
	 }
 }

 void randomFadeOut()
 {
	 static uint8_t currentLed = 0;
	 static uint8_t randPWM = 0;
	 static uint8_t currentDelay = 100;
	 static int currentRandNumber;
	 randomSeed = adc[0] + adc[1] + adc[2] + adc[3];
	 static uint8_t prevLed[2];
	 while(1)
	 {
		 prevLed[1] = prevLed[0];
		 prevLed[0] = currentLed;
		 while(currentLed == prevLed[0] || currentLed == prevLed[1])
		 {
			 currentRandNumber = rand_r(&randomSeed);
			 currentLed = currentRandNumber % 6 + 1;
		 }
		 randPWM = currentRandNumber % 21 + 80;
		 currentDelay = currentRandNumber % 120 + 50;
		 setLed(currentLed, randPWM * brightnessMultiplier);
		 vTaskDelay(currentDelay * 10);
	 }
 }

 void checkModes()
 {
	 suspendModes();
	 switch(mode)
	 {
	 case 0:
		 ChangeSystemClock(2);
		 minPWM = 1;
		 vTaskResume(xTaskGetHandle("fadeLedsOut"));
		 vTaskResume(xTaskGetHandle("rotate"));
		 fadeDelay = 3;
		 break;
	 case 1:
		 ChangeSystemClock(2);
		 minPWM = 1;
		 vTaskResume(xTaskGetHandle("fadeLedsOut"));
		 vTaskResume(xTaskGetHandle("breathe"));
		 fadeDelay = 14;
		 break;
	 case 2:
		 ChangeSystemClock(2);
		 minPWM = 1;
		 vTaskResume(xTaskGetHandle("fadeLedsOut"));
		 vTaskResume(xTaskGetHandle("singleColors"));
		 fadeDelay = 6;
		 break;
	 case 3:
		 ChangeSystemClock(2);
		 vTaskResume(xTaskGetHandle("loop"));
		 break;
	 case 4:
		 ChangeSystemClock(2);
		 minPWM = 0;
		 vTaskResume(xTaskGetHandle("randomFadeOut"));
		 vTaskResume(xTaskGetHandle("fadeLedsOut"));
		 fadeDelay = 40;
		 break;
	 default: ChangeSystemClock(2);vTaskResume(xTaskGetHandle("rotate"));break;
	 }
 }

 void checkButton()
 {
	 while(1)
	 {
		 if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8) == GPIO_PIN_RESET)
		 {
			 mode++;
			 mode %= maxModes;
			 checkModes();
			 while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8) == GPIO_PIN_RESET)vTaskDelay(10); //wait for the button to be depressed
		 }
		 vTaskDelay(100);
	 }
 }
