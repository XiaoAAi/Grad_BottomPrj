#include "bsp_common.h"

u16 led_falg=10000;

void GPIO_Configure(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, ENABLE); //使能ABCDE端口时钟
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //关闭JTAG
	
	//DEBUG调试灯
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOC, GPIO_Pin_13);
	
	//DHT22温湿度传感器引脚B8
	//OLED引脚配置
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15|GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15|GPIO_Pin_8);//设置为高电平
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_7;//A8引脚用于OLED
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_8);	
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;//A8引脚用于OLED屏幕，A7用于人体传感器指示灯
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//GPIO_ResetBits(GPIOA,GPIO_Pin_7);	
	
}

void OLED_SpiInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
	//oled数据线和时钟线初始化
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_12|GPIO_Pin_13);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;       //设置SPI工作模式:设置为主SPI
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;       //设置SPI的数据大小:SPI发送接收8位帧结构
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;     //选择了串行时钟的稳态:时钟悬空高
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;    //数据捕获(采样)于第二个时钟沿
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;       //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制		
//  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;        //定义波特率预分频的值:波特率预分频值为256
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;  //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
  SPI_InitStructure.SPI_CRCPolynomial = 7;    //CRC值计算的多项式
//  SPI_Init(SPI2, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16 ;		//设置为18M时钟,高速模式
  SPI_Init(SPI2, &SPI_InitStructure);
  SPI_Cmd(SPI2, ENABLE); //使能SPI外设
	
}
void GPIOB_set(uint16_t pin,u8 state)
{
	if(state==1)
		GPIO_SetBits(GPIOB,pin);//设置为高电平
	else
		if(state==0)
			GPIO_ResetBits(GPIOB,pin);//设置为低电平
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

#if EXTI_CONFIG_ENABLED > 0
    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;             //A4中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   //先占优先级1级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;          //从优先级3级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
#endif
}

void EXTIX_Init(void)
{
	EXTI_InitTypeDef EXTI_initStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//使能AFIO复用时钟功能：	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource4);
	EXTI_initStructure.EXTI_Line=EXTI_Line4;
	EXTI_initStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断模式，可选值为中断 EXTI_Mode_Interrupt 和事件 EXTI_Mode_Event。
	EXTI_initStructure.EXTI_Trigger = EXTI_Trigger_Rising;//触发方式，可以是下降沿触发 EXTI_Trigger_Falling，上升沿触发 EXTI_Trigger_Rising，或者任意电平（上升沿和下降沿）触发EXTI_Trigger_Rising_Falling
	EXTI_initStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_initStructure);//根据结构体信息进行初始化
	
}
//外部引脚A8中断服务例程
void EXTI4_IRQHandler(void)
{
	//delay_ms(1000);
  //USART_DEBUG("ren ti chuan gan qi zhongduan\r\n");
		PAout(7)=1;
		//led_falg=10000;
    EXTI_ClearITPendingBit(EXTI_Line4);//清楚中断标志
}



