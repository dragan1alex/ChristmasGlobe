/*
 * mainOperatingFunctions.h
 *
 *  Created on: Dec 12, 2019
 *      Author: Andrei
 */

#ifndef MAINOPERATINGFUNCTIONS_H_
#define MAINOPERATINGFUNCTIONS_H_

void statusLed(uint8_t status);
void setLed(uint8_t n, uint32_t val);
uint32_t getLed(uint8_t nr);
void setAll(uint16_t value);
void pauseLeds();
void resumeLeds();
void initLeds();
void enableSensors();
void disableSensors();
void decreaseLed(uint8_t n, uint8_t minVal);
void decreaseAll(uint8_t minVal);
void enableTimers();

#endif /* MAINOPERATINGFUNCTIONS_H_ */
