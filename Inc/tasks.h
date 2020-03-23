#ifndef TASKS_H_
#define TASKS_H_

extern int fadeDelay;
extern uint8_t mode, maxModes;

void decreaseBrightness();
void suspendModes();
void suspendAll();
uint32_t convDelay(uint32_t d);
void rotate();
void breathe();
void singleColors();
void loop();
void checkModes();
void checkButton();

#endif
