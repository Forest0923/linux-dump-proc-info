#include <linux/sched.h>
#include <linux/mm_types.h>

#include <asm/paravirt_types.h>

void dump_ids(struct task_struct *tsk)
{
	pr_info("dump_proc_ids: => PID = %d, TGID = %d", tsk->pid, tsk->tgid);
	pr_info("dump_proc_ids: => PPID = %d", tsk->parent->pid);
}

void dump_pgd(struct task_struct *tsk)
{
	unsigned long cr3 = pv_ops.mmu.read_cr3();
	pr_info("dump_proc_pgd: => *pgd = %p", tsk->mm->pgd);
	pr_info("dump_proc_pgd: =>  pgd = 0x%lx", *tsk->mm->pgd);
	pr_info("dump_proc_cr3: =>  cr3 = 0x%lx", cr3);
}
