//
// Created by peca on 23-Apr-26.
//

#ifndef ETVDAQ_BOARDLED_H
#define ETVDAQ_BOARDLED_H
#include <stdint.h>

void ledOn();
void ledOnmS(uint32_t ticksOn);

void ledOff();
void ledOffmS(uint32_t ticksOn);

void ledToggle();
void ledTogglemS(uint32_t ticksOn, uint32_t ticksOff);

void ledBlur(uint32_t ticksDuration, uint32_t ticksOn, uint32_t ticksOff);

#endif //ETVDAQ_BOARDLED_H