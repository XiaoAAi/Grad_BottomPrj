#ifndef _BSP_USART_H
#define _BSP_USART_H
#include "stm32f10x_conf.h"

//串口接收队列缓存长度
#define USART_BUFFER_LEN    4000

//红外对射--对射升级HEX版本定义DUISHE_SYS_UPDATE宏，其他版本没有定义
#ifndef DUISHE_SYS_UPDATE
#define SYS_ENABLE_IAP      0
#else
#define SYS_ENABLE_IAP      1
#endif


//定义配置使能
#define USART1_CONFIG_ENABLED                (1)
#define USART2_CONFIG_ENABLED                (1)
#define USART3_CONFIG_ENABLED                (0)



//定义串口波特率
#define USART1_BAUDRATE                       9600
#define USART2_BAUDRATE                       9600
#define USART3_BAUDRATE                       9600


//串口功能定义
//升级指令
#define  USART_SERVER_BUTTOM_WillUpdate					0x01AD	//将要升级
#define  USART_SERVER_BUTTOM_WillUpdateFeedBack			0x01BD	//将要升级回馈
#define  USART_SERVER_BUTTOM_StartUpdate				0x01AE	//开始升级
#define  USART_SERVER_BUTTOM_StartUpdateFeedBack		0x01BE	//开始升级回馈
#define  USART_SERVER_BUTTOM_StopUpdate					0x01AF	//停止升级
#define  USART_SERVER_BUTTOM_StopUpdateFeedBack			0x01BF	//停止升级回馈

//重启指令
#define  USART_SERVER_BUTTOM_ResetButtom				0x010E	//重启底层板子及回馈

//开灯指令
#define  USART_SERVER_BUTTOM_OpenLight					0x0101	//开灯指令



//IAP系统升级
#define UPDATE_FLAG_FLASH_ADDR              0x800BB80    //写入升级标志地址
#define UPDATE_FLAG_FLASH_DATA             0xDDCCBBAA   //写入升级标志值
#define UPDATE_FLAG_FLASH_DATA1            0x3210ABCD   //写入升级标志值



//函数接口
void USART_SendByte(USART_TypeDef* USARTx, uint8_t byte);
void USART_SendBytes(USART_TypeDef* USARTx, uint8_t *str, uint8_t len);
u8 USART_BufferRead(u8 *data);
void USART_BufferWrite(u8 ntemp);
void IAP_Write_UpdateFLAG(void);
void IAP_Reset_UpdateFLAG(void);
u8 IAP_Read_UpdateFLAG(void);
void HandleDatCmd(u16 cmd, char* dat, u16 datLen);
void SendCmd(USART_TypeDef* USARTx, u16 cmd);								//发送不带数据区的数据包
void SendCmdDat(USART_TypeDef* USARTx, u16 cmd, char *dat, u16 dat_len);	//发送带数据区的数据包
void USART_DEBUG(char *str);	//打印到PC端    调试

#endif  //_BSP_USART_H


