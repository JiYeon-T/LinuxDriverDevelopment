/***************************************************************
Copyright © zuozhongkai Co., Ltd. 1998-2019. All rights reserved.
文件名	: 	 bsp_epittimer.c
作者	   : 左忠凯
版本	   : V1.0
描述	   : EPIT定时器驱动文件。
其他	   : 配置EPIT定时器，实现EPIT定时器中断处理函数
论坛 	   : www.wtmembed.com
日志	   : 初版V1.0 2019/1/5 左忠凯创建
***************************************************************/
#include "bsp_epittimer.h"
#if CONFIG_BSP_INT_ENABLE
#include "bsp_int.h"
#else
#error "undefined CONFIG_BSP_INT_ENABLE"
#endif
#if CONFIG_BSP_LED_ENABLE
#include "bsp_led.h"
#endif



/*
 * @description		: 初始化EPIT定时器.
 *					  EPIT定时器是32位向下计数器,时钟源使用ipg=66Mhz
 *
 * @param - frac	: 分频值，12bit, 范围为0~4095，分别对应1~4096分频。
 * @param - count	: 倒计数值。
 * @param - handler : 中断服务程序
 * @return 			: 无
 */
void epit1_init(unsigned int frac, unsigned int count, void *irq_handler)
{
	if(frac > 0XFFF)
		frac = 0XFFF;
		
	EPIT1->CR = 0; /* 先清零CR寄存器 */
	// EPIT1->CR &= ~(0xFFFFFFFF);

	/*
     * EPITx_CR寄存器:
     * bit25:24 01 时钟源选择Peripheral clock=66MHz
     * bit15:4  frac 分频值
     * bit3:	1  当计数器到0的话从EPIXx_LR重新加载数值; 0计数器到期后从 0xFFFFFF 开始倒计时
     * bit2:	1  比较中断使能
     * bit1:    1  初始计数值来源于LR寄存器值
     * bit0:    0  先关闭EPIT1
     */
	EPIT1->CR = (1<<24 | frac << 4 | 1<<3 | 1<<2 | 1<<1);
	
	EPIT1->LR = count; /* 倒计数值 */
	EPIT1->CMPR	= 0; /* 比较寄存器的值为 0，当计数器值和此寄存器值相等的话就会产生中断 */

	/* 使能GIC中对应的中断 */
	GIC_EnableIRQ(EPIT1_IRQn);

	/* 注册中断服务函数 */
	system_register_irqhandler(EPIT1_IRQn, (system_irq_handler_t)irq_handler, NULL);	

#if CONFIG_BSP_EPIT_TEST_ENABLE
	EPIT1->CR |= 1<<0; /* 使能EPIT1, 仅测试 timer 的时候才让 timer 直接跑起来 */ 
#endif
}

/*
 * @description			: EPIT中断处理函数
 * @param				: 无
 * @return 				: 无
 */
void epit1_irqhandler(unsigned int giccIar, void *param)
{
	(void)giccIar;
	(void)param;

#if CONFIG_BSP_EPIT_TEST_ENABLE
	static unsigned char state = 0;
	state = !state;
	if(EPIT1->SR & (1<<0)) { /* 判断比较事件发生 */
		led_switch(LED0, state); /* 定时器周期到，反转LED */
	}
#endif

	EPIT1->SR |= 1<<0; 				/* 清除中断标志位 */
}

/*
 * @description		: 重启定时器
 * @param - reload	: 定时器EPIT计数值, 重装载值
 * @return 			: 无
 */
void epit1_start(unsigned int reload)
{
	EPIT1->CR &= ~(1<<0);	/* 先关闭定时器 */
	EPIT1->LR = reload;		/* 计数值 			*/
	EPIT1->CR |= (1<<0);	/* 打开定时器 		*/
}


/*
 * @description		: 关闭定时器
 * @param 			: 无
 * @return 			: 无
 */
void epit1_stop(void)
{
	EPIT1->CR &= ~(1<<0);	/* 关闭定时器 */
}

