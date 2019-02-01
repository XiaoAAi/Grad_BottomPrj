#include "bsp_common.h"


//功能：相关引脚初始化()
//A6 A7 C4 C5 B0 B1 B10 B11 B12 B13(设置为红外发射引脚)
//C0：红灯  C1：蓝灯  C2：绿灯  C6:掉货使能
//C7 C8 C9 :功率设置引脚
//上拉输入： C3
//相关浮空输入的引脚可以不定义
void GPIO_Configure(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, ENABLE); //使能PB端口时钟
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //关闭JTAG因为要使用PB3和4
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1  | GPIO_Pin_2 | GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;         //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;        //IO口速度为50MHz
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_SetBits(GPIOC, GPIO_Pin_0);                     //LED_RED，常态高电平，灯灭
    GPIO_SetBits(GPIOC, GPIO_Pin_1);                     //LED_BLUE，常态高电平，灯灭
    GPIO_SetBits(GPIOC, GPIO_Pin_2);                     //LED_GREEN，常态高电平，灯灭
    GPIO_SetBits(GPIOC, GPIO_Pin_6);                     //掉货使能，常态高电平，停止检测
    //红外发送功能 功率设置引脚 初始化
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8  | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;         //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;        //IO口速度为50MHz
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_SetBits(GPIOC, GPIO_Pin_7);                     //PB.4 输出1
    GPIO_SetBits(GPIOC, GPIO_Pin_8);                     //PB.4 输出1
    GPIO_SetBits(GPIOC, GPIO_Pin_9);                     //PB.4 输出1
    //红外发送功能 层发送引脚 初始化 目前只用到六层
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;         //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;        //IO口速度为50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOA, GPIO_Pin_6 | GPIO_Pin_7);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;         //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;        //IO口速度为50MHz
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOC, GPIO_Pin_4 | GPIO_Pin_5);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;         //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;        //IO口速度为50MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13);                     //PB.4 输出1
    //调试按键KEY
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;//KEY0-KEY2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
    GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOB12,13
//功能：不需要设置浮空输入，打开时钟门后，相应端口默认为浮空输入。
#if 0
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;//KEY0-KEY2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
    GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIOB12,13
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15 | GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //PA0设置成输入，默认上拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.8
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //PA0设置成输入，默认上拉
    GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOA.8
#endif
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
#if UART4_CONFIG_ENABLED > 0                  /*串口配置使能判断， 串口4 TX = PC10， RX = PC11  */
    //使能GPIOB,复用功能以及USART3时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
    //USART4_TX   PC.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PC.10
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    //USART4_RX   PC.11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    //USART 初始化设置
    USART_InitStructure.USART_BaudRate = UART4_BAUDRATE;       //见宏定义;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; //字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式
    USART_Init(UART4, &USART_InitStructure); //初始化串口
    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//开启中断
    USART_Cmd(UART4, ENABLE);
    USART_ClearFlag(UART5, USART_FLAG_TC);     /* 清除发送完成标志 */
#endif
#if UART5_CONFIG_ENABLED > 0                  /*串口配置使能判断， 串口5 TX = PC12， RX = PD2  */
    //使能GPIOB,复用功能以及USART3时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
    //USART5_TX   PC.12
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //PC.12
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    //USART5_RX   PD.2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    //USART 初始化设置
    USART_InitStructure.USART_BaudRate = UART5_BAUDRATE;       //见宏定义;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; //字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式
    USART_Init(UART5, &USART_InitStructure); //初始化串口
    USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//开启中断
    USART_Cmd(UART5, ENABLE);
    USART_ClearFlag(UART5, USART_FLAG_TC);     /* 清除发送完成标志 */
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
#if UART4_CONFIG_ENABLED > 0
    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0 ; //抢占优先级0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;      //子优先级4
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);                         //根据指定的参数初始化VIC寄存器
#endif
#if UART5_CONFIG_ENABLED > 0
    NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0 ; //抢占优先级0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;      //子优先级5
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);                         //根据指定的参数初始化VIC寄存器
#endif
//定时器中断优先级
#if TIM2_CONFIG_ENABLED > 0
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级1级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
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


//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下;1，KEY0按下
u8 KEY_Scan(u8 mode)
{
    static u8 key_up = 1; //按键按松开标志

    if(mode)
        key_up = 1; //支持连按

    if(key_up && (KEY0 == 0))
    {
        delay_ms(10);//去抖动
        key_up = 0;

        if(KEY0 == 0)
            return KEY0_PRES;
    }
    else if(KEY0 == 1)
        key_up = 1;

    return 0;// 无按键按下
}

//功能：红外发射功率设置
//说明：参考EXECL文档，设置红外发射功率。
void LED_Power_ctrl(void)
{
    //发射功率设置，不能全部设置为0
    LED_Power1 = 1;      //发送功率设置1，常态为高电平，配置为低电平，500mA工作
    LED_Power2 = 1;      //发送功率设置2，常态为高电平，配置为低电平，250mA工作
    LED_Power3 = 0;      //发送功率设置3，常态为高电平，配置为低电平，100mA工作
}

//功能：红外发射引脚关闭
//说明：红外发射引脚恢复到默认的低电平状态
void Led_Init_HY(void)
{
    LED_SEND_L1 = 0;          //红外发射引脚，常态：低电平
    LED_SEND_L2 = 0;
    LED_SEND_L3 = 0;
    LED_SEND_L4 = 0;
    LED_SEND_L5 = 0;
    LED_SEND_L6 = 0;
}




