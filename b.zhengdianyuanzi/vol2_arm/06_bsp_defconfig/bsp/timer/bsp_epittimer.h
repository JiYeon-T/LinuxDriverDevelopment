/***************************************************************
Copyright © zuozhongkai Co., Ltd. 1998-2019. All rights reserved.
文件名	: 	 bsp_epittimer.h
作者	   : 左忠凯
版本	   : V1.0
描述	   : EPIT定时器驱动头文件。
其他	   : 无
论坛 	   : www.wtmembed.com
日志	   : 初版V1.0 2019/1/5 左忠凯创建
***************************************************************/
#ifndef _BSP_EPITTIMER_H
#define _BSP_EPITTIMER_H

#include "imx6ul.h"

#define CONFIG_BSP_EPIT_TEST_ENABLE 0

void epit1_init(unsigned int frac, unsigned int count, void *irq_handler);
void epit1_irqhandler(unsigned int giccIar, void *param);
void epit1_start(unsigned int reload);
void epit1_stop(void);

#endif
