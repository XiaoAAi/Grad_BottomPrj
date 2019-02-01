#include "bsp_common.h"

//红外接收引脚，被物体阻挡次数纪录
static u16 cnt_REV_Lx = 0;
//红外阻挡后接收到红外光次数纪录
static u16 cnt_HY_Lx = 0;
extern u8 Enable_LEVEL;
extern u8 flag_HY;      //红外对射功能选择位

//通用定时器2中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值；psc：时钟预分频数
void TIM2_Int_Init(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能
    TIM_DeInit(TIM2);
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
    static u16 led_num = 0;      //LED闪烁频率
    static u8 cycle_HY = 0;      //7ms循环计数，用于控制载波发射
    static bool flag_zudang = FALSE;    //阻挡标记位，阻挡到后为TRUE

    if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //检查指定的TIM中断发生与否:TIM 中断源
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);    //清除TIMx的中断待处理位:TIM 中断源
        led_num++;

        if(led_num == 500)  //500ms闪烁
        {
            LED_Green = !LED_Green;     //程序指示灯，绿灯闪烁

            if(Enable_LEVEL != 0)       //开启层对射检测，蓝灯闪烁
            {
                LED_Blue = !LED_Blue;
            }

            led_num = 0;
        }

        if(Enable_LEVEL != 0)       //上电默认，关闭对射的。TIM3也是关闭的
        {
            cycle_HY++;          //控制8ms循环，1ms载波，7ms低电平

            if(1 == cycle_HY)
            {
                TIM_Cmd(TIM3, ENABLE);  //使能TIM3，发射载波
            }
            else if(cycle_HY != 1)
            {
                TIM_Cmd(TIM3, DISABLE);  //关闭TIM3，不发射载波
                Led_Init_HY();  //红外发射引脚恢复到常态，常态：低电平
            }

            if(5 == cycle_HY)        //5ms一个循环
            {
                cycle_HY = 0;        //清零
            }

            switch(Enable_LEVEL)
            {
                case 1:
                    if(LED_RCV_L1 == RCV_38kHz)      //接收到38kHZ输出低电平，没有有物体挡住
                    {
                        if(flag_zudang == FALSE)
                        {
                            cnt_REV_Lx = 0;
                        }
                        else
                        {
                            cnt_HY_Lx++;    //阻挡后再次接收红外光计数
                        }

//                        USART_SendByte(USART1, 'Z');     //调试打印Z
                    }
                    else
                    {
                        if(flag_zudang == FALSE)
                        {
                            cnt_REV_Lx++;   //红外对射没有都到接收载波，所以有物体挡住
                        }

//                        USART_SendByte(USART1, 'A');     //调试打印A
                    }

                    break;

                case 2:
                    if(LED_RCV_L2 == RCV_38kHz)      //接收到38kHZ输出低电平，
                    {
                        if(flag_zudang == FALSE)
                        {
                            cnt_REV_Lx = 0;
                        }
                        else
                        {
                            cnt_HY_Lx++;    //阻挡后再次接收红外光计数
                        }

//                        USART_SendByte(USART1, 'Z');     //调试打印Z
                    }
                    else
                    {
                        if(flag_zudang == FALSE)
                        {
                            cnt_REV_Lx++;   //红外对射没有都到接收载波，所以有物体挡住
                        }

//                        USART_SendByte(USART1, 'B');     //调试打印B
                    }

                    break;

                case 3:
                    if(LED_RCV_L3 == RCV_38kHz)
                    {
                        if(flag_zudang == FALSE)
                        {
                            cnt_REV_Lx = 0;
                        }
                        else
                        {
                            cnt_HY_Lx++;    //阻挡后再次接收红外光计数
                        }

//                        USART_SendByte(USART1, 'Z');     //调试打印Z
                    }
                    else
                    {
                        if(flag_zudang == FALSE)
                        {
                            cnt_REV_Lx++;   //红外对射没有都到接收载波，所以有物体挡住
                        }

//                        USART_SendByte(USART1, 'C');     //调试打印C
                    }

                    break;

                case 4:
                    if(LED_RCV_L4 == RCV_38kHz)
                    {
                        if(flag_zudang == FALSE)
                        {
                            cnt_REV_Lx = 0;
                            USART_SendByte_Test(0xA1);
                        }
                        else
                        {
                            cnt_HY_Lx++;    //阻挡后再次接收红外光计数
                            USART_SendByte_Test(0xA2);
                        }

//                        USART_SendByte(USART1, 'Z');     //调试打印Z
                    }
                    else
                    {
                        if(flag_zudang == FALSE)
                        {
                            cnt_REV_Lx++;   //红外对射没有都到接收载波，所以有物体挡住
                            USART_SendByte_Test(0xA3);
                        }

//                        USART_SendByte(USART1, 'D');     //调试打印D
                    }

                    break;

                case 5:
                    if(LED_RCV_L5 == RCV_38kHz)
                    {
                        if(flag_zudang == FALSE)
                        {
                            cnt_REV_Lx = 0;
                        }
                        else
                        {
                            cnt_HY_Lx++;    //阻挡后再次接收红外光计数
                        }

//                        USART_SendByte(USART1, 'Z');     //调试打印Z
                    }
                    else
                    {
                        if(flag_zudang == FALSE)
                        {
                            cnt_REV_Lx++;   //红外对射没有都到接收载波，所以有物体挡住
                        }

//                        USART_SendByte(USART1, 'E');     //调试打印E
                    }

                    break;

                case 6:
                    if(LED_RCV_L6 == RCV_38kHz)
                    {
                        if(flag_zudang == FALSE)
                        {
                            cnt_REV_Lx = 0;
                        }
                        else
                        {
                            cnt_HY_Lx++;    //阻挡后再次接收红外光计数
                        }

//                        USART_SendByte(USART1, 'Z');     //调试打印Z
                    }
                    else
                    {
                        if(flag_zudang == FALSE)
                        {
                            cnt_REV_Lx++;   //红外对射没有都到接收载波，所以有物体挡住
                        }

//                        USART_SendByte(USART1, 'F');     //调试打印F
                    }

                    break;

                case 7:
                    if(LED_RCV_L7 == RCV_38kHz)
                    {
                        if(flag_zudang == FALSE)
                        {
                            cnt_REV_Lx = 0;
                        }
                        else
                        {
                            cnt_HY_Lx++;    //阻挡后再次接收红外光计数
                        }

//                        USART_SendByte(USART1, 'Z');     //调试打印Z
                    }
                    else
                    {
                        if(flag_zudang == FALSE)
                        {
                            cnt_REV_Lx++;   //红外对射没有都到接收载波，所以有物体挡住
                        }

//                        USART_SendByte(USART1, 'C');     //调试打印C
                    }

                    break;

                case 8:
                    if(LED_RCV_L8 == RCV_38kHz)
                    {
                        if(flag_zudang == FALSE)
                        {
                            cnt_REV_Lx = 0;
                        }
                        else
                        {
                            cnt_HY_Lx++;    //阻挡后再次接收红外光计数
                        }

//                        USART_SendByte(USART1, 'Z');     //调试打印Z
                    }
                    else
                    {
                        if(flag_zudang == FALSE)
                        {
                            cnt_REV_Lx++;   //红外对射没有都到接收载波，所以有物体挡住
                        }

//                        USART_SendByte(USART1, 'D');     //调试打印D
                    }

                    break;

                case 9:
                    if(LED_RCV_L9 == RCV_38kHz)
                    {
                        if(flag_zudang == FALSE)
                        {
                            cnt_REV_Lx = 0;
                        }
                        else
                        {
                            cnt_HY_Lx++;    //阻挡后再次接收红外光计数
                        }

//                        USART_SendByte(USART1, 'Z');     //调试打印Z
                    }
                    else
                    {
                        if(flag_zudang == FALSE)
                        {
                            cnt_REV_Lx++;   //红外对射没有都到接收载波，所以有物体挡住
                        }

//                        USART_SendByte(USART1, 'E');     //调试打印E
                    }

                    break;

                case 10:
                    if(LED_RCV_L10 == RCV_38kHz)
                    {
                        if(flag_zudang == FALSE)
                        {
                            cnt_REV_Lx = 0;
                        }
                        else
                        {
                            cnt_HY_Lx++;    //阻挡后再次接收红外光计数
                        }

//                        USART_SendByte(USART1, 'Z');     //调试打印Z
                    }
                    else
                    {
                        if(flag_zudang == FALSE)
                        {
                            cnt_REV_Lx++;   //红外对射没有都到接收载波，所以有物体挡住
                        }

//                        USART_SendByte(USART1, 'F');     //调试打印F
                    }

                    break;

                default:
                    break;
            }

            //连续20ms有物体挡住，判定物体遮挡，必须连续
            if((cnt_REV_Lx > NUM_ZHEDANG) && (flag_zudang == FALSE))
            {
                cnt_REV_Lx = 0;     //纪录物体挡住次数清零
                cnt_HY_Lx = 0;      //阻挡后接收到红外光计数

                if(flag_HY == 0xAA)     //阻挡就发送反馈
                {
                    switch(Enable_LEVEL)
                    {
                        case 1:
                            Send_CMD(USART2, HBYTE(DUISHE_DIANJI_NUMb1), LBYTE(DUISHE_DIANJI_NUMb1));   //第1层反馈
                            USART_SendByte(USART1, 0x31);     //调试打印1
                            break;

                        case 2:
                            Send_CMD(USART2, HBYTE(DUISHE_DIANJI_NUMb2), LBYTE(DUISHE_DIANJI_NUMb2));   //第2层反馈
                            USART_SendByte(USART1, 0x32);     //调试打印2
                            break;

                        case 3:
                            Send_CMD(USART2, HBYTE(DUISHE_DIANJI_NUMb3), LBYTE(DUISHE_DIANJI_NUMb3));   //第3层反馈
                            USART_SendByte(USART1, 0X33);     //调试打印3
                            break;

                        case 4:
                            Send_CMD(USART2, HBYTE(DUISHE_DIANJI_NUMb4), LBYTE(DUISHE_DIANJI_NUMb4));   //第4层反馈
                            USART_SendByte(USART1, 0X34);     //调试打印4
                            USART_SendByte_Test(0xAF);
                            break;

                        case 5:
                            Send_CMD(USART2, HBYTE(DUISHE_DIANJI_NUMb5), LBYTE(DUISHE_DIANJI_NUMb5));   //第5层反馈
                            USART_SendByte(USART1, 0X35);     //调试打印5
                            break;

                        case 6:
                            Send_CMD(USART2, HBYTE(DUISHE_DIANJI_NUMb6), LBYTE(DUISHE_DIANJI_NUMb6));   //第6层反馈
                            USART_SendByte(USART1, 0X36);     //调试打印6
                            break;

                        case 7:
                            Send_CMD(USART2, HBYTE(DUISHE_DIANJI_NUMb7), LBYTE(DUISHE_DIANJI_NUMb7));   //第6层反馈
                            USART_SendByte(USART1, 0X37);     //调试打印7
                            break;

                        case 8:
                            Send_CMD(USART2, HBYTE(DUISHE_DIANJI_NUMb8), LBYTE(DUISHE_DIANJI_NUMb8));   //第6层反馈
                            USART_SendByte(USART1, 0X38);     //调试打印8
                            break;

                        case 9:
                            Send_CMD(USART2, HBYTE(DUISHE_DIANJI_NUMb9), LBYTE(DUISHE_DIANJI_NUMb9));   //第6层反馈
                            USART_SendByte(USART1, 0X39);     //调试打印9
                            break;

                        case 10:
                            Send_CMD(USART2, HBYTE(DUISHE_DIANJI_NUMb10), LBYTE(DUISHE_DIANJI_NUMb10));   //第6层反馈
                            USART_SendByte(USART1, 0X40);     //调试打印10
                            break;

                        default:
                            break;
                    }

                    Enable_LEVEL = 0;   //清零
                    Led_Init_HY();  //红外发射引脚恢复到常态，常态：低电平
                }
                else if(flag_HY == 0x55)       //阻挡后再次接收到红外光发送反馈
                {
                    flag_zudang = TRUE;     //阻挡到标记
                    USART_SendByte(USART1, 'W');     //运动到某一层，第一次遮挡住标记
                    USART_SendByte_Test(0xA5);
                }

                TIM_Cmd(TIM3,   DISABLE);       //关闭TIM3
                cycle_HY = 0;    //8s循环回到起始
            }

            if(flag_HY == 0x55)        //阻挡后再次接收到红外光发送反馈
            {
                if((cnt_HY_Lx >= 6) && (flag_zudang == TRUE))     //阻挡后连续20ms接收到红外光
                {
                    switch(Enable_LEVEL)
                    {
                        case 1:
                            Send_CMD(USART2, HBYTE(DUISHE_DIANJI_NUMb1), LBYTE(DUISHE_DIANJI_NUMb1));   //第1层反馈
                            USART_SendByte(USART1, 0x31);     //调试打印1
                            break;

                        case 2:
                            Send_CMD(USART2, HBYTE(DUISHE_DIANJI_NUMb2), LBYTE(DUISHE_DIANJI_NUMb2));   //第2层反馈
                            USART_SendByte(USART1, 0x32);     //调试打印2
                            break;

                        case 3:
                            Send_CMD(USART2, HBYTE(DUISHE_DIANJI_NUMb3), LBYTE(DUISHE_DIANJI_NUMb3));   //第3层反馈
                            USART_SendByte(USART1, 0X33);     //调试打印3
                            break;

                        case 4:
                            Send_CMD(USART2, HBYTE(DUISHE_DIANJI_NUMb4), LBYTE(DUISHE_DIANJI_NUMb4));   //第4层反馈
                            USART_SendByte(USART1, 0X34);     //调试打印4
                            USART_SendByte_Test(0xAA);
                            break;

                        case 5:
                            Send_CMD(USART2, HBYTE(DUISHE_DIANJI_NUMb5), LBYTE(DUISHE_DIANJI_NUMb5));   //第5层反馈
                            USART_SendByte(USART1, 0X35);     //调试打印5
                            break;

                        case 6:
                            Send_CMD(USART2, HBYTE(DUISHE_DIANJI_NUMb6), LBYTE(DUISHE_DIANJI_NUMb6));   //第6层反馈
                            USART_SendByte(USART1, 0X36);     //调试打印6
                            break;

                        case 7:
                            Send_CMD(USART2, HBYTE(DUISHE_DIANJI_NUMb7), LBYTE(DUISHE_DIANJI_NUMb7));   //第6层反馈
                            USART_SendByte(USART1, 0X37);     //调试打印7
                            break;

                        case 8:
                            Send_CMD(USART2, HBYTE(DUISHE_DIANJI_NUMb8), LBYTE(DUISHE_DIANJI_NUMb8));   //第6层反馈
                            USART_SendByte(USART1, 0X38);     //调试打印8
                            break;

                        case 9:
                            Send_CMD(USART2, HBYTE(DUISHE_DIANJI_NUMb9), LBYTE(DUISHE_DIANJI_NUMb9));   //第6层反馈
                            USART_SendByte(USART1, 0X39);     //调试打印9
                            break;

                        case 10:
                            Send_CMD(USART2, HBYTE(DUISHE_DIANJI_NUMb10), LBYTE(DUISHE_DIANJI_NUMb10));   //第6层反馈
                            USART_SendByte(USART1, 0X40);     //调试打印10
                            break;

                        default:
                            break;
                    }

                    Enable_LEVEL = 0;   //清零
                    Led_Init_HY();  //红外发射引脚恢复到常态，常态：低电平
                    flag_zudang = FALSE;     //阻挡到标记清零
                    cnt_REV_Lx = 0;     //纪录物体挡住次数清零
                    cnt_HY_Lx = 0;      //阻挡后接收到红外光计数
                    TIM_Cmd(TIM3,   DISABLE);       //关闭TIM3
                    cycle_HY = 0;    //8s循环回到起始
                }
            }
        }
    }
}

//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值；psc：时钟预分频数
void TIM3_Int_Init(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
    TIM_DeInit(TIM3);                       //重设为缺省值，可以看作时钟重启
    TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值  计数到5000为500ms
    TIM_TimeBaseStructure.TIM_Prescaler = psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); //使能指定的TIM3中断,允许更新中断
//    TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设
    TIM_Cmd(TIM3,   DISABLE);  //初始化不使能TIMx外设
}

//功能：定时器3中断服务程序
void TIM3_IRQHandler(void)   //TIM3中断
{
    if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查指定的TIM中断发生与否:TIM 中断源
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源

        switch(Enable_LEVEL)    //对应层38KHz载波发射
        {
            case 1:
                LED_SEND_L1 = !LED_SEND_L1;     //层红外发射引脚，默认都是高电平
                break;

            case 2:
                LED_SEND_L2 = !LED_SEND_L2;
                break;

            case 3:
                LED_SEND_L3 = !LED_SEND_L3;
                break;

            case 4:
                LED_SEND_L4 = !LED_SEND_L4;
                break;

            case 5:
                LED_SEND_L5 = !LED_SEND_L5;
                break;

            case 6:
                LED_SEND_L6 = !LED_SEND_L6;
                break;

            case 7:
                LED_SEND_L7 = !LED_SEND_L7;
                break;

            case 8:
                LED_SEND_L8 = !LED_SEND_L8;
                break;

            case 9:
                LED_SEND_L9 = !LED_SEND_L9;
                break;

            case 10:
                LED_SEND_L10 = !LED_SEND_L10;
                break;

            default:
                break;
        }
    }
}

//TIM3 PWM部分初始化
//arr：自动重装值；psc：时钟预分频数
void TIM3_PWM_Init(u16 arr, u16 psc)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);    //使能定时器3时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA  | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3部分重映射  TIM3_CH2->PB5
    //设置该引脚为复用输出功能,输出TIM3 CH2的PWM脉冲波形 GPIOB.5
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //TIM_CH2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO
    //初始化TIM3
    TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
    TIM_TimeBaseStructure.TIM_Prescaler = psc; //设置用来作为TIMx时钟频率除数的预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
    //初始化TIM3 Channel2 PWM模式
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
    TIM_Cmd(TIM3, ENABLE);  //使能TIM3
}



