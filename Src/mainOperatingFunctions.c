
#include "globalVariables.h"

void statusLed(uint8_t status){
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, status);
}

uint32_t f1=9, f2=10;   // f1/f2
uint8_t led[6];

void setLed(uint8_t n, uint32_t val){
	led[n-1] = val;
	switch(n){
	case 1:
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, val);
		break;
	case 2:
		if(val*f1/f2 < 2 && val > 0){
			__HAL_TIM_SET_COMPARE(&htim21, TIM_CHANNEL_2, 2);
			return;break;
		}
		__HAL_TIM_SET_COMPARE(&htim21, TIM_CHANNEL_2, val*f1/f2);
		break;
	case 3:
		__HAL_TIM_SET_COMPARE(&htim21, TIM_CHANNEL_1, val);
		break;
	case 4:
		__HAL_TIM_SET_COMPARE(&htim22, TIM_CHANNEL_2, val);
		break;
	case 5:
		if(val*f1/f2 < 2 && val > 0 ){
			__HAL_TIM_SET_COMPARE(&htim22, TIM_CHANNEL_1, 2);
			return;break;
		}
		__HAL_TIM_SET_COMPARE(&htim22, TIM_CHANNEL_1, val*f1/f2);
		break;
	case 6:
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, val);
		break;
	default: return;break;
	}
	return;
}

uint32_t getLed(uint8_t nr){
//	switch(nr){
//		case 1:
//			return __HAL_TIM_GET_COMPARE(&htim2, TIM_CHANNEL_1);
//			break;
//		case 2:
//			return __HAL_TIM_GET_COMPARE(&htim21, TIM_CHANNEL_2)*f2/f1;
//			break;
//		case 3:
//			return __HAL_TIM_GET_COMPARE(&htim21, TIM_CHANNEL_1);
//			break;
//		case 4:
//			return __HAL_TIM_GET_COMPARE(&htim22, TIM_CHANNEL_2);
//			break;
//		case 5:
//			return __HAL_TIM_GET_COMPARE(&htim22, TIM_CHANNEL_1)*f2/f1;
//			break;
//		case 6:
//			return __HAL_TIM_GET_COMPARE(&htim2, TIM_CHANNEL_2);
//			break;
//		default: return 0;break;
//		}
		return led[nr-1];
}

void setAll(uint16_t value){
	for(uint8_t i=1;i<=6;i++)
		setLed(i, value);
}


void pauseLeds(){
	for(uint8_t i=0;i<=5;i++){
		led[i] = getLed(i+1);
	}
	setAll(0);
	return;
}

void resumeLeds(){
	for(uint8_t i=0;i<=5;i++){
		setLed(i+1, led[i]);
	}
	return;
}


void initLeds(){
	uint8_t i;
	for( i=1;i<=6;i+=1)
		setLed(i, 200);
}

void enableSensors(){
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, 1);
}

void disableSensors(){
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 0);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, 0);
}




void decreaseLed(uint8_t n, uint8_t minVal){
	uint8_t currentValue = getLed(n);
	if((n < 7) && (currentValue > minVal)){
		setLed(n, currentValue - 1);
	}
	return;
}

void decreaseAll(uint8_t minVal){
	for(uint8_t i=1;i<=6;i++){
		decreaseLed(i, minVal);
	}
	return;
}

void enableTimers(){
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim21, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim21, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim22, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim22, TIM_CHANNEL_2);
}
