================================================================================
                                样例使用说明
================================================================================
功能描述：
此样例演示了TIM1的单脉冲模式，CH2(PA13)引脚上的上升沿，触发计数器开始计数，当计数
值与CCR1匹配时，CH1(PA3)输出高电平，直到计数器溢出，CH1再次输出低电平，计数器溢
出后，定时器停止工作。

本例程脉冲宽度计算 (TIM1_ARR-TI1_CCR1)/CLK=（65535-16383）/16000000=3.072ms

================================================================================
测试环境：
测试用板：PY32F003_STK
MDK版本： 5.28
IAR版本： 9.20
================================================================================
使用步骤：
1. 编译下载程序到MCU，并运行；
2. 按下按键后，在PA13上产生一个上升沿；
3. 通过示波器监测PA13和PA3引脚，可监测到上述波形；

================================================================================
注意事项：
时钟主频为16M
因为PA13（默认为SWD_DIO）引脚复用为CH2，如果要重新烧录程序需要复位后再下载
输出效果见下图：
                                ____
                                |   |
   CH2 _________________________|   |_________________________________________
   
                                              ___________________________
                                             |                           |
   CH1 ______________________________________|                           |____
                                <---Delay----><------Pulse--------------->
================================================================================