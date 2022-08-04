#include <linux/sched.h>
#include <linux/mm_types.h>
#include <asm/io.h>

void dump_ids(struct task_struct *tsk)
{
	pr_info("dump_proc_ids: => PID = %d, TGID = %d\n", tsk->pid, tsk->tgid);
	pr_info("dump_proc_ids: => PPID = %d\n", tsk->parent->pid);
}

void dump_pgd(struct task_struct *tsk)
{
	pr_info("dump_proc_pgd: => *pgd		= 0x%lx\n", (unsigned long)tsk->mm->pgd);
	pr_info("dump_proc_pgd: => phys_pgd	= 0x%llx\n", virt_to_phys(tsk->mm->pgd));
}
