#include "bsp_common.h"

char strtemp[128] = {0};

int main(void)
{
	u8 data = 0;
	u8 cnt = 0;
	u16 nlen = 0;
	u8 ntemp[512] = {0};
	u8 ndat[512] = {0};
	u16 ncrc = 0;
	u16 ncmd = 0;
#if SYS_ENABLE_IAP
    SCB->VTOR = 0x8002000;
    __enable_irq();
#endif
    delay_init();            //延时函数初始化
    GPIO_Configure();
    USART_Configure();
    NVIC_Configure();
	TIM3_Int_Init(999, 7199);	//100ms
	
	sprintf(strtemp, "%s-%s.%s%s\r\n", Prefix, Version_Year, Version_Month, Version_Day);
	USART_SendBytes(USART1, (u8*)strtemp, sizeof(strtemp));			//打印版本信息
#if SYS_ENABLE_IAP

    if(IAP_Read_UpdateFLAG() != 1)
    {
        IAP_Write_UpdateFLAG();
    }

#endif
 wifi_start_trans();
 USART_SendBytess(USART1,"wifi tou chuan jie shu");
	while(1)
	{
		if(USART_BufferRead(&data) != 0)
		{
			ntemp[((++cnt) % 512)] = data;
//			sprintf(strtemp, "cnt:%d\r\n", cnt);
//			USART_DEBUG(strtemp);
			if(ntemp[cnt] == 0xEE && ntemp[cnt - 1] == 0xDD)
			{
//				USART_DEBUG("start1\r\n");
				nlen = MAKEWORD(ntemp[cnt - 5], ntemp[cnt - 4]);
				if(cnt > nlen)
				{
					ncrc = ModBusCRC(&ntemp[cnt - nlen - 5], nlen + 2);
//					sprintf(strtemp, "ncrc:%X\r\n", ncrc);
//					USART_DEBUG(strtemp);
					if(ncrc == MAKEWORD(ntemp[cnt - 3], ntemp[cnt - 2]))
					{
						ncmd = MAKEWORD(ntemp[cnt - nlen - 5], ntemp[cnt - nlen - 4]);		//指令
						memset(ndat, 0, sizeof(ndat));
						memcpy(&ndat[0], &ntemp[cnt - nlen - 3], nlen - 2);				//数据
						//命令及数据处理
						HandleDatCmd(ncmd, (char *)ndat, nlen - 2);	
						cnt = 0;	//清空计数						
//						USART_DEBUG("RevSuccess\r\n");
					}
				}
				
			}
		}
		
	}
}




