//
// Created by peca on 19-Mar-26.
//

#include <stdint.h>
#include "cmsis_os2.h"

#include "Merenja.h"
#include "Kalendar.h"

osMutexId_t mutexMerenjaHandle;
const osMutexAttr_t mutexMerenja_attributes = {
	.name = "mutexMerenja"
};

CurMerenja_t curMerenja;


CurMerenja_t get() {
	return curMerenja;
}
osStatus_t setDigitalResult(uint32_t arg) {
	osStatus_t rez = osMutexAcquire(mutexMerenjaHandle, structTimeout);
	if (rez == osOK) {
		curMerenja.timeStamp = getTimestamp();
		curMerenja.digitalIO = arg;
		return osOK;
	} else {
		return rez;
	}
}

uint32_t getDigitalResult() {
	return curMerenja.digitalIO;
}

osStatus_t setAnalogResult(uint8_t position, uint32_t arg) {
	osStatus_t rez = osMutexAcquire(mutexMerenjaHandle, structTimeout);
	if (rez == osOK) {
		curMerenja.timeStamp = getTimestamp();
		curMerenja.analogIO[position] = arg;
		return osOK;
	} else {
		return rez;
	}
}


