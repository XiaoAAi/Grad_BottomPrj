#include "bsp_common.h"

//DHT22初始化
u8 DHT22_Rst(void)
{
	u8 retry=0;
	DHT22_port_out();
	DHT22_out_high();//主机释放总线20-40us
	delay_us(10);
	DHT22_out_low();//主机拉低总线500Us
	delay_us(800);
	DHT22_out_high();//主机释放总线20-40us
	delay_us(30);
	DHT22_port_in();
	delay_us(80);
//	if(DHT22_DQ_IN)//判断引脚电平是否为低电平
//	{
//		while(DHT22_DQ_IN);
//		delay_us(80);
//		if(!DHT22_DQ_IN)//判断引脚输入是否为高
//			return 1;
//		else 
//			return 0;
//	}
//	else
//	{		
//		USART_DEBUG("mei you di dian ping\r\n");
//		return 0;
//	}
		while (DHT22_DQ_IN&&retry<100)//DHT22会拉低40~80us
	{
		retry++;
		delay_us(1);
	};	 
	if(retry>=100)return 0;
	else retry=0;
  while (!DHT22_DQ_IN&&retry<100)//DHT22拉低后会再次拉高40~80us
	{
		retry++;
		delay_us(1);
	};
	if(retry>=100)
	{
		return 0;	 
	}   
	return 1;
}
//等待DHT22是否有回应
u8 DHT22_ReadByte()
{
	u8 i,bit=0;
	for(i=0;i<8;i++)
	{
		u8 retry=0;
//		if(DHT22_DQ_IN)//判断引脚电平是否为低电平
//		{	
		while(DHT22_DQ_IN&&retry<100)//等待变为高电平
		{
			retry++;
			delay_us(1);
		}
		retry=0;			
			//while(DHT22_DQ_IN);//等待低电平结束
			delay_us(40);
			if(!DHT22_DQ_IN)
			{
				USART_DEBUG("ci shu +1\r\n");
				bit|=(0x80>>i);
			}
				//while(!DHT22_DQ_IN);//GPIOB->IDR & GPIO_IDR_IDR8
			while(!DHT22_DQ_IN&&retry<100)//等待变为低电平
		{
			retry++;
			delay_us(1);
		}
	
//		}
	}
	return bit;
}

u8 DHT22_Read_Data(u8 *humi,u8 *temp)
{
	u8 i,buf[5];
	if(DHT22_Rst())
		{		
//			USART_DEBUG("chu shi hua cheng gong\r\n");
//			delay_ms(1000);
//			if(DHT22_Rst())
//				USART_DEBUG("chu shi hua222 cheng gong\r\n");
			for (i=0;i<5;i++)
			{
				buf[i]=DHT22_ReadByte();
			}
			if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4]&&buf[4]!=NULL)
				{
					*humi=buf[0]<<8|buf[1];//将高8位和低8位数据合并成16位，再转换成十进制数据
					
					*temp=buf[2]<<8|buf[3];//将高8位和低8位数据合并成16位，再转换成十进制数据
					USART_DEBUG("999999999999\r\n");
					USART_SendByte(USART1,buf[0]);
					return 1;
				}
				else
				{
					USART_DEBUG("wen du du qu cuo wu------>\r\n");
					USART_SendByte(USART1,buf[1]);
					USART_SendByte(USART1,buf[2]);
					USART_SendByte(USART1,buf[3]);
					USART_SendByte(USART1,buf[4]);
					return 0;
				}
		}
		else
		{
			USART_DEBUG("chushi shua shibai 777777777777777\r\n");
			return 0;
		}
}



