#include "bsp_common.h"



extern char ATBuffer[AT_BUFFER_LEN]; //数据缓冲区

u8 send_AT_cmd(char *AT_cmd,char *AT_ack,u16 waittime)
{
	u8 rs_data;
	u16 c=3;
	//最多重发3次
	while(c>0)
	{
		c--;
		//strcat(AT_cmd," \r\n");
		USART_SendBytess(USART2,AT_cmd);//发送AT指令
		USART_SendBytess(USART2,"\r\n");
		USART_SendBytess(USART1,AT_cmd);
		delay_ms(waittime);
		delay_ms(1000);
		if(USART_BufferRead(&rs_data) == 0)
		{
			USART_SendBytess(USART1,"Get AT_ACK is overtime");			//AT指令返回失败
			
		}
		else
			{
				
			
//		while(USART_BufferRead(&rs_data) == 0)
//		{
//			int i=1;
//			delay_ms(1);
//			i++;
//			if(i>10)
//			{
//				USART_SendBytess(USART1,"Get AT_ACK is overtime");			//AT指令返回失败
//				break;
//			}
//		}
					
					
					USART_DEBUG("\r\n");
				if(esp8266_check_cmd(AT_ack)!=NULL)
				{
					USART_SendBytess(USART1, "AT command ACK is Successfly\r\n");			//AT指令发送和确认成功
					
					return 1;
				}
			}
		}	
	USART_SendBytess(USART1,"3thd AT ACK error\r\n");			//3次AT指令发送失败
	return 0;
}


//ESP8266发送命令后,检测接收到的应答
//str:期待的应答结果
//返回值:0,没有得到期待的应答结果;其他,期待应答结果的位置(str的位置)
u8* esp8266_check_cmd(char *str)
{
	u16 i=0;
	char *strx=0;
	 
		//USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//添加结束符
		strx=strstr((const char*)ATBuffer,(const char*)str);
		USART_DEBUG("------------huan chong qu---------\r\n");
		USART_DEBUG((char*)ATBuffer);
		USART_DEBUG("\r\n");
	USART_DEBUG("+++++++++++++++++++++++++++\r\n");
		while(i<1000)
		{
			ATBuffer[i]='\0';
			i++;
		}
		
	return (u8*)strx;
}


//
//ESP8266模块和PC进入透传模式
//
void wifi_start_trans(void)
{
	//设置工作模式 1：station模式   2：AP模式  3：兼容 AP+station模式
	send_AT_cmd("AT+CWMODE=1\r\n","OK",50);
	USART_SendBytess(USART1,"AT+CWMODE=1 jie shu\r\n");
	
//	//让Wifi模块重启的命令
//	send_AT_cmd("AT+RST","ready",20);
//	
//	delay_ms(1000);         //延时5S等待重启成功
//	delay_ms(1000);
//	delay_ms(1000);
//	delay_ms(1000);
//	delay_ms(1000);
	
	//让模块连接上自己的路由
	send_AT_cmd("AT+CWJAP=\"ZNHL\",\"ZnHl@4501.\"","WIFI CONNECTED",200);
	
	//=0：单路连接模式     =1：多路连接模式
	send_AT_cmd("AT+CIPMUX=0","OK",20);
	
	//建立TCP连接  这四项分别代表了 要连接的ID号0~4   连接类型  远程服务器IP地址   远程服务器端口号
	send_AT_cmd("AT+CIPSTART=\"TCP\",\"192.168.3.202\",8580","CONNECT",200);
	
	//是否开启透传模式  0：表示关闭 1：表示开启透传
	send_AT_cmd("AT+CIPMODE=1","OK",200);
	
	//透传模式下 开始发送数据的指令 这个指令之后就可以直接发数据了
	send_AT_cmd("AT+CIPSEND","OK",50);
}




