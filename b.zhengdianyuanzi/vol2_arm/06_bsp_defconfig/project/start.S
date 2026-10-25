/***************************************************************
Copyright © zuozhongkai Co., Ltd. 1998-2019. All rights reserved.
文件名	: 	 start.s
作者	   : 左忠凯
版本	   : V1.0
描述	   : ZERO-I.MX6UL/I.MX6ULL开发板启动文件，完成C环境初始化，
		 C环境初始化完成以后跳转到C代码。
其他	   : 无
论坛 	   : www.wtmembed.com
日志	   : 初版V1.0 2019/1/3 左忠凯修改
		 V2.0 2019/1/4 左忠凯修改
		 添加中断相关定义
**************************************************************/

.global _start  				/* 全局标号 */

/*
 * 描述：	_start函数，首先是中断向量表的创建
 * 参考文档:ARM Cortex-A(armV7)编程手册V4.0.pdf P42，3 ARM Processor Modes and Registers（ARM处理器模型和寄存器）
 * 		 	ARM Cortex-A(armV7)编程手册V4.0.pdf P165 11.1.1 Exception priorities(异常)
 */
_start:
	ldr pc, =Reset_Handler		/* 复位中断 */
	/* 这里为什么这样写, 因为 ResetHandler 最后会跳转到 main 函数继续执行,不会返回 */
	ldr pc, =Undefined_Handler	/* 未定义中断 */
	ldr pc, =SVC_Handler		/* SVC(Supervisor)中断 */
	ldr pc, =PrefAbort_Handler	/* 预取终止中断 */
	ldr pc, =DataAbort_Handler	/* 数据终止中断 */
	ldr	pc, =NotUsed_Handler	/* 未使用中断 */
	ldr pc, =IRQ_Handler		/* IRQ中断 */
	ldr pc, =FIQ_Handler		/* FIQ(快速中断)未定义中断 */

/* 复位中断 */	
Reset_Handler:

	cpsid i						/* 关闭全局中断 */

	/* 关闭I,DCache和MMU 
	 * 采取读-改-写的方式。
	 */
	mrc     p15, 0, r0, c1, c0, 0     /* 读取CP15的C1寄存器(协处理器CP15的寄存器)到R0中，SCTLR(System Control Register) */
    bic     r0,  r0, #(0x1 << 12)     /* 清除C1寄存器SCTLR的bit12位(I位)，关闭I Cache */
    bic     r0,  r0, #(0x1 <<  2)     /* 清除C1寄存器SCTLR的bit2(C位)，关闭D Cache */
    bic     r0,  r0, #0x2             /* 清除C1寄存器SCTLR的bit1(A位)，关闭内存对齐检查 */
    bic     r0,  r0, #(0x1 << 11)     /* 清除C1寄存器SCTLR的bit11(Z位)，关闭分支预测 */
    bic     r0,  r0, #0x1             /* 清除C1寄存器SCTLR的bit0(M位)，关闭MMU */
    mcr     p15, 0, r0, c1, c0, 0     /* 将r0寄存器中的值写入到CP15的C1寄存器中 */

	
#if 0
	/* 汇编版本设置中断向量表偏移 */
	ldr r0, =0X87800000

	dsb
	isb
	mcr p15, 0, r0, c12, c0, 0
	dsb
	isb
#endif

	/* 设置各个模式下的栈指针，
	 * 注意：IMX6UL的堆栈是向下增长的！
	 * 堆栈指针地址一定要是4字节地址对齐的！！！
	 * DDR范围:0X80000000~0X9FFFFFFF
	 */
	/* 进入IRQ模式 */
	mrs r0, cpsr
	bic r0, r0, #0x1f 	/* 将r0寄存器中的低5位清零，也就是cpsr的M0~M4 	*/
	orr r0, r0, #0x12 	/* r0|=0x12,表示使用IRQ模式 */
	msr cpsr, r0		/* 将r0 的数据写入到cpsr_c中,进入 IRQ 模式 */
	ldr sp, =0x80600000	/* 设置IRQ模式下的栈首地址SP为0X80600000,大小为2MB */

	/* 进入SYS模式 */
	mrs r0, cpsr
	bic r0, r0, #0x1f 	/* 将r0寄存器中的低5位清零，也就是cpsr的M0~M4 */
	orr r0, r0, #0x1f 	/* r0或上0x1F,表示使用SYS模式 */
	msr cpsr, r0		/* 将r0 的数据写入到cpsr_c中,进入 SYS 模式 */
	ldr sp, =0x80400000	/* 设置SYS模式下的栈首地址SP为0X80400000,大小为2MB */

	/* 进入SVC模式 */
	mrs r0, cpsr
	bic r0, r0, #0x1f 	/* 将r0寄存器中的低5位清零，也就是cpsr的M0~M4 */
	orr r0, r0, #0x13 	/* r0或上0x13,表示使用SVC模式 */
	msr cpsr, r0		/* 将r0 的数据写入到cpsr_c中,进入 SVC 模式 */
	ldr sp, =0X80200000	/* 设置SVC模式下的栈首地址SP为0X80200000,大小为2MB */

	cpsie i				/* 打开全局中断 */
#if 0
	/* 操作CPSR寄存器打开IRQ中断 */
	mrs r0, cpsr		/* 读取cpsr寄存器值到r0中 */
	bic r0, r0, #0x80	/* 将r0寄存器中bit7清零，也就是CPSR中的I位清零，表示允许IRQ中断 */
	msr cpsr, r0		/* 将r0重新写入到cpsr中 */
#endif

	b main				/* 跳转到main函数 */


/* 中断服务函数：未定义中断 */
Undefined_Handler:
	ldr r0, =Undefined_Handler	/*死循环*/
	bx r0

/* SVC中断 */
SVC_Handler:
	ldr r0, =SVC_Handler
	bx r0

/* 预取终止中断 */
PrefAbort_Handler:
	ldr r0, =PrefAbort_Handler	
	bx r0

/* 数据终止中断 */
DataAbort_Handler:
	ldr r0, =DataAbort_Handler
	bx r0

/* 未使用的中断 */
NotUsed_Handler:

	ldr r0, =NotUsed_Handler
	bx r0

/* IRQ中断！重点！！！！！ */
IRQ_Handler:
	push {lr}					/* 保存lr地址 */
	push {r0-r3, r12}			/* 保存r0-r3，r12寄存器 */

	mrs r0, spsr				/* 读取spsr寄存器 */
	push {r0}					/* 保存spsr寄存器 */

	mrc p15, 4, r1, c15, c0, 0 /* 从CP15的C15寄存器(CBAR, Configuration Base Address Register, 其中保存的就是 GIC 的基地址)内的值到R1寄存器中
								* 参考文档ARM Cortex-A(armV7)编程手册V4.0.pdf P49
								* Cortex-A7 Technical ReferenceManua.pdf P68 P138
								*/							
	add r1, r1, #0X2000			/* GIC基地址加0X2000，也就是GIC的CPU接口端基地址 */
	ldr r0, [r1, #0XC]			/* GIC的CPU接口端基地址加0X0C就是 GICC_IAR 寄存器，
								 * GICC_IAR寄存器保存这当前发生中断的中断号，我们要根据
								 * 这个中断号来绝对调用哪个中断服务函数
								 */
	push {r0, r1}				/* 保存r0,r1 */
	
	cps #0x13					/* 进入SVC模式，允许其他中断再次进去, 在 SVC 模式下执行中断服务程序 */

	push {lr}					/* 保存SVC模式的lr寄存器 */
	ldr r2, =system_irqhandler	/* 加载C语言中断处理函数到r2寄存器中*/
	blx r2						/* 运行C语言中断处理函数，带有一个参数，保存在R0寄存器中， 参数通过 R0 传递(ATPCS) */

	pop {lr}					/* 执行完C语言中断服务函数，lr出栈 */
	cps #0x12					/* 进入IRQ模式 */
	pop {r0, r1}
	str r0, [r1, #0X10]			/* 中断执行完成，写 GICC_EOIR, 通知 CPU 该中断执行完成 */

	pop {r0}						
	msr spsr_cxsf, r0			/* 恢复spsr */

	pop {r0-r3, r12}			/* r0-r3,r12出栈 */
	pop {lr}					/* lr出栈 */
	subs pc, lr, #4				/* 将lr-4赋给pc, ARM CPU 三级流水线结构, 所以PC=Lr-4
								Fetch, Decode, Excute, 执行代码的时候, PC 的值为当前执行代码+8,
								返回时返回当前执行指令的吓一条指令,所以是 lr-4
								LR 中保存的是中断时候的 pc 的值 */


/* FIQ中断 */
FIQ_Handler:

	ldr r0, =FIQ_Handler	
	bx r0									

