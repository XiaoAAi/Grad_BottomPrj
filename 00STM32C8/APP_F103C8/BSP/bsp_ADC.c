#include "bsp_adc.h"

static void RCC_ConfigInitail()
{
    ErrorStatus HSEStartUpStatus;
    FlagStatus Status;

    //RCC配置
    RCC_DeInit();//重置
    RCC_HSEConfig(RCC_HSE_ON);//外部8MHz晶振启动！
    HSEStartUpStatus = RCC_WaitForHSEStartUp();
    if(SUCCESS == HSEStartUpStatus)//若启动成功
    {
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_7);//56MHz PLL输出
        RCC_PLLCmd(ENABLE);//PLL输出使能
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);//等待PLL输出成功  

        //设置系统时钟56MHz
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
        while(0x08 != RCC_GetSYSCLKSource());//等待设置成功       

        RCC_HCLKConfig(RCC_SYSCLK_Div1);
        RCC_PCLK2Config(RCC_HCLK_Div1);//PLCK2 56MHz
        RCC_PCLK1Config(RCC_HCLK_Div2);//PLCK1 28MHz 
        //使能APB2外设时钟 ADC & GPIOA
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE);
        RCC_ADCCLKConfig(RCC_PCLK2_Div4);//ADC1时钟频率 14MHz
    }
}

void ADC1_config()
{
	ADC_InitTypeDef ADC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	//GPIO配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//GPIO采用模拟输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);//对PA0初始化
	//因为我使用的是ADC1_IN5就是通道0，而这对应STM32C8T6的PA5口
	//对于其它型号的要具体看芯片手册

	//ADC配置
	ADC_DeInit(ADC1);//重置

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//ADC1和ADC2单独工作，互不影响
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//ADC单次采样，即采样一次就停止
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;   //ADC单通道采样(ENABLE是多通道扫描)
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//软件触发ADC
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;//ADC通道转换数目，我们只用一个ADC，那么就是1
	//对于多通道采集才使这个值 >= 2， 取值范围是1~16

	ADC_Init(ADC1,&ADC_InitStructure);//初始化
	ADC_Cmd(ADC1, ENABLE);//使能

	//ADC校准
	ADC_ResetCalibration(ADC1);//重置ADC校准器
	while(ADC_GetResetCalibrationStatus(ADC1));//等待重置结束

	ADC_StartCalibration(ADC1);//开始校准
	while(ADC_GetCalibrationStatus(ADC1));//等待校准完成
//ADC_Channel_5是通道，ADC_SampleTime_1Cycles5是设置周期的参数
//周期计算；如下面ADC_SampleTime_1Cycles5就是1.5个周期，所以一共12.5 + 1.5 = 14个周期 
//ADC时钟频率14KHZ / 14 = 1MHz，这样我们就能得到1MHz最大的采样率了
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_1Cycles5);

}


