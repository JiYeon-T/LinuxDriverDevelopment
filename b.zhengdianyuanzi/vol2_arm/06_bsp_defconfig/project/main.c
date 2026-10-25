/**************************************************************
Copyright © zuozhongkai Co., Ltd. 1998-2019. All rights reserved.
文件名	: 	 mian.c
作者	   : 左忠凯
版本	   : V1.0
描述	   : I.MX6U开发板裸机实验5 BSP形式的LED驱动
其他	   : 本实验学习目的:
         1、将各个不同的文件进行分类，学习如何整理工程、就
         和学习STM32一样创建工程的各个文件夹分类，实现工程文件
         的分类化和模块化，便于管理。
         2、深入学习Makefile，学习Makefile的高级技巧，学习编写
         通用Makefile。
论坛 	   : www.wtmembed.com
日志	   : 初版V1.0 2019/1/4 左忠凯创建
**************************************************************/
#if CONFIG_BSP_CLK_ENABLE
#include "bsp_clk.h"
#endif
#if CONFIG_BSP_DELAY_ENABLE
#include "bsp_delay.h"
#endif
#if CONFIG_BSP_LED_ENABLE
#include "bsp_led.h"
#endif
#if CONFIG_BSP_BEEP_ENABLE
#include "bsp_beep.h"
#endif
#if CONFIG_BSP_KEY_ENABLE
#include "bsp_key.h"
#endif
#if CONFIG_BSP_INT_ENABLE
#include "bsp_int.h"
#endif
#if CONFIG_BSP_EPIT_ENABLE
#include "bsp_epittimer.h"
#include "bsp_gpt.h"
#endif
#if CONFIG_BSP_UART_ENABLE
#include "bsp_uart.h"
#endif
#if CONFIG_STDIO_PORT_ENABLE
#include "stdio.h"
#include "stdio_test.h"
#endif
#if CONFIG_BSP_LCD_ENABLE
#include "bsp_lcd.h"
#endif

/*
 * @description	: mian函数
 * @param 		: 无
 * @return 		: 无
 */
int main(void)
{
#if CONFIG_BSP_INT_ENABLE
    int_init(); /* 初始化中断，一定要最先调用 */
#endif

#if CONFIG_BSP_CLK_ENABLE
#if CONFIG_BSP_CLK_TEST_ENABLE
    imx6u_clkinit();
#endif
    clk_enable(); /* 使能所有的时钟 */
#endif

#if CONFIG_BSP_LED_ENABLE
    led_init(); /* 初始化led */
#endif

#if CONFIG_BSP_BEEP_ENABLE
    beep_init();
#endif

#if CONFIG_BSP_KEY_ENABLE
#if CONFIG_BSP_KEY_TEST_ENABLE
    key_init();
#elif CONFIG_BSP_KEY_INT_TEST_ENABLE
    key_exit_init();
#endif
#endif

#if CONFIG_BSP_EPIT_TEST_ENABLE
    /* 时钟源66MHz, 1分频, 初始值为66000000/2,也就是500ms */
    epit1_init(0, 66000000/2, epit1_irqhandler);
#endif

#if CONFIG_BSP_HIGH_PREC_DELAY_TEST
    gpt1_init(65, 0xFFFFFFFF, NULL);
    gpt1_start();
#elif CONFIG_BSP_GPT_IRQ_TEST
    /* 时钟源66MHz, 1分频, 初始值为66000000/2,也就是500ms */
    gpt1_init(0, 66000000, gpt1_irqhandler);
    gpt1_start();
#endif

#if CONFIG_BSP_UART_ENABLE
    uart1_init();
#endif

#if CONFIG_BSP_LCD_ENABLE
    lcd_init();
#if CONFIG_BSP_LCD_TEST
    lcd_test_loop();
#endif
#endif

    while (1) {
#if CONFIG_BSP_LED_TEST_ENABLE
        led_test();
#endif

#if CONFIG_BSP_BEEP_TEST_ENABLE
        beep_test();
#endif

#if CONFIG_BSP_KEY_TEST_ENABLE
        bsp_key_test_loop();
#endif

#if CONFIG_BSP_HIGH_PREC_DELAY_TEST
        delay_test_loop();
#elif CONFIG_BSP_GPT_IRQ_TEST
#endif

#if CONFIG_BSP_UART_ECHO_TEST
        uart1_echo_test_loop();
#endif

#if CONFIG_STDIO_PORT_TEST
        stdio_io_format_test();
#endif
    }

    return 0;
}
