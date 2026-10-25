/***************************************************************
Copyright © zuozhongkai Co., Ltd. 1998-2019. All rights reserved.
文件名	: 	 bsp_led.c
作者	   : 左忠凯
版本	   : V1.0
描述	   : 串口驱动文件头文件。
其他	   : 无
论坛 	   : www.wtmembed.com
日志	   : 初版V1.0 2019/1/15 左忠凯创建
***************************************************************/
#ifndef _BSP_UART_H
#define _BSP_UART_H


#define CONFIG_BSP_UART_ECHO_TEST 0

void uart1_init(void);
void putc(unsigned char c);
void puts(char *str);
unsigned char getc(void);
void raise(int sig_nr);
void uart1_echo_test_loop(void);


#endif