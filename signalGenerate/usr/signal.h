#ifndef __SIGNAL_H__
#define __SIGNAL_H__

// #include "stm32f4xx_hal.h"
#include "main.h"

void setVoltage(float voltage);
void rectangleWave(float voltage, float frequency);
void Frequency_square_wave(uint16_t maxval, uint16_t frequency);
void Frequency_triangle_wave(uint16_t maxval, uint16_t frequency, uint32_t bandpass);
void Frequency_triangle_wave2(uint16_t maxval, uint16_t frequency, float rate);

#endif // !__SIGNAL_H__
