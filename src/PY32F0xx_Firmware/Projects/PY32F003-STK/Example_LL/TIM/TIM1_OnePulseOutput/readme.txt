================================================================================
                                样例使用说明
================================================================================
功能描述：
此样例演示了TIM1的单脉冲模式。配置TIM1为从模式触发模式，触发源为TI2FP2，通道1为
PWM2模式，映射到PA3，通道2为输入模式，映射到PA13。当PA13上检测到一个上升沿时，PA3
延迟20ms后产生一个宽度为80ms的脉冲。

================================================================================
测试环境：
测试用板：PY32F003_STK
MDK版本： 5.28
IAR版本： 9.20
================================================================================
使用步骤：
1. 编译下载程序到MCU，并运行
2. 按下按键
2. 使用示波器的捕获模式观察PA3，PA13
3. 当PA13上产生一个上升沿时，延迟20ms后PA3输出一个宽度为80ms的脉冲

================================================================================
注意事项：


================================================================================