#include "bsp_common.h"
char test[20]={0};
//DHT22��ʼ��
u8 DHT22_Rst(void)
{
	u8 retry=0;
	DHT22_port_out();
//	DHT22_out_high();//�����ͷ�����20-40us
//	delay_us(10);
	DHT22_out_low();//������������500Us
	delay_us(500);
	DHT22_out_high();//�����ͷ�����20-40us
	delay_us(30);
	DHT22_port_in();
	delay_us(80);
		while (DHT22_DQ_IN&&retry<100)//DHT22������40~80us
	{
		retry++;
		delay_us(1);
	};	 
	if(retry>=100||retry<=1) return 0;	
	else {
		//USART_SendBytes(USART1, &retry, sizeof(retry));
		retry=0;
	}
  while (!DHT22_DQ_IN&&retry<100)//DHT22���ͺ���ٴ�����40~80us
	{
		retry++;
		delay_us(1);
	};
	if(retry>=100||retry<=1)
	{
		return 0;	 
	} 
	return 1;
}
//��ȡһ���ֽ�����
u8 DHT22_ReadByte()
{
	u8 i,bit=0;
	for(i=0;i<8;i++)
	{
		u8 retry=0;
		bit<<=1;
//		if(DHT22_DQ_IN)//�ж����ŵ�ƽ�Ƿ�Ϊ�͵�ƽ
//		{	
		while(!DHT22_DQ_IN&&retry<100)//�ȴ���Ϊ�͵�ƽ
		{
			retry++;
			delay_us(1);
		}
		retry=0;	
		
			//while(DHT22_DQ_IN);//�ȴ��͵�ƽ����
			delay_us(40);
		if(DHT22_DQ_IN)
			{
				//USART_DEBUG("ci shu +1\r\n");
				bit |=0x1;
			}
			while(DHT22_DQ_IN&&retry<100)//�ȴ���Ϊ�ߵ�ƽ
		{
			retry++;
			delay_us(1);
		}		
		
	}
	return bit;
}

u8 DHT22_Read_Data(u16 *humi,u16 *temp)
{
	u8 i=0;
	u8 buf[5];
	u8 check;
	if(DHT22_Rst())
		{		
			while(i<5)
			{
				buf[i]=DHT22_ReadByte();
				i++;
			}
			check=buf[0]+buf[1]+buf[2]+buf[3];
			if(check==buf[4])
				{
					*humi=(buf[0]<<8|buf[1])/10;//����8λ�͵�8λ���ݺϲ���16λ����ת����ʮ��������
					
					*temp=(buf[2]<<8|buf[3])/10;//����8λ�͵�8λ���ݺϲ���16λ����ת����ʮ��������
					USART_DEBUG("DHT22 read OK\r\n");
					return 1;
				}
				else
				{
					USART_DEBUG("wen du du qu cuo wu------>\r\n");
					sprintf(test,"shidu:%d,%d,wendu:%d,%d,jiaoyan:%d��check:%d\r\n",buf[0],buf[1],buf[2],buf[3],buf[4],check);
					USART_DEBUG(test);
					return 0;
				}
		}
		else
		{
			USART_DEBUG("DHT22 rest is error\r\n");
			return 0;
		}
}



