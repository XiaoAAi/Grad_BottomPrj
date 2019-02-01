#ifndef _BSP_GPIO_H
#define _BSP_GPIO_H
#include "stm32f10x_conf.h"

//调试按键
#define KEY0  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3)

//按键KEY0键值
#define KEY0_PRES   1   //KEY0按下

//红外接收灯
#define LED_RCV_L1  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)
#define LED_RCV_L2  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_10)
#define LED_RCV_L3  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_11)
#define LED_RCV_L4  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12)
#define LED_RCV_L5  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2)
#define LED_RCV_L6  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3)
#define LED_RCV_L7  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4)
#define LED_RCV_L8  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)
#define LED_RCV_L9  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)
#define LED_RCV_L10 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)

//红外发射灯
#define LED_SEND_L1 PAout(6)
#define LED_SEND_L2 PAout(7)
#define LED_SEND_L3 PCout(4)
#define LED_SEND_L4 PCout(5)
#define LED_SEND_L5 PBout(0)
#define LED_SEND_L6 PBout(1)
#define LED_SEND_L7 PBout(10)
#define LED_SEND_L8 PBout(11)
#define LED_SEND_L9 PBout(12)
#define LED_SEND_L10 PBout(13)

//调试灯，常态为高电平，灯灭；为低电平，灯亮
#define LED_Red PCout(0)
#define LED_Blue PCout(1)
#define LED_Green PCout(2)
//灯亮
#define LED_Red_ON()        (PCout(0) = 0)
#define LED_Blue_ON()       (PCout(1) = 0)
#define LED_Green_ON()      (PCout(2) = 0)
//灯灭
#define LED_Red_OFF()       (PCout(0) = 1)
#define LED_Blue_OFF()      (PCout(1) = 1)
#define LED_Green_OFF()     (PCout(2) = 1)

//红外发射功率控制
#define LED_Power1 PCout(9)
#define LED_Power2 PCout(8)
#define LED_Power3 PCout(7)


//掉货检测
#define PUT_THING  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)
//掉货使能
#define PUT_ST PCout(6)

void USART_Configure(void);
void GPIO_Configure(void);
void NVIC_Configure(void);

u8 KEY_Scan(u8);    //按键扫描函数
//功能：设置红外发射功率
void LED_Power_ctrl(void);
void Led_Init_HY(void);


#endif  //_BSP_GPIO_H


