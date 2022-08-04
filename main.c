#include <linux/module.h>
#include <linux/sched.h>
#include <linux/pid.h>

#include "dump_proc_info.h"

MODULE_LICENSE("GPL");

static int pid = 0;
module_param(pid, int, 0);

static struct task_struct *search_task_struct_by_pid(int pid)
{
	struct task_struct *tsk	= NULL;
	struct pid *pid_s	= NULL;

	pid_s = find_get_pid(pid);
	if (!pid_s)
		return NULL;

	tsk = get_pid_task(pid_s, PIDTYPE_PID);
	if (!tsk)
		return NULL;

	return tsk;
}

static int dump_proc_info_init(void)
{
	struct task_struct *tsk	= NULL;

	if (!pid)
		goto current_info;

	pr_info("dump_proc_info: Dump process's infomation (%d)\n", pid);

	tsk	= search_task_struct_by_pid(pid);
	if (!tsk) {
		pr_info("dump_proc_info: task struct not found\n");
		goto err;
	}

	dump_tls(tsk);
	dump_files(tsk);
	dump_ids(tsk);
	dump_pgd(tsk);
	return 0;

current_info:
	check_sys_clone();
	dump_current();
	return 0;

err:
	return 1;
}

static void dump_proc_info_exit(void)
{
	return;
}

module_init(dump_proc_info_init);
module_exit(dump_proc_info_exit);
