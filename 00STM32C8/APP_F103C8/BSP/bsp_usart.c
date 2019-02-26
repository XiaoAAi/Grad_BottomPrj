#include "bsp_common.h"

bool flagEnableDebug = TRUE;		//DEBUG打印

u8 UsartBuffer[USART_BUFFER_LEN] = {0}; //数据缓冲区
u16 UsartWptr = 0;
u16 UsartRptr = 0;



//功能：串口1接PC机
#if USART1_CONFIG_ENABLED > 0
void USART1_IRQHandler(void)
{
    uint8_t nTemp;

    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        nTemp = USART_ReceiveData(USART1);
        USART_ClearITPendingBit(USART1, USART_IT_RXNE); //clear flag
        USART_BufferWrite(nTemp);
    }

    if(USART_GetFlagStatus(USART1, USART_FLAG_ORE) == SET) //overflow
    {
        USART_ReceiveData(USART1);    // delete data
        USART_ClearFlag(USART1, USART_FLAG_ORE);
    }
}
#endif

//功能：串口2接电机板
#if USART2_CONFIG_ENABLED > 0
void USART2_IRQHandler(void)
{
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

}
#endif

#if USART3_CONFIG_ENABLED > 0
void USART3_IRQHandler(void)
{
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
    if(flagEnableDebug)
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

}

void SendCmd(USART_TypeDef* USARTx, u16 cmd)//无数据区数据包
{
    u8 str[8] = {0};
    u8 cnt = 0;
    u16 ncrc = 0;
    str[0] = HBYTE(cmd);
    str[1] = LBYTE(cmd);
    str[2] = 0;
    str[3] = 2;
    ncrc = ModBusCRC(&str[0], 4);
    str[4] = HBYTE(ncrc);
    str[5] = LBYTE(ncrc);
    str[6] = 0xDD;
    str[7] = 0xEE;

    for(cnt = 0; cnt < 8; cnt++)
    {
        USART_SendByte(USARTx, str[cnt]);
    }
}

void SendCmdDat(USART_TypeDef* USARTx, u16 cmd, char *dat, u16 dat_len)//完整数据包
{
    u8 cnt = 0;
    u16 ncrc = 0;
    u8 str[512] = {0};
    u16 datalen = 0;
    str[0] = HBYTE(cmd);
    str[1] = LBYTE(cmd);
    memcpy(&str[2], &dat[0], dat_len);
    datalen = dat_len + 2;
    str[dat_len + 2] = HBYTE(datalen);
    str[dat_len + 3] = LBYTE(datalen);
    ncrc = ModBusCRC(&str[0], 4 + dat_len);
    str[dat_len + 4] = HBYTE(ncrc);
    str[dat_len + 5] = LBYTE(ncrc);
    str[dat_len + 6] = 0xDD;
    str[dat_len + 7] = 0xEE;

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
//	sprintf(strtemp, "Data:%X\r\n", *data);
//	USART_DEBUG(strtemp);
	return 1;
}

//写入数据 + 接收中断处理
void USART_BufferWrite(u8 ntemp)
{
    if((UsartWptr + 1) % USART_BUFFER_LEN == UsartRptr) // full
    {
        return;
    }

    UsartBuffer[UsartWptr] = ntemp;

    if(UsartBuffer[UsartWptr] == 0xEE && UsartBuffer[(USART_BUFFER_LEN + UsartWptr - 1) % USART_BUFFER_LEN] == 0xDD
            && UsartBuffer[(USART_BUFFER_LEN + UsartWptr - 2) % USART_BUFFER_LEN] == 0xDA && UsartBuffer[(USART_BUFFER_LEN + UsartWptr - 3) % USART_BUFFER_LEN] == 0xE1
            && UsartBuffer[(USART_BUFFER_LEN + UsartWptr - 4) % USART_BUFFER_LEN] == 0x02 && UsartBuffer[(USART_BUFFER_LEN + UsartWptr - 5) % USART_BUFFER_LEN] == 0x00
            && UsartBuffer[(USART_BUFFER_LEN + UsartWptr - 6) % USART_BUFFER_LEN] == 0x0E && UsartBuffer[(USART_BUFFER_LEN + UsartWptr - 7) % USART_BUFFER_LEN] == 0x01)
    {
		SendCmd(USART2, USART_SERVER_BUTTOM_ResetButtom);			//发送板子重启反馈
		USART_SendBytes(USART1, (u8*)"ButtomLayerReset\r\n", 19);	//做打印也做延时
        //复位对射
        __disable_irq();
        NVIC_SystemReset();
    }

    UsartWptr = (UsartWptr + 1) % USART_BUFFER_LEN;
}

//功能：主要用于指令的处理
void HandleDatCmd(u16 cmd, char* dat, u16 datLen)
{
	sprintf(strtemp, "Cmd: %X\r\n", cmd);
	USART_DEBUG(strtemp);
	
	if(cmd == USART_SERVER_BUTTOM_WillUpdate) //准备升级对射
	{
		SendCmd(USART2, USART_SERVER_BUTTOM_WillUpdateFeedBack);
		USART_SendBytes(USART1, (u8*)"WillUpdateButtom\r\n", 18);			//做打印也做延时
		IAP_Reset_UpdateFLAG();
		__disable_irq();
		NVIC_SystemReset();
	}
	else if(cmd == USART_SERVER_BUTTOM_OpenLight)			//开灯指令
	{
		USART_DEBUG("OpenLightCol\r\n");
	}
	
	
}




