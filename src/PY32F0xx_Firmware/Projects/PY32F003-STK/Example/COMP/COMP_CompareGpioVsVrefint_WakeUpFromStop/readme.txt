================================================================================
                                样例使用说明
================================================================================
功能描述：
此样例演示了COMP比较器唤醒功能，PA01作为比较器正端输入，VREFINT作为比较器负端输入，
PA06作为比较器的输出端口，进入stop模式后，通过调整PA01上的输入电压，产生中断唤醒
stop模式。

================================================================================
测试环境：
测试用板：PY32F003_STK
MDK版本： 5.28
IAR版本： 9.20
================================================================================
使用步骤：
1. 编译下载程序到MCU，并运行；
2. LED亮起后按下按键，进入stop模式，LED熄灭；
3. 在PA01上输入从大于1.3V到小于1.1V的电压时，产生中断，LED点亮1s；
4. 接下来LED每隔200ms进行翻转，程序正常运行。

================================================================================
注意事项：


================================================================================