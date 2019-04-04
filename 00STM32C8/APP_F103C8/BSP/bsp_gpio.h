#ifndef _BSP_GPIO_H
#define _BSP_GPIO_H
#include "stm32f10x_conf.h"





//引脚输入输出宏定义
//小灯开关
#define  LED_OPEN    		GPIO_SetBits(GPIOC, GPIO_Pin_13)
#define  LED_CLOSE   		GPIO_ResetBits(GPIOC, GPIO_Pin_13)
#define  LED_SWITCH()	 	(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) ? LED_CLOSE : LED_OPEN)
#define EXTI_CONFIG_ENABLED    	(1)//引脚中断触发

#define 	Human_body_Light_ON 						PAout(7)=1		//人体传感器小灯开
#define 	Human_body_Light_OFF  					PAout(7)=0		//人体传感器小灯关

#define 	light_sensor_port_in()		{GPIOB->CRL&=0XFFF0FFFF;GPIOB->CRL|=(u32)8<<5;}	//输入
#define		fen_out								PBout(3)			//风扇引脚
#define		Home_light						PBout(4)			//	房屋灯引脚
#define		light_sensor_port			PBin(5)				//	光敏二极管引脚
#define 	Buzzer_potr						PAout(1)			//蜂鸣器

//函数定义
void USART_Configure(void);
void GPIO_Configure(void);
void NVIC_Configure(void);
void GPIOB_set(uint16_t pin,u8 state);
void OLED_SpiInit(void);
void EXTIX_Init(void);

#endif  //_BSP_GPIO_H


