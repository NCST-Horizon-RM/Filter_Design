#include "VOFA.h"
#include "main.h"
#include "usart.h"

data_t data;
/**
 * @brief VOFA+ʹ�ô��ڷ���justfloat��ʽ����
 *
 * @param a
 * @param b
 * @param c
 * @param d
 * @param e
 * @param f
 * @param g
 * @param h
 * @param j
 * @param k
 */
void VOFA_justfloat(float a, float b, float c, float d, float e, float f, float g, float h, float j, float k)
{
    uint8_t i = 0;
    data.data1[i++] = a;
    data.data1[i++] = b;
    data.data1[i++] = c;
    data.data1[i++] = d;
    data.data1[i++] = e;
    data.data1[i++] = f;
    data.data1[i++] = g;
    data.data1[i++] = h;
    data.data1[i++] = j;
    data.data1[i++] = k;
    data.data2[40] = 0x00;
    data.data2[41] = 0x00;
    data.data2[42] = 0x80;
    data.data2[43] = 0x7f;
   HAL_UART_Transmit_DMA(&huart2, data.data2, sizeof(data.data2));
}
