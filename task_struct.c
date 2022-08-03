#include <linux/sched.h>

void dump_ids(struct task_struct *tsk)
{
	pr_info("dump_proc_ids: => PID = %d, TGID = %d", tsk->pid, tsk->tgid);
	pr_info("dump_proc_ids: => PPID = %d", tsk->parent->pid);
}
