//
// Created by peca on 19-Mar-26.
//

#include "Kalendar.h"

#include "cmsis_os2.h"

uint32_t getTimestamp() {
	return osKernelGetTickCount();		// TODO koristiti RTC
}
