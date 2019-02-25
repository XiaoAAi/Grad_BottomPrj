#ifndef _BSP_GPIO_H
#define _BSP_GPIO_H
#include "stm32f10x_conf.h"





//引脚输入输出宏定义
//小灯开关
#define  LED_OPEN    		GPIO_SetBits(GPIOC, GPIO_Pin_13)
#define  LED_CLOSE   		GPIO_ResetBits(GPIOC, GPIO_Pin_13)
#define  LED_SWITCH()	 	(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) ? LED_CLOSE : LED_OPEN)




//函数定义
void USART_Configure(void);
void GPIO_Configure(void);
void NVIC_Configure(void);


#endif  //_BSP_GPIO_H


