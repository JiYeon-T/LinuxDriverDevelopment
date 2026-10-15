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
#include "prj_config.h"
#include "bsp_clk.h"
#include "bsp_delay.h"
#if CONFIG_BSP_LED_ENABLE
#include "bsp_led.h"
#endif
#if CONFIG_BSP_BEEP_ENABLE
#include "bsp_beep.h"
#endif


/*
 * @description	: mian函数
 * @param 		: 无
 * @return 		: 无
 */
int main(void)
{
    clk_enable(); /* 使能所有的时钟 */
#if CONFIG_BSP_LED_ENABLE
    led_init(); /* 初始化led */
#endif
#if CONFIG_BSP_BEEP_ENABLE
    beep_init();
#endif

    while (1) {
#if CONFIG_BSP_LED_TEST_ENABLE
        led_test();
#endif

#if CONFIG_BSP_BEEP_TEST_ENABLE
        beep_test();
#endif

    }

    return 0;
}
