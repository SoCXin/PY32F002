================================================================================
                                样例使用说明
================================================================================
功能描述：
此样例演示了TIM的同步触发模式。TIM3设置为主模式触发，TIM1设置从模式触发。程序复位
后TIM3开始计数，触发更新事件后向TIM1发送同步信号，TIM1开始计数，并在更新中断中翻转
LED。

================================================================================
测试环境：
测试用板：PY32F003_STK
MDK版本： 5.28
IAR版本： 9.20
================================================================================
使用步骤：
1. 编译下载程序到MCU，并运行
2. 复位MCU后LED亮起，2s后LED开始翻转，翻转间隔为1s

================================================================================
注意事项：


================================================================================