#include "bsp_common.h"

char strtemp[128] = {0};
volatile u32 led_flag = 0xFFFFF;				//小灯亮的时间
void oled_DHT22(u16 Temperature,u16 Humidity);

int main(void)
{
	u8 data = 0;
	u8 cnt = 0;
	u16 nlen = 0;
	u8 ntemp[512] = {0};
	u8 ndat[512] = {0};
	u16 ncrc = 0;
	u16 ncmd = 0;
	
	u16 Humidity=0,Temperature=0;
	u16 old_Humidity=0,old_Temperature=0;
#if SYS_ENABLE_IAP
    SCB->VTOR = 0x8002000;
    __enable_irq();
#endif
    delay_init();            //延时函数初始化
    GPIO_Configure();
    USART_Configure();
    NVIC_Configure();
	EXTIX_Init();//外部引脚中断初始化
	TIM3_Int_Init(999, 7199);	//100ms
	OLED_Init();//oled初始化
		DHT22_Read_Data(&Humidity,&Temperature);		//读取DHT22数据
	sprintf(strtemp, "%s-%s.%s%s\r\n", Prefix, Version_Year, Version_Month, Version_Day);
	USART_SendBytes(USART1, (u8*)strtemp, sizeof(strtemp));			//打印版本信息
#if SYS_ENABLE_IAP

    if(IAP_Read_UpdateFLAG() != 1)
    {
        IAP_Write_UpdateFLAG();
    }

#endif

	oled_DHT22( Temperature, Humidity);


	wifi_start_trans();		//WIFI 透传打开
 
 USART_SendBytess(USART1,"wifi tou chuan jie shu");
	while(1)
	{		
		led_flag--;		
		if(led_flag==0)
		{	
			//DHT22读取温度部分
			if(DHT22_Read_Data(&Humidity,&Temperature))
				{
					if(Humidity!=old_Humidity||Temperature!=old_Temperature)
					{
						oled_DHT22( Temperature, Humidity);
						old_Humidity=Humidity;					//更新
						old_Temperature=Temperature;
					}
					if(Humidity>=24)		fen_out=1;	//湿度大打开风扇												
					else								fen_out=0;	
				}
				else	oled_DHT22( 0, 0);
				led_flag=0xFFFFFF;
		}
		
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

//函数功能：显示DHT22的温湿度
//参数：Temperature（温度）、Humidity（湿度）
//返回值：无
void oled_DHT22(u16 Temperature,u16 Humidity)
{
	//OLED显示DHT22温湿度
	OLED_P8x16Str(0,0,(u8 *)"Temperature is:");	
	OLED_Print_Num(30,2,Temperature);
	OLED_P8x16Str(70,4,"`C");
	OLED_P8x16Str(0,4,"Humidity is:");
	OLED_Print_Num(30,6,Humidity);
	OLED_P8x16Str(70,6,"%");
}



