#include "bsp_adc.h"

static void RCC_ConfigInitail()
{
    ErrorStatus HSEStartUpStatus;
    FlagStatus Status;

    //RCC����
    RCC_DeInit();//����
    RCC_HSEConfig(RCC_HSE_ON);//�ⲿ8MHz����������
    HSEStartUpStatus = RCC_WaitForHSEStartUp();
    if(SUCCESS == HSEStartUpStatus)//�������ɹ�
    {
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_7);//56MHz PLL���
        RCC_PLLCmd(ENABLE);//PLL���ʹ��
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);//�ȴ�PLL����ɹ�  

        //����ϵͳʱ��56MHz
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
        while(0x08 != RCC_GetSYSCLKSource());//�ȴ����óɹ�       

        RCC_HCLKConfig(RCC_SYSCLK_Div1);
        RCC_PCLK2Config(RCC_HCLK_Div1);//PLCK2 56MHz
        RCC_PCLK1Config(RCC_HCLK_Div2);//PLCK1 28MHz 
        //ʹ��APB2����ʱ�� ADC & GPIOA
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE);
        RCC_ADCCLKConfig(RCC_PCLK2_Div4);//ADC1ʱ��Ƶ�� 14MHz
    }
}

void ADC1_config()
{
	ADC_InitTypeDef ADC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	//GPIO����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//GPIO����ģ������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��PA0��ʼ��
	//��Ϊ��ʹ�õ���ADC1_IN5����ͨ��0�������ӦSTM32C8T6��PA5��
	//���������ͺŵ�Ҫ���忴оƬ�ֲ�

	//ADC����
	ADC_DeInit(ADC1);//����

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//ADC1��ADC2��������������Ӱ��
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//ADC���β�����������һ�ξ�ֹͣ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;   //ADC��ͨ������(ENABLE�Ƕ�ͨ��ɨ��)
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//�������ADC
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;//ADCͨ��ת����Ŀ������ֻ��һ��ADC����ô����1
	//���ڶ�ͨ���ɼ���ʹ���ֵ >= 2�� ȡֵ��Χ��1~16

	ADC_Init(ADC1,&ADC_InitStructure);//��ʼ��
	ADC_Cmd(ADC1, ENABLE);//ʹ��

	//ADCУ׼
	ADC_ResetCalibration(ADC1);//����ADCУ׼��
	while(ADC_GetResetCalibrationStatus(ADC1));//�ȴ����ý���

	ADC_StartCalibration(ADC1);//��ʼУ׼
	while(ADC_GetCalibrationStatus(ADC1));//�ȴ�У׼���
//ADC_Channel_5��ͨ����ADC_SampleTime_1Cycles5���������ڵĲ���
//���ڼ��㣻������ADC_SampleTime_1Cycles5����1.5�����ڣ�����һ��12.5 + 1.5 = 14������ 
//ADCʱ��Ƶ��14KHZ / 14 = 1MHz���������Ǿ��ܵõ�1MHz���Ĳ�������
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_1Cycles5);

}


