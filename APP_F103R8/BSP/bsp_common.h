#ifndef _BSP_COMMON_H
#define _BSP_COMMON_H
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>
#include "stm32f10x_conf.h"
#include "stm32f10x.h"



#if SYSTEM_SUPPORT_UCOS
#include "includes.h"
#endif

typedef enum
{
    FALSE = 0, TRUE = 1
} bool;

#include "bsp_usart.h"
#include "bsp_gpio.h"
#include "bsp_timer.h"
#include "bsp_delay.h"
#include "bsp_sys.h"

#define HBYTE(a)               ((unsigned char)(((unsigned short)(a) >> 8) & 0xFF))
#define LBYTE(a)               ((unsigned char)((unsigned short)(a)& 0xff))
#define MAKEWORD(bLow,bHigh)   ((unsigned short)(((unsigned char)(bLow))|(((unsigned short)((unsigned char)(bHigh)))<<8)))

//版本号及前缀
#define   Prefix            "HJG_HY"    //前缀，区分不同机型
#define   Version_Year      "68"       // year
#define   Version_Month     "08"      // month
#define   Version_Day       "04"      // day

//红外对射检测方案选择，方案1：对应层检测到阻挡就发送回馈；方案2：检测到阻挡，需要再次接收到红外光才能电机停止，对应红酒柜结构
//#define DUISHE_VER      1

unsigned short CRC16_isr(unsigned char *Dat, unsigned int len);
bool strLenCmp(char *cmd1, char *cmd2, u8 len);
bool strLenCpy(char *dst, char *src, u8 len);
u8 strLen(char *cmd);

void delay_init(void);
void delay_ms(u16 nms);
void delay_us(u32 nus);


//用于各个文件打印调试信息
extern u8 strtemp[128];

#endif  //_BSP_COMMON_H



