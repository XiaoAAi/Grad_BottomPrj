#include "bsp_timer.h"

extern u8 Lock_flag;

#if TIM2_CONFIG_ENABLED
void TIM2_Int_Init(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能
    TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值  计数到5000为500ms
    TIM_TimeBaseStructure.TIM_Prescaler = psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); //使能指定的TIM3中断,允许更新中断
	
    TIM_Cmd(TIM2, ENABLE);  //使能TIMx外设
}

//功能：定时器2中断服务程序
void TIM2_IRQHandler(void)   //TIM3中断
{
    if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //检查指定的TIM中断发生与否:TIM 中断源
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);    //清除TIMx的中断待处理位:TIM 中断源
	}
}


#endif

#if  TIM3_CONFIG_ENABLED
u8 cntDebugLed = 0,cnt=0;
u16 cntHuman_light=0;
u16 cnt_date_time=0;
u8 cnt_time=0;
u8 cnt_Lock=0;//为门锁计时
static u16 Humidity=0,Tempreture=0;

void TIM3_Int_Init(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    //初始化TIM3
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
    TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值     计数到5000为500ms
    TIM_TimeBaseStructure.TIM_Prescaler = psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); //使能指定的TIM3中断,允许更新中断
	
	TIM_Cmd(TIM3, ENABLE);  

}


//功能：定时器3中断服务程序
void TIM3_IRQHandler(void)   //TIM3中断
{
    if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查指定的TIM中断发生与否:TIM 中断源
    {
		if(cnt_time++>10)	//1秒
		{
			Oled_ShowTime();
			cnt_time=0;	
			cnt%=30;			
			if(cnt==0)
			{
				u8 dht22data[5]={0};
				DHT22_Read_Data(&Humidity,&Tempreture);
				oled_DHT22(Humidity,Tempreture);		//温湿度要除以10才是正确值
				dht22data[0]=HBYTE(Humidity);
				dht22data[1]=LBYTE(Humidity);
				dht22data[2]=HBYTE(Tempreture);
				dht22data[3]=LBYTE(Tempreture);
				SendCmdDat(USART2,USART_BUTTOM_SERVER_SendHeartbeat,(char*)dht22data,4);	//心跳包
			}
			cnt++;
			if(Lock_flag)
			{
				cnt_Lock++;
				if(cnt_Lock>5)
				{
					char send_cmd=0xbb;
					cnt_Lock=0;
					Lock_flag=0;
					DoorLockClose;
					SendCmdDat(USART2,USART_BUTTOM_SERVER_LockFeedback,&send_cmd,1);
				}
			}
				
		}
		//调试灯
		if(cntDebugLed++ > 5)
		{
			cntDebugLed = 0;
			LED_SWITCH();			
		}
		//人体检测灯
		if(cntHuman_light++ > 30)//3秒
		{
			cntHuman_light = 0;
			Human_body_Light_OFF;			
		}
		if(cnt_date_time++ >600)//1分钟
		{			
			SendCmd(USART2,USART_BUTTOM_SERVER_Date);//获取时间
			cnt_date_time=0;
		}
		
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);    //清除TIMx的中断待处理位:TIM 中断源
	}
}

#endif

