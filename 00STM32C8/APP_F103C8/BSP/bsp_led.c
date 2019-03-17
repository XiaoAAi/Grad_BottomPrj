#include "bsp_common.h"


void RCC_congig(void)
{
//	SystemInit（）;

//	RCC_APB2PeriphClockCmd（RCC_APB2Periph_GPIOA，ENABLE）;

//	RCC_APB1PeriphClockCmd（RCC_APB1Periph_TIM2，ENABLE）;

//	//GPIO_AFIODeInit（）;和下面相同效果，可以查看源码证明

//	RCC_APB2PeriphClockCmd（RCC_APB2Periph_AFIO，ENABLE）;
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //使能定时器 3 时钟 
	
}

//引脚初始化使用引脚PA2
void GPIO_Configuration(void)

{

	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;

	GPIO_Init(GPIOA, &GPIO_InitStructure);

}


void TIM2_Configuration(void)
{

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	TIM_OCInitTypeDef TIM_OCInitStructure;

	TIM_DeInit(TIM2);

	TIM_InternalClockConfig(TIM2);

	//定时器初始化

	TIM_TimeBaseStructure.TIM_Period = 100-1;//初始值是100，2MHz=2000KHz，2MHz/100=0.02MHz，
																						//所有周期T=1/0.02MHz=50us，示波器显示周期应该为50us

	TIM_TimeBaseStructure.TIM_Prescaler = 36-1;//36倍分频，72MHz/36=2MHz

	TIM_TimeBaseStructure.TIM_ClockDivision = 0;

	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	//PWM???

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//小于比较器设定值时输出低电平

	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;

	TIM_OCInitStructure.TIM_Pulse = 50;//占空比值是50，所以占空比是：50/100=50%

TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;//起始波形为低电平

	TIM_OC3Init(TIM2,&TIM_OCInitStructure);

	TIM_OCInitStructure.TIM_Pulse = 20;

	TIM_OC4Init(TIM2,&TIM_OCInitStructure);

	//TIM_OC3PreloadConfig(TIM2,TIM_OCPreload_Enable);

	//TIM_ARRPreloadConfig(TIM2, ENABLE);

	TIM_Cmd(TIM2,ENABLE);

	TIM_CtrlPWMOutputs(TIM2,ENABLE);

}

void TIM2_PWM_init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; 
	TIM_OCInitTypeDef  TIM_OCInitStructure; 
 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  //①使能定时器 3 时钟 
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB| RCC_APB2Periph_AFIO, ENABLE);            //①使能 GPIO 和 AFIO 复用功能时钟 
 
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //②重映射 TIM3_CH2->PB5     
 
 //设置该引脚为复用输出功能,输出 TIM3 CH2 的 PWM 脉冲波形 GPIOB.5 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;             //TIM_CH2 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    //复用推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);                //①初始化 GPIO    
	//初始化 TIM3 
	TIM_TimeBaseStructure.TIM_Period = arr;            //设置在自动重装载周期值 
	TIM_TimeBaseStructure.TIM_Prescaler =psc;          //设置预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM 向上计数模式 
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);   //③初始化 TIMx    
	//初始化 TIM3 Channel2 PWM 模式   
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;     //选择 PWM 模式 2 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //输出极性高 
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);                 //④初始化外设 TIM3 OC2 
 
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能预装载寄存器 TIM_Cmd(TIM3, ENABLE);                          //⑤使能 TIM3 
}

