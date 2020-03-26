/*  
 *  Disable/Enable Write Streaming No-Allocate
 */
#include <linux/module.h>   /* Needed by all modules */
#include <linux/kernel.h>   /* Needed for KERN_INFO */
#include <linux/smp.h>

#define PERF_DEF_OPTS 			(1 | 16)
#define PERF_OPT_RESET_CYCLES 		(2 | 4)
#define PERF_OPT_DIV64 			(8)

#define MODULE_PARAM(type, name, value, desc) \
    type name = value; \
    module_param(name, type, 0664); \
    MODULE_PARM_DESC(name, desc)

MODULE_PARAM(int, debug, 0, "Debug output");

#define DRVR_NAME "disable_wsna"

#if !defined(__arm__)
#error Module can only be compiled on ARM platforms.
#endif

static void
disable_wsna(void *data)
{
	int reg;
	printk(KERN_INFO "[" DRVR_NAME "] disable WSNA on CPU #%d\n",
			smp_processor_id());

	/* Read current value of ACTLR */
	asm volatile("MRC p15, 0, %0, c1, c0, 1" : "=r"(reg));
	reg = reg | (1 << 27);
	/* Set current value of ACTLR */
	asm volatile("MCR p15, 0, %0, c1, c0, 1" : : "r"(reg));
}

static void
reenable_wsna(void *data)
{
	int reg;
	printk(KERN_INFO "[" DRVR_NAME "] re-enabling WSNA on CPU #%d\n",
			smp_processor_id());

	/* Read current value of ACTLR */
	asm volatile("MRC p15, 0, %0, c1, c0, 1" : "=r"(reg));
	reg = reg & (~(1 << 27));
	/* Set current value of ACTLR */
	asm volatile("MCR p15, 0, %0, c1, c0, 1" : : "r"(reg));
}

int start_module(void)
{
    on_each_cpu(disable_wsna, NULL, 1);
    printk(KERN_INFO "[" DRVR_NAME "] initialized\n");
    return 0;
}
void stop_module(void)
{
	on_each_cpu(reenable_wsna, NULL, 1);
	printk(KERN_INFO "[" DRVR_NAME "] unloaded\n");
}

module_init(start_module);
module_exit(stop_module)

MODULE_AUTHOR("Johannes Hofmann <johannes.hofmann@fau.de>");
MODULE_DESCRIPTION("Disable/Enable WSNA");
MODULE_LICENSE("GPL");
