//
// Created by peca on 19-Mar-26.
//

#ifndef ETVDAQ_MERENJA_H
#define ETVDAQ_MERENJA_H

typedef struct {
	uint32_t timeStamp;
	uint32_t digitalIO;
	uint32_t analogIO[16];							// TODO nikad nece biti 16 kanala. ispraviti da bude hadc1.Init.NbrOfConversion
} CurMerenja_t;

static const uint32_t ADC_REPETITION_PERIOD_mS = 20 * 1000U;		// period izmedju dva ADC merenja
static const uint32_t structTimeout = 10 * 1000U;				// (mS) max koliko cekam mutex za upis rezultata u merenjaStruct

osStatus_t setDigital(uint32_t arg);
osStatus_t setAnalog(uint8_t position, uint32_t arg);

#endif //ETVDAQ_MERENJA_H