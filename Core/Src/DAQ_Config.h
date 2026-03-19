//
// Created by peca on 19-Mar-26.
//

#ifndef ETVDAQ_DAQ_CONFIG_H
#define ETVDAQ_DAQ_CONFIG_H
#include <stdint.h>

uint32_t cfg_GetHw_HbPer(void);
void cfg_SetHw_HbPer(uint32_t period_mS);

#endif //ETVDAQ_DAQ_CONFIG_H