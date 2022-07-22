#include <linux/module.h>
#include <linux/sched.h>
#include <linux/pid.h>

MODULE_LICENSE("GPL");

static int pid = 0;
module_param(pid, int, 0);

extern void dump_tls(struct task_struct *tsk);

static int dump_ts_info_init(void)
{
	struct task_struct *tsk	= NULL;
	struct pid *pid_s	= NULL;

	pr_info("dump_ts: Dump task_struct infomation (%d)", pid);

	pid_s = find_get_pid(pid);
	if (!pid_s) {
		pr_info("dump_ts: PID not found");
		goto err;
	}

	tsk = get_pid_task(pid_s, PIDTYPE_PID);
	if (!tsk) {
		pr_info("dump_ts: task struct not found");
		goto err;
	}

	dump_tls(tsk);
	return 0;
err:
	return 1;
}

static void dump_ts_info_exit(void)
{
	return;
}

module_init(dump_ts_info_init);
module_exit(dump_ts_info_exit);
