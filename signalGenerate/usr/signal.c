#include "signal.h"
#include "main.h"
#include "tim.h"
#include "dac.h"
#include "stm32f4xx_hal_dac.h"
#include "dwt.h"
#include "math.h"
#include "stdlib.h"

/**
 * @brief  设置DAC输出电压
 * @param  voltage 期望输出电压 (0.0f ~ 3.3f)
 * @retval 实际好像有0.1V的误差
 */
void setVoltage(float voltage)
{
    uint16_t dac_value = (uint32_t)((voltage / 3.3f) * 4095);
    HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dac_value);
}


/**
 * @brief  产生频率方波
 * @note   方波频率存在局限性！
 * @param  maxval    最大值 (0 < maxval < 4095)
 * @param  frequency 方波的频率
 * @retval 无
 */
void Frequency_square_wave(uint16_t maxval, float frequency)
{
    HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, maxval);
    DWT_Delay_us(1000000 / (1 * frequency));  // 半周期延时 ,好像这个延时有问题，理论应该是500000 / frequency
    HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 0);
    DWT_Delay_us(1000000 / (1 * frequency));
}


/**
 * @brief  产生频率梯形波
 * @note   梯形波频率存在局限性！
 * @param  maxval    最大值 (0 < maxval < 4095)
 * @param  frequency 梯形波的频率
 * @param  bandpass  上升/下降沿的采样点数
 * @retval 时间并不精确
 */
void Frequency_triangle_wave(uint16_t maxval, float frequency, uint32_t bandpass)
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
void Frequency_triangle_wave2(uint16_t maxval, float frequency, float rate)
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


/**
 * @brief  产生频率正弦波
 * @note   正弦波频率存在局限性！
 * @param  maxval    最大值 (0 < maxval < 4095)
 * @param  frequency 正弦波的频率
 * @param  samples   一个周期的采样点数
 * @retval 无
 */
void Frequency_sin_wave(uint16_t maxval, float frequency, uint16_t samples)
{
    uint16_t time = 1.0f / frequency * 1000000;
    float delta = 2 * PI / samples;
    float delta_time = time / samples;
    for (int i = 0; i < samples; i++)
    {
        HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, maxval * sin(delta * i) + maxval);
        DWT_Delay_us((uint32_t)delta_time);
    }
}

/**
 * @brief  产生频率三角波
 * @note   三角波频率存在局限性！
 * @param  maxval    最大值 (0 < maxval < 4095)
 * @param  frequency 三角波的频率
 * @param  samples   一个周期的采样点数
 * @retval 无
 */
void Frequency_triangle_wave3(uint16_t maxval, float frequency)
{
    Frequency_triangle_wave2(maxval, frequency, 0.5f);
}

void Frequency_noise_wave(uint16_t maxval)
{
    int val = 0;
    val = rand() % maxval;
    HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, val);
}

/**
 * @brief  产生频率正弦噪声波
 * 
 * @param maxval    最大值 (0 < maxval < 4095)
 * @param frequency 正弦噪声波的频率
 * @param samples   一个周期的采样点数
 * @param noise_amplitude 噪声波的振幅（建议10分之一主振幅）
 * @param noise_frequency 噪声波相对于主波频率的倍数
 * @retval 杂波为100倍主频率
 */
void Frequency_sinNoise_wave(uint16_t maxval, float frequency, uint16_t samples, uint16_t noise_amplitude, float noise_frequency)
{
    uint16_t time = 1.0f / frequency * 1000000;
    float delta = 2 * PI / samples;
    float delta_time = time / samples;
    for (int i = 0; i < samples; i++)
    {
        HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, maxval * sin(delta * i) + maxval + noise_amplitude * sin(delta * noise_frequency * i));
        DWT_Delay_us((uint32_t)delta_time);
    }
}