//
// Created by peca on 19-Mar-26.
//

#include "DAQ_Config.h"
#include <stdint.h>

uint32_t HW_HeartBeat_per_mS = 5 * 1000U;		// hardware heartbeat za led diodu

uint32_t cfg_GetHw_HeartbeatPer() {
	return HW_HeartBeat_per_mS;
}

void cfg_SetHw_HbPer(uint32_t period_mS) {
	HW_HeartBeat_per_mS = period_mS;
	// TODO write to flash
}