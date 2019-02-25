#include "bsp_common.h"



void GPIO_Configure(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, ENABLE); //使能PB端口时钟
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //关闭JTAG因为要使用PB3和4
	
	//DEBUG调试灯
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOC, GPIO_Pin_13);

}

//功能：串口初始化程序
void USART_Configure(void)
{
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
#if USART1_CONFIG_ENABLED > 0       /* 串口配置使能判断，串口1 TX = PA9   RX = PA10 */
    /* 第1步：打开GPIO和USART部件的时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    /* 第2步：将USART Tx的GPIO配置为推挽输出模式 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    /* 第3步：将USART Rx的GPIO配置为浮空输入模式 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    // 第4步：USART 初始化设置
    USART_InitStructure.USART_BaudRate = USART1_BAUDRATE;//头文件宏定义，方便移植;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式
    USART_Init(USART1, &USART_InitStructure); //初始化串口
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
    USART_Cmd(USART1, ENABLE);                    //使能串口
    /* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
    如下语句解决第1个字节无法正确发送出去的问题 */
    USART_ClearFlag(USART1, USART_FLAG_TC);     /* 清除发送完成标志 */
#endif
#if USART2_CONFIG_ENABLED > 0                  /*串口配置使能判断， 串口2 TX = PA2， RX = PA3  */
    //使能GPIOA,复用功能以及USART2时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    //USART2_TX   PA.2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //USART2_RX   PA.3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //USART 初始化设置
    USART_InitStructure.USART_BaudRate = USART2_BAUDRATE;       //见宏定义;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式
    USART_Init(USART2, &USART_InitStructure); //初始化串口
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启中断
    USART_Cmd(USART2, ENABLE);
    USART_ClearFlag(USART2, USART_FLAG_TC);     /* 清除发送完成标志 */
#endif
#if USART3_CONFIG_ENABLED > 0                  /*串口配置使能判断， 串口3 TX = PB10， RX = PB11  */
    //使能GPIOB,复用功能以及USART3时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    //USART3_TX   PB.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB.10
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    //USART3_RX   PB.11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    //USART 初始化设置
    USART_InitStructure.USART_BaudRate = USART3_BAUDRATE;       //见宏定义;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; //字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式
    USART_Init(USART3, &USART_InitStructure); //初始化串口
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断
    USART_Cmd(USART3, ENABLE);
    USART_ClearFlag(USART3, USART_FLAG_TC);     /* 清除发送完成标志 */
#endif
}

//功能：工程所有中断优先级设置
void NVIC_Configure(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
//串口优先级设置
#if USART1_CONFIG_ENABLED > 0
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0 ; //抢占优先级0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;      //子优先级1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);                         //根据指定的参数初始化VIC寄存器
#endif
#if USART2_CONFIG_ENABLED > 0
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0 ; //抢占优先级0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;      //子优先级2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);                         //根据指定的参数初始化VIC寄存器
#endif
#if USART3_CONFIG_ENABLED > 0
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0 ; //抢占优先级0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;      //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);                         //根据指定的参数初始化VIC寄存器
#endif
//定时器中断优先级
#if TIM2_CONFIG_ENABLED > 0
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级1级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级3级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
#endif
#if TIM3_CONFIG_ENABLED > 0
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;             //TIM3中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   //先占优先级1级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;          //从优先级3级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
#endif
}








