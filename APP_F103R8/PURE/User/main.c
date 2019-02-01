#include "bsp_common.h"

u8 strtemp[128];  //用于打印调试信息
u8 PUT_ST_BIT = 1; //

int main(void)
{
    u8 data = 0;
    u16 i = 0;
    u8 ntmp[255] = {0};
    u8 ndat[255] = {0}; // 协议数据
    u16 nlen = 0; // 协议数据包长度
    u16 ncrc = 0; // 协议crc16
    u16 ncmd = 0; // 协议指令
#if SYS_ENABLE_IAP
    SCB->VTOR = 0x8002000;
    __enable_irq();
#endif
    delay_init();            //延时函数初始化
    GPIO_Configure();
    USART_Configure();
    NVIC_Configure();
    TIM2_Int_Init(9, 7199); //1mS时间轮
    TIM3_Int_Init(472, 1);  //38KHz 载波
    LED_Power_ctrl();       //设置红外发射功率
    memset(strtemp, 0, sizeof(strtemp));
    sprintf((char*)strtemp, "%s--%s.%s%s\r\n", Prefix, Version_Year, Version_Month, Version_Day);
    USART_SendBytes(USART1, strtemp, sizeof(strtemp));
#if SYS_ENABLE_IAP

    if(IAP_Read_UpdateFLAG() != 1)
    {
        Send_CMD(USART2, HBYTE(USARTCMD_DIANJI_DUISHE_StopUpdateDuishe), LBYTE(USARTCMD_DIANJI_DUISHE_StopUpdateDuishe));
        IAP_Write_UpdateFLAG();
    }

#endif

    while(1)
    {
        if(USART_BufferRead(&data) == 1)
        {
            i++;
            i = i % 255;
            ntmp[i] = data;

            if(ntmp[i - 1] == 0x0D && ntmp[i] == 0x0A) // 判断包尾
            {
                nlen = MAKEWORD(ntmp[i - 4], ntmp[i - 5]); // 获取数据包长度

                if(i > nlen)
                {
                    ncrc = CRC16_isr(&ntmp[i - (nlen + 5)], nlen + 2); // crc计算

                    if(ncrc == MAKEWORD(ntmp[i - 2], ntmp[i - 3])) // crc判断
                    {
                        ncmd = MAKEWORD(ntmp[i - (nlen + 5 - 1)], ntmp[i - (nlen + 5)]); // 解析出串口协议命令,cmd1+cmd2

                        if(nlen > 2) // 获取数据区域
                        {
                            memset(ndat, 0, sizeof(ndat));
                            memcpy(ndat, &ntmp[i - (nlen + 5 - 2)], nlen - 2);		//解析出的数据 ndat
                            Handle_USART_CMD(ncmd, (char *)ndat, nlen - 2); // 处理指令+数据
                        }
                        else
                        {
                            Handle_USART_CMD(ncmd, "", 0); // 处理指令
                        }

                        i = 0;
                    }
                }
            }
        }

        /*掉货检测*/
        if(PUT_ST_BIT == 0)
        {
						delay_ms(10);
            if(PUT_THING == 1)
            {
                Send_CMD(USART2, HBYTE(DUISHE_DIANJI_NUMb7_CheckThings), LBYTE(DUISHE_DIANJI_NUMb7_CheckThings));
                USART_SendByte_Test(0x88);
                //Send_CMD(USART1, HBYTE(DUISHE_DIANJI_NUMb7_CheckThings), LBYTE(DUISHE_DIANJI_NUMb7_CheckThings));
            }
        }
    }
}




