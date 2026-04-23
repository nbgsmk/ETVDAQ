//
// Created by peca on 23-Apr-26.
//

#include "BoardLed.h"

#include "cmsis_os2.h"
#include "main.h"


void ledOn() {
	HAL_GPIO_WritePin(BOARD_LED0_GPIO_Port, BOARD_LED0_Pin, GPIO_PIN_RESET);
}

void ledOnmS(uint32_t ticksOn) {
	HAL_GPIO_WritePin(BOARD_LED0_GPIO_Port, BOARD_LED0_Pin, GPIO_PIN_RESET);
	osDelay(ticksOn);
}

void ledOff() {
	HAL_GPIO_WritePin(BOARD_LED0_GPIO_Port, BOARD_LED0_Pin, GPIO_PIN_SET);
}

void ledOffmS(uint32_t ticksOn) {
	HAL_GPIO_WritePin(BOARD_LED0_GPIO_Port, BOARD_LED0_Pin, GPIO_PIN_SET);
	osDelay(ticksOn);
}

void ledToggle() {
	HAL_GPIO_TogglePin(BOARD_LED0_GPIO_Port, BOARD_LED0_Pin);
}
void ledTogglemS(uint32_t ticksOn, uint32_t ticksOff) {
	HAL_GPIO_TogglePin(BOARD_LED0_GPIO_Port, BOARD_LED0_Pin);
	osDelay(ticksOn);
	HAL_GPIO_TogglePin(BOARD_LED0_GPIO_Port, BOARD_LED0_Pin);
	osDelay(ticksOff);
}

void ledBlur(uint32_t ticksDuration, uint32_t ticksOn, uint32_t ticksOff) {
	uint32_t dur = ticksDuration / (ticksOn + ticksOff);
	for (int i = 0; i < dur; ++i) {
		ledOnmS(ticksOn);
		ledOffmS(ticksOff);
	}
}
