#ifndef _BSP_TIMER_H
#define _BSP_TIMER_H
#include "stm32f10x_conf.h"

//定时器中断优先级设置使能位
#define TIM2_CONFIG_ENABLED     (1)
#define TIM3_CONFIG_ENABLED     (1)

//红外接收引脚，接收到38kHz,输入低电平
#define RCV_38kHz   0

//红外层检测，检测到物体遮挡次数设置(根据商品大小进行适当测试)
#define NUM_ZHEDANG     6



void TIM2_Int_Init(u16 arr, u16 psc);
void TIM3_Int_Init(u16 arr, u16 psc);

#endif  //_BSP_TIM_H
