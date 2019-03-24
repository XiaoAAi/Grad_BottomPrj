#ifndef _BSP_OLED_H
#define _BSP_OLED_H

#include "stm32f10x.h"

#include "stm32f10x_rcc.h"

#include "stm32f10x_gpio.h"

#include "stm32f10x_conf.h"



#define  OLED_SCL_PIN	 	12
#define  OLED_SDA_PIN	 	13
#define  OLED_RST_PIN	 	14
#define  OLED_DC_PIN		15
#define  OLED_CS_PIN	 	8



void OLED_Init(void);
void OLED_Fill(u8 bmp_data);
void OLED_WrDat(u8 data);
void OLED_Wr6Dat(u8 data);
void OLED_PutPixel(u8 x,u8 y);
void OLED_ClrPixel(u8 x,u8 y);
void OLED_Set_Pos(u8 x, u8 y);
void OLED_HEXACSII(u16 hex,u8* Print);
void OLED_Print_Num1(u8 x, u8 y, s16 num);
void dis_bmp(u16 high, u16 width, u8 *p,u8 value);
void OLED_Print_Num(u8 x, u8 y, u16 num);
void OLED_P8x16Str(u8 x,u8 y,u8 ch[]);
void OLED_P6x8Str(u8 x,u8 y,u8 ch[]);

void RCC_congig(void);//∫ÙŒ¸µ∆≈‰÷√




#endif

