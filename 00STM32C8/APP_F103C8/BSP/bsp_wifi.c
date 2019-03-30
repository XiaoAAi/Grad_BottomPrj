#include "bsp_common.h"

extern char ATBuffer[AT_BUFFER_LEN]; 										//数据缓冲区
extern u8 cntAt ;																				//WiFi缓冲计数
u8 wifi_Reset(void);																		//wifi复位
u8 esp8266_check_cmd(char *str);												//检查返回的字符是否正确


//函数功能：启动ESP8266模块和PC进入透传模式
//参数：无
//返回值：1（成功）0（失败）
u8 wifi_start_trans(void)
{
	wifi_dis_trans();
	//设置工作模式 1：station模式   2：AP模式  3：兼容 AP+station模式
	send_AT_cmd("AT+CWMODE=1\r\n","OK",50);
	
	//让模块连接上自己的路由
	send_AT_cmd("AT+CWJAP=\"ZNHL\",\"ZnHl@4501.\"","WIFI CONNECTED",1000);
	
	//=0：单路连接模式     =1：多路连接模式
	send_AT_cmd("AT+CIPMUX=0","OK",50);
	
	//建立TCP连接  这四项分别代表了 要连接的ID号0~4   连接类型  远程服务器IP地址   远程服务器端口号
	if(send_AT_cmd("AT+CIPSTART=\"TCP\",\"192.168.3.202\",8580","CONNECT",200))
	{
		//是否开启透传模式  0：表示关闭 1：表示开启透传
		send_AT_cmd("AT+CIPMODE=1","OK",200);
		
		//透传模式下 开始发送数据的指令 这个指令之后就可以直接发数据了
		if(send_AT_cmd("AT+CIPSEND","OK",50))
		{
			//测试信息
			esp8266_send_data("hello xiao na",50);
			wifi_dis_trans();
			return 1;
		}					
	}
	return 0;
}


//函数功能：发送AT指令启动WiFi模式
//参数：AT_cmd：AT指令、AT_ack：指令返回值、waittime:等待时间
//返回值：1（成功）0（失败）
u8 send_AT_cmd(char *AT_cmd,char *AT_ack,u16 waittime)
{
	u16 c=3;
	//最多重发3次
	while(c>0)
	{
		c--;
		//strcat(AT_cmd," \r\n");
		USART_SendBytess(USART2,AT_cmd);//发送AT指令
		USART_SendBytess(USART2,"\r\n");
		USART_DEBUG(AT_cmd);
		delay_ms(waittime);
		delay_ms(1000);		
		while(cntAt==0)				//接受缓冲区没数据延时2秒
		{
			delay_ms(1000);
			delay_ms(1000);
		}
		USART_DEBUG("\r\n");
		if(esp8266_check_cmd(AT_ack))
			{
					USART_SendBytess(USART1, "AT command ACK is Successfly\r\n");			//AT指令发送和确认成功
					return 1;
			}			
		}	
	USART_SendBytess(USART1,"3thd AT ACK error\r\n");			//3次AT指令发送失败
	return 0;
}


//ESP8266发送命令后,检测接收到的应答
//str:期待的应答结果
//返回值:0,没有得到期待的应答结果;其他,期待应答结果的位置(str的位置)
u8 esp8266_check_cmd(char *str)
{	
	char *strx=0;
	strx=strstr((const char*)ATBuffer,(const char*)str);
	USART_DEBUG("------------huan chong qu---------\r\n");
	USART_DEBUG(ATBuffer);
	USART_DEBUG("\r\n");
	memset(ATBuffer, 0, sizeof(ATBuffer)); 			//清空AT接受缓冲区
	cntAt=0;																		//复位缓冲区计数
	USART_DEBUG("-------------------------------\r\n");
	if(strx!=NULL)
		return 	1;
	else
		return	0;	
}


//
//主要功能：结束WiFi透传
//返回：1（成功）0：（失败）
//参数：无
u8 wifi_dis_trans(void)
{
	USART_SendBytess(USART2,"+++");//发送AT指令
	delay_ms(30);
	if(send_AT_cmd("AT","OK",50))
	{		
		USART_DEBUG("wifi trans disconnect\r\n");
		return 1;
	}
	else
	{		
		USART_DEBUG("dont disconnect wifi trans\r\n");
		return 0;
	}		
}



//函数功能：ESP8266向服务器发送数据
//参数：cmd:发送的数据字符串;waittime:等待时间(单位:ms)
//返回值:发送数据后，服务器的返回验证码
u8* esp8266_send_data(char *cmd,u16 waittime)
{
	char temp[5];
	char *ack=temp;
	USART_SendBytess(USART2,cmd);//发送AT指令
	delay_ms(waittime);
	if(USART_BufferRead((u8*)ack) == 0)
		{
			ack=(char*)ATBuffer;
			USART_DEBUG(ack);
			cntAt=0;						//复位缓冲区计数
		}
	return (u8*)ack;
} 


//函数功能：WiFi复位主要是重启和退出透传模式
//返回值：1（成功）0（失败）
//
u8 wifi_Reset(void)
{
	wifi_dis_trans();
	USART_SendBytess(USART1,"Get AT_ACK is overtime");			//AT指令返回失败
	USART_SendBytess(USART2,"AT+RST\r\n");//发送重启AT指令
	delay_ms(1000);         //延时5S等待重启成功
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	if(esp8266_check_cmd("ready")!=NULL)
		return 1;
	else 
		return 0;
}
