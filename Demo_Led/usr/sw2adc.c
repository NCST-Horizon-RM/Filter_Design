#include "main.h"
#include "sw2adc.h"
#include "VOFA.h"
#include "dwt.h"
#include "adc.h"
#include "usart.h"
#include "iir.h"

uint16_t SW2ADC_Value = 0;
float adc_voltage[500] = {0};
int index = 0;

float dt_pc = 0, dt_vofa = 0;
uint32_t INS_DWT_Count = 0;

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if (hadc->Instance == ADC1)
    {
        // SW2ADC_Value = HAL_ADC_GetValue(hadc);
        // data.data1[0] = (float)SW2ADC_Value/4095.0f*3.3f;
        // if (index >= 500) index = 0;
        // adc_voltage[index++] = data.data1[0];
        // dt_pc = DWT_GetDeltaT(&INS_DWT_Count);
        // INS_DWT_Count = DWT->CYCCNT;
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM1)
    {
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_12);  // Toggle LED1
    }
    else if (htim->Instance == TIM2)
    {
        getVoltage();
        printFilteredValue(SW2ADC_Value);

        // printValue(SW2ADC_Value, &huart2);
        // HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_13);  // Toggle LED2
    }
}

void printValue(uint16_t value, UART_HandleTypeDef *huart)
{
    // data.data1[0] = (float)value/4095.0f*3.3f; 
    // static uint32_t last_print_time = 0;
    VOFA_justfloat(data.data1[0], dt_pc, dt_vofa, 0, 0, 0, 0, 0, 0, 0);
    // dt_vofa = DWT_GetDeltaT(&last_print_time); 
    // last_print_time = DWT->CYCCNT;
}

void getVoltage(void)
{
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&SW2ADC_Value, 1);
    data.data1[0] = (float)SW2ADC_Value/4095.0f*3.3f;
    if (index >= 500) index = 0;
    adc_voltage[index++] = data.data1[0];
    dt_pc = DWT_GetDeltaT(&INS_DWT_Count);
    INS_DWT_Count = DWT->CYCCNT;
}


void printFilteredValue(uint16_t value)
{
    double filtered_value = butterOrdF((float)value/4095.0f*3.3f);
    VOFA_justfloat((float)filtered_value, data.data1[0], dt_vofa, 0, 0, 0, 0, 0, 0, 0);
}