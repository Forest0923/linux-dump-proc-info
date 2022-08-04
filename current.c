#include <linux/sched.h>
#include <asm/paravirt_types.h>
#include <asm/io.h>
#include "dump_proc_info.h"

void dump_current(void)
{
	struct task_struct *tsk = current;
	unsigned long cr3 = pv_ops.mmu.read_cr3();

	pr_info("dump_proc_current: => cr3	= 0x%lx\n", cr3);
	dump_pgd(tsk);
}
