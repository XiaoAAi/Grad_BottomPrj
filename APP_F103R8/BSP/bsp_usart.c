#include "bsp_common.h"

u8 UsartBuffer[USART_BUFFER_LEN] = {0}; //数据缓冲区
u16 UsartWptr = 0;
u16 UsartRptr = 0;
bool flag_enable_debug = FALSE;     //调试DUBUG开关标志位，默认debug关闭
//调试标志位
u8 flag_test = 0;
u8 Enable_LEVEL = 0;    //纪录红外检测的第几层(记录电机板发过来的层数)
//红外检测功能选择，红外检测用在到达某一层和货道出货两种，串口发送选择功能
u8 flag_HY = 0X55;      //0x55为到达某一层；0xAA为货道出货，默认为货道出货
extern u8 PUT_ST_BIT;

//加入以下代码,支持printf函数,而不需要选择use MicroLIB
#if 0
#pragma import(__use_no_semihosting)
//标准库需要的支持函数
struct __FILE
{
    int handle;

};

FILE __stdout;
//定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
    x = x;
}
//重定义fputc函数
int fputc(int ch, FILE *f)
{
    while((USART1->SR & 0X40) == 0); //循环发送,直到发送完毕

    USART1->DR = (u8) ch;
    return ch;
}
#endif

//功能：串口1接PC机
#if USART1_CONFIG_ENABLED > 0
void USART1_IRQHandler(void)
{
#if SYSTEM_SUPPORT_UCOS
    OSIntEnter();
#endif
    uint8_t nTemp;

    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        nTemp = USART_ReceiveData(USART1);
        USART_ClearITPendingBit(USART1, USART_IT_RXNE); //clear flag
        /**********************************************/
        flag_test = nTemp;
        USART_BufferWrite(nTemp);
    }

    if(USART_GetFlagStatus(USART1, USART_FLAG_ORE) == SET) //overflow
    {
        USART_ReceiveData(USART1);    // delete data
        USART_ClearFlag(USART1, USART_FLAG_ORE);
    }

#if SYSTEM_SUPPORT_UCOS
    OSIntExit();
#endif
}
#endif

//功能：串口2接电机板
#if USART2_CONFIG_ENABLED > 0
void USART2_IRQHandler(void)
{
#if SYSTEM_SUPPORT_UCOS
    OSIntEnter();
#endif
    uint8_t nTemp;

    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        nTemp = USART_ReceiveData(USART2);
        USART_ClearITPendingBit(USART2, USART_IT_RXNE); //clear flag
        /**********************************************/
        USART_BufferWrite(nTemp);
    }

    if(USART_GetFlagStatus(USART2, USART_FLAG_ORE) == SET) //overflow
    {
        USART_ReceiveData(USART2);    // delete data
        USART_ClearFlag(USART2, USART_FLAG_ORE);
    }

#if SYSTEM_SUPPORT_UCOS
    OSIntExit();
#endif
}
#endif

#if USART3_CONFIG_ENABLED > 0
void USART3_IRQHandler(void)
{
#if SYSTEM_SUPPORT_UCOS
    OSIntEnter();
#endif
    uint8_t nTemp;

    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        nTemp = USART_ReceiveData(USART3);
        USART_ClearITPendingBit(USART3, USART_IT_RXNE); //clear flag
        /**********************************************/
    }

    if(USART_GetFlagStatus(USART3, USART_FLAG_ORE) == SET) //overflow
    {
        USART_ReceiveData(USART3);    // delete data
        USART_ClearFlag(USART3, USART_FLAG_ORE);
    }

#if SYSTEM_SUPPORT_UCOS
    OSIntExit();
#endif
}
#endif

#if UART4_CONFIG_ENABLED > 0
void UART4_IRQHandler(void)
{
#if SYSTEM_SUPPORT_UCOS
    OSIntEnter();
#endif
    uint8_t nTemp;

    if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
    {
        nTemp = USART_ReceiveData(UART4);
        USART_ClearITPendingBit(UART4, USART_IT_RXNE); //clear flag
        /**********************************************/
    }

    if(USART_GetFlagStatus(UART4, USART_FLAG_ORE) == SET) //overflow
    {
        USART_ReceiveData(UART4);    // delete data
        USART_ClearFlag(UART4, USART_FLAG_ORE);
    }

#if SYSTEM_SUPPORT_UCOS
    OSIntExit();
#endif
}
#endif

#if UART5_CONFIG_ENABLED > 0
void UART5_IRQHandler(void)
{
#if SYSTEM_SUPPORT_UCOS
    OSIntEnter();
#endif
    uint8_t nTemp;

    if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
    {
        nTemp = USART_ReceiveData(UART5);
        USART_ClearITPendingBit(UART5, USART_IT_RXNE); //clear flag
        /**********************************************/
    }

    if(USART_GetFlagStatus(UART5, USART_FLAG_ORE) == SET) //overflow
    {
        USART_ReceiveData(UART5);    // delete data
        USART_ClearFlag(UART5, USART_FLAG_ORE);
    }

#if SYSTEM_SUPPORT_UCOS
    OSIntExit();
#endif
}
#endif

//USART发送单字节
void USART_SendByte(USART_TypeDef* USARTx, uint8_t byte)
{
    /* 发送一个字节数据到USART */
    USART_SendData(USARTx, byte);

    /* 等待发送数据寄存器为空 */
    while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
}

//USART发送字符串
void USART_SendBytes(USART_TypeDef* USARTx, uint8_t *str, uint8_t len)
{
    uint8_t i;

    for(i = 0; i < len; i++)
    {
        USART_SendByte(USARTx, str[i]);
    }
}

//USART发送字符串
void USART_SendBytess(USART_TypeDef* USARTx, char *str)
{
    while(*str != '\0')
    {
        USART_SendByte(USARTx, *str);
        str++;
    }
}
void USART_DEBUG(char *str)
{
    if(flag_enable_debug)
    {
        while(*str != '\0')
        {
            USART_SendByte(USART1, *str);
            str++;
        }
    }
}

void IAP_Write_UpdateFLAG(void)
{
    uint32_t Address = 0x00;
    uint32_t Data = UPDATE_FLAG_FLASH_DATA1;
    FLASH_Unlock();
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
    Address = UPDATE_FLAG_FLASH_ADDR;
    FLASH_ErasePage(Address);
    FLASH_ProgramWord(Address, Data);
    FLASH_Lock();
}

void IAP_Reset_UpdateFLAG(void)
{
    uint32_t Address = 0x00;
    FLASH_Unlock();
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
    Address = UPDATE_FLAG_FLASH_ADDR;
    FLASH_ErasePage(Address);
    FLASH_Lock();
}

u8 IAP_Read_UpdateFLAG(void)
{
    uint32_t Address = 0x00;
    uint32_t Data = UPDATE_FLAG_FLASH_DATA;
    uint32_t Data1 = UPDATE_FLAG_FLASH_DATA1;
    Address = UPDATE_FLAG_FLASH_ADDR;

    if(((*(__IO uint32_t*) Address) == Data) || ((*(__IO uint32_t*) Address) == Data1))
    {
        return 1;
    }
    else
    {
        return 0;
    }

    //if((*(__IO uint32_t*) Address) != Data)
    //{
    //return 0;
    //}
    //else
    //{
    //return 1;
    //}
}

void Send_CMD(USART_TypeDef* USARTx, u8 HCMD, u8 LCMD)//无数据区数据包
{
    u8 str[8] = {0};
    u8 cnt = 0;
    u16 ncrc = 0;
    str[0] = HCMD;
    str[1] = LCMD;
    str[2] = 0;
    str[3] = 2;
    ncrc = CRC16_isr(&str[0], 4);
    str[4] = HBYTE(ncrc);
    str[5] = LBYTE(ncrc);
    str[6] = 0x0D;
    str[7] = 0x0A;

    for(cnt = 0; cnt < 8; cnt++)
    {
        USART_SendByte(USARTx, str[cnt]);
    }
}

void Send_CMD_DAT(USART_TypeDef* USARTx, u8 HCMD, u8 LCMD, char *dat, u16 dat_len)//完整数据包
{
    u8 cnt = 0;
    u16 ncrc = 0;
    u8 str[255] = {0};
    u16 datalen = 0;
    str[0] = HCMD;
    str[1] = LCMD;
    memcpy(&str[2], &dat[0], dat_len);
    datalen = dat_len + 2;
    str[dat_len + 2] = HBYTE(datalen);
    str[dat_len + 3] = LBYTE(datalen);
    ncrc = CRC16_isr(&str[0], 4 + dat_len);
    str[dat_len + 4] = HBYTE(ncrc);
    str[dat_len + 5] = LBYTE(ncrc);
    str[dat_len + 6] = 0x0D;
    str[dat_len + 7] = 0x0A;

    for(cnt = 0; cnt < 8 + dat_len; cnt++)
    {
        USART_SendByte(USARTx, str[cnt]);
    }
}

u8 USART_BufferRead(u8 *data)
{
    if(UsartRptr == UsartWptr) // empty
    {
        return 0;
    }

    *data = UsartBuffer[UsartRptr];
    UsartRptr = (UsartRptr + 1) % USART_BUFFER_LEN;
    return 1;
}

void USART_BufferWrite(u8 ntemp)
{
    if((UsartWptr + 1) % USART_BUFFER_LEN == UsartRptr) // full
    {
        return;
    }

    UsartBuffer[UsartWptr] = ntemp;

    if(UsartBuffer[UsartWptr] == 0x0A && UsartBuffer[(USART_BUFFER_LEN + UsartWptr - 1) % USART_BUFFER_LEN] == 0x0D
            && UsartBuffer[(USART_BUFFER_LEN + UsartWptr - 2) % USART_BUFFER_LEN] == 0xC1 && UsartBuffer[(USART_BUFFER_LEN + UsartWptr - 3) % USART_BUFFER_LEN] == 0x81
            && UsartBuffer[(USART_BUFFER_LEN + UsartWptr - 4) % USART_BUFFER_LEN] == 0x02 && UsartBuffer[(USART_BUFFER_LEN + UsartWptr - 5) % USART_BUFFER_LEN] == 0x00
            && UsartBuffer[(USART_BUFFER_LEN + UsartWptr - 6) % USART_BUFFER_LEN] == 0x14 && UsartBuffer[(USART_BUFFER_LEN + UsartWptr - 7) % USART_BUFFER_LEN] == 0x03)
    {
        //复位对射
        __disable_irq();
        NVIC_SystemReset();
    }

    UsartWptr = (UsartWptr + 1) % USART_BUFFER_LEN;
}


//功能：串口命令处理程序
void Handle_USART_CMD(u16 Data, char *Dat, u16 dat_len)
{
    sprintf((char*)strtemp, "Data: %04X\r\n", Data);
    USART_DEBUG((char*)strtemp);

    if(dat_len != 0) // 处理数据区域
    {
        if(Data == USARTCMD_DIANJI_DUISHE_StartLedSend)     //开启对应层的红外检测
        {
            if(dat_len == 2)
            {
                if(*Dat <= LEVEL_MAX && *Dat >= LEVEL_MIN)  //层数在1到10层范围内
                {
                    Enable_LEVEL = *Dat;    //纪录层数，用于定时器中红外检测
                    flag_HY = *(Dat + 1);   //红外检测功能选择
                    sprintf((char*)strtemp, "USARTCMD_DIANJI_DUISHE_StartLedSend: %04X LEVEL: %d %02X\r\n", USARTCMD_DIANJI_DUISHE_StartLedSend, Enable_LEVEL, flag_HY);
                    USART_DEBUG((char*)strtemp);
                }
            }
        }
    }
    else
    {
        if(Data == DIANJI_DUISHE_NUMb4)         //开启掉货检测，电机板->对射板
        {
            PUT_ST = 0;         //掉货检测使能
            PUT_ST_BIT = 0;
        }
        else if(Data == DIANJI_DUISHE_NUMb5)    //关闭掉货检测，电机板->对射板
        {
            PUT_ST = 1;
            PUT_ST_BIT = 1;
        }
        else if(Data == DIANJI_DUISHE_NUMb6)    //收到层反馈，电机板->对射板
        {
            LED_Red = !LED_Red;     //收到层反馈，红灯状态切换
        }
        else if(Data == USARTCMD_DIANJI_DUISHE_WillUpdateDuishe) //准备升级对射
        {
            Send_CMD(USART2, HBYTE(USARTCMD_DIANJI_DUISHE_WillUpdateDuishe), LBYTE(USARTCMD_DIANJI_DUISHE_WillUpdateDuishe));
            IAP_Reset_UpdateFLAG();
            __disable_irq();
            NVIC_SystemReset();
        }
        else if(Data == USARTCMD_DIANJI_DUISHE_GetDuisheVer) // 获取对射版本
        {
            char strstr[20] = {0};
            sprintf(strstr, "%s--%s.%s%s", Prefix, Version_Year, Version_Month, Version_Day);   //发射版本号及前缀
            Send_CMD_DAT(USART2, HBYTE(USARTCMD_DIANJI_DUISHE_GetDuisheVer), LBYTE(USARTCMD_DIANJI_DUISHE_GetDuisheVer), strstr, strlen(strstr));
        }
        else if(Data == 0x01FE) // 开启debug打印
        {
            if(!flag_enable_debug)
            {
                flag_enable_debug = TRUE;
            }

            USART_DEBUG("debug\r\n");
        }
        else if(Data == 0x01FD) // 关闭debug打印
        {
            flag_enable_debug = FALSE;
        }
        else if(Data == 0x01FF) // 软件复位
        {
            SoftwareRESET();
        }
    }
}


//功能：软件复位操作
void SoftwareRESET(void)
{
    __set_FAULTMASK(1);     // 关闭所有中断
    NVIC_SystemReset();     //软件复位，类似于按下RESET按键
}

//功能：测试时使用，仅仅发送一个字节，方便对延时bug的测试
//参数：uint8_t byte :要发送的单个字节
//返回值：无  20180803
void USART_SendByte_Test(uint8_t byte)
{
#if   FLAG_BYTE_TEST
    //发送一个字节到PC端
    USART_SendData(USART1, byte);

    /* 等待发送数据寄存器为空 */
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);

#endif
}




