.global _start
.global _bss_end
.global _bss_start


_start:
/*中断向量表*/
    ldr pc,=Reset_Handler
    ldr pc,=Undefined_Handler
    ldr pc,=SVC_Handler
    ldr pc,=PreAbort_Handler
    ldr pc,=DataAbort_Handler
    ldr pc,=NotUsed_Handler
    ldr pc,=IRQ_Handler
    ldr pc,=FIQ_Handler

Reset_Handler:

    cpsid i /* 关闭全局中断 */
/*关闭I、DCache和MMU*/
    mrc p15, 0, r0, c1, c0, 0  /*读取SCTLR值到ARM R0寄存器*/
    bic r0,r0,#(1<<12) /*关闭I Cache*/
    bic r0,r0,#(1<<11) /*关闭分支预测使能*/
    bic r0,r0,#(1<<2) /*禁止 D Cache 和缓存一致性*/
    bic r0,r0,#(1<<1) /*关闭内存对齐检查*/
    bic r0,r0,#(1<<0) /*关闭MMU*/
    mcr p15, 0, r0, c1, c0, 0   /*将ARM R0寄存器值写入到SCTLR*/

#if 0
/*设置中断向量偏移*/
    ldr r0,=0x87800000
    dsb   /*数据同步*/
    isb   /*指令同步*/
    mcr p15, 0, r0, c12, c0, 0   /*将ARM R0寄存器值写入到VBAR*/ 
    dsb
    isb
#endif

_bss_start:
    .word __bss_start

_bss_end:
    .word __bss_end
/*清楚BSS段 */
    ldr r0,_bss_start
    ldr r1,_bss_end
    mov r2,#0
bss_loop:

    stmia r0!,{r2}  /* */
    cmp r0,r1     /*ro-r1 比较 */
    ble bss_loop /*ro<=r1跳转*/








/*设置各个模式下的SP指针*/



/*设置cortex A7处理器为IRQ模式 */
    mrs r0,cpsr
    bic r0,r0,#0x1f
    orr r0,r0,#0x12
    msr cpsr,r0
    ldr sp,=0x80600000 /*设置sp指向0x80600000 2MByte*/

/*设置cortex A7处理器为sys模式 */
    mrs r0,cpsr
    bic r0,r0,#0x1f
    orr r0,r0,#0x1f
    msr cpsr,r0
    ldr sp,=0x80400000 /*设置sp指向0x80400000 2MByte*/

/*设置cortex A7处理器为超级管理员SVC模式 */
    mrs r0,cpsr
    bic r0,r0,#0x1f
    orr r0,r0,#0x13
    msr cpsr,r0
    ldr sp,=0x80200000 /*设置sp指向0x80200000 2MByte*/
    
    cpsie i /* 打开IRQ */
/*跳转到main*/
    b main

Undefined_Handler:
    ldr r0,=Undefined_Handler
    bx r0

SVC_Handler:
    ldr r0,=SVC_Handler
    bx r0

PreAbort_Handler:
    ldr r0,=PreAbort_Handler
    bx r0

DataAbort_Handler:
    ldr r0,=DataAbort_Handler
    bx r0

NotUsed_Handler:
    ldr r0,=NotUsed_Handler
    bx r0

IRQ_Handler:
	push {lr}					/* 保存lr地址R14连接寄存器，保存当前子函数的返回地址 */
	push {r0-r3, r12}			/* 保存r0-r3，r12寄存器 */
    /*当特定的异常中断发生时， SPSR 寄存器用来保存当前程序状
    *态寄存器(CPSR)的值，当异常退出以后可以用 SPSR 中保存的值来恢复 CPSR
    */
	mrs r0, spsr				/* 读取spsr寄存器 */
	push {r0}					/* 保存spsr寄存器 */

	mrc p15, 4, r1, c15, c0, 0 /* 从CP15的C0寄存器内的值到R1寄存器中,CBAR寄存器保存GIC基地址
								* 参考文档ARM Cortex-A(armV7)编程手册V4.0.pdf P49
								* Cortex-A7 Technical ReferenceManua.pdf P68 P138
								*/							
	add r1, r1, #0X2000			/* GIC基地址加0X2000，也就是GIC的CPU接口端基地址 */
	ldr r0, [r1, #0XC]			/* GIC的CPU接口端基地址加0X0C就是GICC_IAR寄存器，
								 * GICC_IAR寄存器保存这当前发生中断的中断号，我们要根据
								 * 这个中断号来绝对调用哪个中断服务函数
								 */
	push {r0, r1}				/* 保存r0,r1 */
	
	cps #0x13					/* 进入SVC模式，允许其他中断再次进去 */
	
	push {lr}					/* 保存SVC模式的lr寄存器 */
	ldr r2, =system_irqhandler	/* 加载C语言中断处理函数到r2寄存器中*/
	blx r2						/* 运行C语言中断处理函数，带有一个参数，保存在R0寄存器中 */

	pop {lr}					/* 执行完C语言中断服务函数，lr出栈 */
	cps #0x12					/* 进入IRQ模式 */
	pop {r0, r1}				
	str r0, [r1, #0X10]			/* 中断执行完成，写EOIR */

	pop {r0}						
	msr spsr_cxsf, r0			/* 恢复spsr */

	pop {r0-r3, r12}			/* r0-r3,r12出栈 */
	pop {lr}					/* lr出栈 */
	subs pc, lr, #4				/* 将lr-4赋给pc 三级流水线，执行，译指，取指*/


FIQ_Handler:
    ldr r0,=FIQ_Handler
    bx r0


/*设置9种模式下对应的SP指针*/




























