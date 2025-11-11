#ifndef __SW2ADC_H
#define __SW2ADC_H

#include "main.h"
extern uint16_t SW2ADC_Value;
extern float adc_voltage[500] ;

void getVoltage(void);
void printValue(uint16_t value, UART_HandleTypeDef *huart);
void printFilteredValue(uint16_t value);

#endif // !__SW2ADC_H