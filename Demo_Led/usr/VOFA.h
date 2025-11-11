#ifndef __VOFA_H_
#define __VOFA_H_

#include "main.h"

typedef union
{
    float data1[10];
    uint8_t data2[44];
} data_t;

extern data_t data;

void VOFA_justfloat(float a, float b, float c, float d, float e, float f, float g, float h, float j, float k);

#endif