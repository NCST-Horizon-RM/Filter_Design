#ifndef __SIGNAL_H__
#define __SIGNAL_H__

// #include "stm32f4xx_hal.h"
#include "main.h"

#define PI 3.14159265358979323846f

void setVoltage(float voltage);
void rectangleWave(float voltage, float frequency);
void Frequency_square_wave(uint16_t maxval, float frequency);
void Frequency_triangle_wave(uint16_t maxval, float frequency, uint32_t bandpass);
void Frequency_triangle_wave2(uint16_t maxval, float frequency, float rate);
void Frequency_sin_wave(uint16_t maxval, float frequency, uint16_t samples);
void Frequency_triangle_wave3(uint16_t maxval, float frequency);
void Frequency_noise_wave(uint16_t maxval);
void Frequency_sinNoise_wave(uint16_t maxval, float frequency, uint16_t samples, uint16_t noise_amplitude, float noise_frequency);

#endif // !__SIGNAL_H__
