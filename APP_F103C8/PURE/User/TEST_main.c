#include "bsp_common.h"


u8 strtemp[128];        //用于打印调试信息
extern u8 flag_test;
extern u8 Enable_LEVEL;

int main(void)
{
//    u8 data = 0;
//    u16 i = 0;
#if SYS_ENABLE_IAP
    SCB->VTOR = 0x8002000;
    __enable_irq();
#endif
    delay_init();            //延时函数初始化
    NVIC_Configure();
    GPIO_Configure();
    USART_Configure();
    TIM3_Int_Init(472, 1);  //38KHz 载波
    TIM2_Int_Init(9, 7199); //1mS时间轮
    LED_Power_ctrl();       //设置红外发射功率
    memset(strtemp, 0, sizeof(strtemp));
    sprintf((char*)strtemp, "%s.%s%s\r\n", Version_Year, Version_Month, Version_Day);
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
        if(flag_test == 1)
        {
            flag_test = 0;      //清零
            Enable_LEVEL = 1;   //对应层对射开启
            USART_DEBUG("LEVEL_1\r\n");
        }
        else if(flag_test == 2)
        {
            flag_test = 0;      //清零
            Enable_LEVEL = 2;
            USART_DEBUG("LEVEL_2\r\n");
        }
        else if(flag_test == 3)
        {
            flag_test = 0;      //清零
            Enable_LEVEL = 3;
            USART_DEBUG("LEVEL_3\r\n");
        }
        else if(flag_test == 4)
        {
            flag_test = 0;      //清零
            Enable_LEVEL = 4;
            USART_DEBUG("LEVEL_4\r\n");
        }
        else if(flag_test == 5)
        {
            flag_test = 0;      //清零
            Enable_LEVEL = 5;
            USART_DEBUG("LEVEL_5\r\n");
        }
        else if(flag_test == 6)
        {
            flag_test = 0;      //清零
            Enable_LEVEL = 6;
            USART_DEBUG("LEVEL_6\r\n");
        }
        else if(flag_test == 7)
        {
            flag_test = 0;      //清零
            Enable_LEVEL = 0;   //清零
            TIM_Cmd(TIM3, DISABLE);  //关闭TIM3，不发射载波
            USART_DEBUG("Qing Ling\r\n");
        }
    }
}
