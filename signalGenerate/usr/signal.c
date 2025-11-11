#include "signal.h"
#include "main.h"
#include "tim.h"
#include "dac.h"
#include "stm32f4xx_hal_dac.h"
#include "dwt.h"

uint32_t dac_value = 0;
/* 可调电压 */
void setVoltage(float voltage)
{
    dac_value = (uint32_t)((voltage / 3.3f) * 4095);
    HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dac_value);
}


/**
 * @brief  产生频率方波
 * @note   方波频率存在局限性！
 * @param  maxval    最大值 (0 < maxval < 4095)
 * @param  frequency 方波的频率
 * @retval 无
 */
void Frequency_square_wave(uint16_t maxval, uint16_t frequency)
{
    HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, maxval);
    DWT_Delay_us(1000000 / (1 * frequency));  // 半周期延时 ,好像这个延时有问题，理论应该是500000 / frequency
    HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 0);
    DWT_Delay_us(1000000 / (1 * frequency));
}

// 梯形波
/**
 * @brief  产生频率梯形波
 * @note   梯形波频率存在局限性！
 * @param  maxval    最大值 (0 < maxval < 4095)
 * @param  frequency 梯形波的频率
 * @param  bandpass  上升/下降沿的采样点数
 * @retval 时间并不精确
 */
void Frequency_triangle_wave(uint16_t maxval, uint16_t frequency, uint32_t bandpass)
{
    uint32_t step = maxval / bandpass;
    // 上升沿
    for (int i = 0; i < bandpass; i++)
    {
        HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, i * step);
        DWT_Delay_us(500);
    }
    // 平顶
    HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, maxval);
    DWT_Delay_us(1000000 / (1 * frequency));
    // 下降沿
    for (int i = bandpass; i > 0; i--)
    {
        HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, i * step);
        DWT_Delay_us(500);
    }
    DWT_Delay_us(1000000 / (1 * frequency));

}

/**
 * @brief  产生频率梯形波2
 * @note   梯形波频率存在局限性！
 * @param  maxval    最大值 (0 < maxval < 4095)
 * @param  frequency 梯形波的频率
 * @param  bandpass  单边梯度持续比例
 * @retval 无
 */
void Frequency_triangle_wave2(uint16_t maxval, uint16_t frequency, float rate)
{
    // uint32_t step = maxval / (bandpass * rate);
    float bandpass_time = 1.0f / frequency * rate * 1000000; // us
    float step_time = bandpass_time / (maxval);              // 每步持续时间 us
    float con_time = (1.0f / frequency * (1 - 2 * rate)) * 1000000; // us

    // 上升沿
    for (int i = 0; i < maxval; i++)
    {
        HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, i);
        DWT_Delay_us((uint32_t)step_time);
    }
    // 平顶
    HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, maxval);
    DWT_Delay_us((uint32_t)con_time);
    // 下降沿
    for (int i = maxval; i > 0; i--)
    {
        HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, i);
        DWT_Delay_us((uint32_t)step_time);
    }
    DWT_Delay_us((uint32_t)con_time);

}
