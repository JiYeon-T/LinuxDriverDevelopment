/***************************************************************
Copyright © zuozhongkai Co., Ltd. 1998-2019. All rights reserved.
文件名	: 	 bsp_delay.h
作者	   : 左忠凯
版本	   : V1.0
描述	   : 通用定时器头文件
其他	   : 无
论坛 	   : www.wtmembed.com
日志	   : 初版V1.0 2019/1/4 左忠凯创建
		
***************************************************************/
#ifndef __BSP_GPT_H__
#define __BSP_GPT_H__

#define CONFIG_BSP_HIGH_PREC_DELAY_TEST     0
#define CONFIG_BSP_GPT_IRQ_TEST             1

void gpt1_init(int frac, int count, void *irqhandler);
void gpt1_start(void);
void gpt1_stop(void);
void gpt1_irqhandler(void);

void gpt1_delay_us(unsigned int us_cnt);
void gpt1_delay_ms(unsigned int ms_cnt);
void delay_test_loop(void);


#endif /* __BSP_GPT_H__ */