#include "bsp_common.h"


extern u16 UsartWptr;
extern u8 UsartBuffer[USART_BUFFER_LEN]; 	//数据缓冲区
u8* esp8266_check_cmd(char *str);			//检查返回的字符是否正确

u8 send_AT_cmd(char *AT_cmd,char *AT_ack,u16 waittime)
{
	//USART_DEBUG(ATBuffer);
	u8 rs_data;
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
		if(USART_BufferRead(&rs_data) == 0)
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
				break;
			
		}
		else
			{
							
					USART_DEBUG("\r\n");
				//USART_DEBUG("15454545\r\n");
				if(esp8266_check_cmd(AT_ack)!=NULL)
				{
					USART_SendBytess(USART1, "AT command ACK is Successfly\r\n");			//AT指令发送和确认成功
					USART_DEBUG("****************");
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
	char *strx=0;
	strx=strstr((const char*)UsartBuffer,(const char*)str);
	USART_DEBUG("------------huan chong qu---------\r\n");
	USART_DEBUG((char *)UsartBuffer);
	USART_DEBUG("\r\n");
	//UsartWptr=0;
	memset(UsartBuffer, 0, 4000); 
	USART_DEBUG("-------------------------------\r\n");
	USART_DEBUG("=============\r\n");
	USART_DEBUG(strx);
	USART_DEBUG("=============\r\n");
	return (u8*)strx;
}



//结束透传
void wifi_dis_trans(void)
{
	USART_SendBytess(USART2,"+++");//发送AT指令
	delay_ms(30);
	if(send_AT_cmd("AT","OK",50))
		USART_DEBUG("wifi trans disconnect");
	else
		USART_DEBUG("dont disconnect wifi trans");
}



//
//ESP8266模块和PC进入透传模式
//
void wifi_start_trans(void)
{
	//设置工作模式 1：station模式   2：AP模式  3：兼容 AP+station模式
	send_AT_cmd("AT+CWMODE=1\r\n","OK",50);
	USART_SendBytess(USART1,"AT+CWMODE=1 jie shu\r\n");
//	memset(ATBuffer, 0, 1000); 

//	//让Wifi模块重启的命令
//	send_AT_cmd("AT+RST","ready",20);
//	

	
	//让模块连接上自己的路由
	send_AT_cmd("AT+CWJAP=\"ZNHL\",\"ZnHl@4501.\"","WIFI CONNECTED",1000);
	
	//=0：单路连接模式     =1：多路连接模式
	send_AT_cmd("AT+CIPMUX=0","OK",20);
	
	//建立TCP连接  这四项分别代表了 要连接的ID号0~4   连接类型  远程服务器IP地址   远程服务器端口号
	send_AT_cmd("AT+CIPSTART=\"TCP\",\"192.168.3.202\",8580","CONNECT",200);
	
	//是否开启透传模式  0：表示关闭 1：表示开启透传
	send_AT_cmd("AT+CIPMODE=1","OK",200);
	
	//透传模式下 开始发送数据的指令 这个指令之后就可以直接发数据了
	send_AT_cmd("AT+CIPSEND","OK",50);
		//测试信息
		send_AT_cmd("hello xiao na: fang ge pi","OK",50);
	wifi_dis_trans();
}

//向ESP8266发送数据
//cmd:发送的命令字符串;waittime:等待时间(单位:10ms)
//返回值:发送数据后，服务器的返回验证码
u8* esp8266_send_data(char *cmd,u16 waittime)
{
	char temp[5];
	char *ack=temp;
	//USART3_RX_STA=0;
	USART_SendBytess(USART2,cmd);//发送AT指令
	if(waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			delay_ms(10);
//			if(USART3_RX_STA&0X8000)//接收到期待的应答结果
//			{
//				USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//添加结束符
				ack=(char*)UsartBuffer;
				printf("ack:%s\r\n",(u8*)ack);
				//USART3_RX_STA=0;
				break;//得到有效数据 
			//} 
		}
	}
	return (u8*)ack;
} 

