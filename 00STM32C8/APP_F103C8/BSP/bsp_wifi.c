#include "bsp_common.h"



extern u8 UsartBuffer[USART_BUFFER_LEN]; //数据缓冲区

u8 send_AT_cmd(char *AT_cmd,char *AT_ack,u16 waittime)
{
	u8 rs_data;

	//最多重发3次
	for(u8 c=3;c>0;c--)
	{
		//c--;
		USART_SendBytess(USART2,AT_cmd);//发送AT指令
		delay_ms(waittime);
		delay_ms(1000);
		USART_SendByte(USART1,rs_data);
		while(!USART_BufferRead(&rs_data))
		{
			int i=1;
			delay_ms(1);
			i++;
			if(i>10)
			{
				USART_SendBytess(USART1,"Get AT_ACK is overtime");			//AT指令返回失败
				break;
			}
		}
		USART_SendBytess(USART1,"jie shou dao zhi lin\r\n");
		
		if(esp8266_check_cmd(AT_ack)!=NULL)
		{
			USART_SendBytess(USART1, "AT command ACK is Successfly");			//AT指令发送和确认成功
			return 1;
		}
		
		USART_SendBytess(USART1,"-------\r\n");
	}		
	USART_SendBytess(USART1,"3thd AT ACK error\r\n");			//3次AT指令发送失败
//	char *error_AT="111";
//	sprintf(error_AT,"ther error AT is %s\r\n",AT_cmd);
//	USART_SendBytess(USART1,"++++++++");
//	USART_SendBytess(USART1,error_AT);//打印错误的AT指令
	return 0;
}


//ESP8266发送命令后,检测接收到的应答
//str:期待的应答结果
//返回值:0,没有得到期待的应答结果;其他,期待应答结果的位置(str的位置)
u8* esp8266_check_cmd(char *str)
{
	char *strx=0;
//	if(USART3_RX_STA&0X8000)		//接收到一次数据了
//	{ 
		//USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//添加结束符
		strx=strstr((const char*)UsartBuffer,(const char*)str);
	//} 
	return (u8*)strx;
}


//
//ESP8266模块和PC进入透传模式
//
void wifi_start_trans(void)
{
	//设置工作模式 1：station模式   2：AP模式  3：兼容 AP+station模式
	send_AT_cmd("AT+CWMODE=1","OK",50);
	USART_SendBytess(USART1,"AT+CWMODE=1 jie shu\r\n");
	
	//让Wifi模块重启的命令
	send_AT_cmd("AT+RST","ready",20);
	
	delay_ms(1000);         //延时5S等待重启成功
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	
	//让模块连接上自己的路由
	while(send_AT_cmd("AT+CWJAP=\"ZNHL\",\"ZnHl@4501.\"","OK",200));
	
	//=0：单路连接模式     =1：多路连接模式
	send_AT_cmd("AT+CIPMUX=0","OK",20);
	
	//建立TCP连接  这四项分别代表了 要连接的ID号0~4   连接类型  远程服务器IP地址   远程服务器端口号
	while(send_AT_cmd("AT+CIPSTART=\"TCP\",\"192.168.3.202\",8580","CONNECT",200));
	
	//是否开启透传模式  0：表示关闭 1：表示开启透传
	send_AT_cmd("AT+CIPMODE=1","OK",200);
	
	//透传模式下 开始发送数据的指令 这个指令之后就可以直接发数据了
	send_AT_cmd("AT+CIPSEND","OK",50);
}



