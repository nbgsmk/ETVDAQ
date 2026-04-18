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

static const int32_t AD_THRESHOLD__DEBUG = 1000;			// pozitivna vrednost -> alarmiram kad je izmereno VECE od praga TODO treba biti smislena konfigurabilna vrednost
static const uint32_t AD_HYSTERSIS__DEBUG = 100;					// TODO treba biti smislena konfigurabilna vrednost
static const uint32_t ADC_REPETITION_PERIOD_mS = 1000U * 2U;	// period izmedju dva ADC merenja
static const uint32_t structTimeout = 10 * 1000U;				// (mS) max koliko cekam mutex za upis rezultata u merenjaStruct

osStatus_t setDigitalResult(uint32_t arg);
uint32_t getDigitalResult();

osStatus_t setAnalogResult(uint8_t position, uint32_t arg);

#endif //ETVDAQ_MERENJA_H