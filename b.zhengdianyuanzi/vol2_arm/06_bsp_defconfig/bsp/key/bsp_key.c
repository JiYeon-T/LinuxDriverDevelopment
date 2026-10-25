/***************************************************************
Copyright © zuozhongkai Co., Ltd. 1998-2019. All rights reserved.
文件名	: 	 bsp_key.h
作者	   : 左忠凯
版本	   : V1.0
描述	   : 按键驱动文件。
其他	   : 无
论坛 	   : www.wtmembed.com
日志	   : 初版V1.0 2019/1/4 左忠凯创建
***************************************************************/

#include "imx6ul.h"

#include "bsp_key.h"
#if CONFIG_BSP_GPIO_ENABLE
#include "bsp_gpio.h"
#else
#error "undefined CONFIG_BSP_GPIO_ENABLE"
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
#if CONFIG_BSP_INT_ENABLE
#include "bsp_int.h"
#include "bsp_exit.h"
#endif
#if CONFIG_BSP_TIMER_ENABLE
#include "bsp_epittimer.h"
#endif

#define CONFIG_KEY_DELAY_FILTER 0 /* 使用死循环延时方式消除按键抖动 */
#define CONFIG_KEY_TIMER_FILTER 1 /* 使用定时器延时方式消除按键抖动 */

/*
 * @description	: 初始化按键
 * @param 		: 无
 * @return 		: 无
 */
void key_init(void)
{
	gpio_pin_config_t key_config;
	
	/* 1、初始化IO复用, 复用为GPIO1_IO18  SW_MUX_CTL_PAD_UART1_CTS_B
	 */
	// #define IOMUXC_UART1_CTS_B_GPIO1_IO18 0x020E008CU, 0x5U, 0x00000000U, 0x0U, 0x020E0318U
	IOMUXC_SetPinMux(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0);

	/* 2、、配置UART1_CTS_B的IO属性	 SW_PAD_CTL_PAD_UART1_CTS_B
	 *bit 16:0 HYS关闭
	 *bit [15:14]: 11 默认22K上拉, 默认高电平, 按键按下后变为低点平
	 *bit [13]: 1 pull功能
	 *bit [12]: 1 pull/keeper使能
	 *bit [11]: 0 关闭开路输出
	 *bit [7:6]: 10 速度100Mhz
	 *bit [5:3]: 000 关闭输出
	 *bit [0]: 0 低转换率
	 */
	IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0xF080);
	
	/* 3、初始化GPIO */
	//GPIO1->GDIR &= ~(1 << 18);	/* GPIO1_IO18设置为输入 */	
	key_config.direction = kGPIO_DigitalInput;
	gpio_init(GPIO1, 18, &key_config);
}

/*
 * @description	: 获取按键值 
 *
 * @param 		: 无
 * @return 		: 0 没有按键按下，其他值:对应的按键值
 */
int key_getvalue(void)
{
	int ret = KEY_NONE;
	static unsigned char released = 1; /* 按键松开 */ 

	if((released==1) && (gpio_pinread(GPIO1, 18) == 0)) /* KEY0 ， 低电平表示按键被按下	*/
	{
		released = 0;	/* 标记按键按下 */
		delay_ms(10);		/* 延时消抖 */
		if(gpio_pinread(GPIO1, 18) == 0) /* key 仍然处于按下状态 */
		{
			ret = KEY0_VALUE;
		}
	}
	else if(gpio_pinread(GPIO1, 18) == 1)
	{
		ret = KEY_NONE;
		released = 1; /* 标记按键释放 */
	}

	return ret;	
}

#if CONFIG_BSP_KEY_TEST_ENABLE
void bsp_key_test_loop(void)
{
	static int state = OFF;
	int key_value = key_getvalue();

	if (key_value == KEY_NONE) {
		return;
	}

	switch(key_value) {
		case KEY0_VALUE: {
			if (state == OFF) {
#if CONFIG_BSP_LED_ENABLE
				led_switch(LED0, ON);
#endif
#if CONFIG_BSP_BEEP_ENABLE
				beep_switch(ON);
#endif
				state = ON;
			} else if (state == ON) {
#if CONFIG_BSP_LED_ENABLE
				led_switch(LED0, OFF);
#endif
#if CONFIG_BSP_BEEP_ENABLE
				beep_switch(OFF);
#endif
				state = OFF;
			}

		break;
		}
		case KEY1_VALUE:
		break;
		case KEY2_VALUE:
		break;
		default:
		break;
	}
}
#endif /* CONFIG_BSP_KEY_TEST_ENABLE */

#if CONFIG_BSP_KEY_INT_TEST_ENABLE

#if CONFIG_KEY_TIMER_FILTER
static void epit1_filter_handler (unsigned int giccIar, void *param)
{
	static unsigned char state = OFF;

	if (EPIT1->SR & (1<<0)) { /* 判断比较事件是否发生 */
		epit1_stop(); /* 关闭定时器 */
		if (gpio_pinread(GPIO1, 18) == 0) { /* KEY0 */
			state = !state;
			beep_switch(state); /* 反转蜂鸣器 */
		}
	}
		
	EPIT1->SR |= 1<<0; /* 清除中断标志位 */
}
#endif /* CONFIG_KEY_TIMER_FILTER */

/*
 * @description			: 初始化GPIO1.18的外部中断, 该 IO UART1_CTS 复用为 GPIO
 *
 * @param				: 无
 * @return 				: 无
 */
void key_exit_init(void)
{
	gpio_pin_config_t key_config;

	/* 1、设置IO复用 */
	IOMUXC_SetPinMux(IOMUXC_UART1_CTS_B_GPIO1_IO18,0); /* 复用为GPIO1_IO18 */
	IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18,0xF080);

	/* 2、初始化GPIO为中断模式 */
	key_config.direction = kGPIO_DigitalInput;
	key_config.interruptMode = kGPIO_IntFallingEdge;
	key_config.outputLogic = 1;
	gpio_init(GPIO1, 18, &key_config);

#ifdef CONFIG_BSP_INT_ENABLE
	GIC_EnableIRQ(GPIO1_Combined_16_31_IRQn); /* 使能GIC中对应的中断 */
	system_register_irqhandler(GPIO1_Combined_16_31_IRQn, (system_irq_handler_t)gpio1_io18_irqhandler, NULL);	/* 注册中断服务函数 */
	gpio_enableint(GPIO1, 18); /* 使能GPIO1_IO18的中断功能 */
#else
#error "undefined CONFIG_BSP_INT_ENABLE"
#endif

#if CONFIG_KEY_TIMER_FILTER
	/* 使用定时器 EPIT1 消抖, 定时时长: 
	epit1 主频 66MHz, Tout = 66000000/100 * (1/66000000) = 1/100s = 10ms  */
	epit1_init(0, 66000000/100, epit1_filter_handler);
#endif
}

/*
 * @description			: GPIO1_IO18最终的中断处理函数
 *
 * @param				: 无
 * @return 				: 无
 */
void gpio1_io18_irqhandler(void)
{
#if CONFIG_KEY_DELAY_FILTER
	static unsigned char state = 0;
	/*
	 *采用延时消抖，中断服务函数中禁止使用延时函数！因为中断服务需要
	 *快进快出！！这里为了演示所以采用了延时函数进行消抖，后面我们会讲解
	 *定时器中断消抖法！！！
 	 */

	delay_ms(10);
	if(gpio_pinread(GPIO1, 18) == 0) { /* 按键按下了  */
		state = !state;
		beep_switch(state);
	}
#elif CONFIG_KEY_TIMER_FILTER
	epit1_start(66000000/100);
#endif

	gpio_clearintflags(GPIO1, 18); /* 清除中断标志位 */
}

#endif /* CONFIG_BSP_KEY_INT_TEST_ENABLE */
