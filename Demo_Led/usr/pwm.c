#include "main.h"
#include "pwm.h"
#include "main.h"
#include "tim.h"

void pwmGenerate(uint16_t pwmValue)
{
    static uint16_t pwmVal=0;   //PWM占锟秸憋拷  
    uint8_t dir=1;   
    while (pwmVal< 100)
	  {
		  pwmVal++;
		  __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 500);    //锟睫改比斤拷值锟斤拷锟睫革拷占锟秸憋拷
		  // TIM2->CCR2 = pwmVal;    锟斤拷锟较凤拷锟斤拷同
		  HAL_Delay(1);
	  }
    pwmVal = 0;
// 	  while (pwmVal)
// 	  {
// 		  pwmVal--;
// 		  __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, pwmVal);    //锟睫改比斤拷值锟斤拷锟睫革拷占锟秸憋拷
// //		  TIM2->CCR2 = pwmVal;     锟斤拷锟较凤拷锟斤拷同
// 		  HAL_Delay(1);
// 	  }
}