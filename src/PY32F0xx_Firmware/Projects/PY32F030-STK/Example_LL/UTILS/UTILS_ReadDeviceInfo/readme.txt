================================================================================
                                样例使用说明
================================================================================
功能描述：
本样例主要读取DBGMCU->IDCODE寄存器和UID的值,并通过串口打印出来。

================================================================================
测试环境：
测试用板：PY32F030_STK
MDK版本： 5.28
IAR版本： 9.20
================================================================================
使用步骤：
1. 编译下载程序到MCU，并运行；
2. 串口依次打印Device ID,Revision ID,X and Y coordinates,Water NB,Lot Nb的值。

================================================================================
注意事项：
STK板        USB转TTL模块
PA02(TX) --> RX
PA03(RX) --> TX
GND      --> GND

波特率:115200

================================================================================