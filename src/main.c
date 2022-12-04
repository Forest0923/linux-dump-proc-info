#include <linux/module.h>
#include <linux/sched.h>
#include <linux/pid.h>
#include <linux/printk.h>
#include <linux/kernel.h>
#include <linux/slab.h>

#include "dump_proc_info.h"

MODULE_LICENSE("GPL");

static int pid = 0;
module_param(pid, int, 0);

static char *uptime;
module_param(uptime, charp, 0);

static unsigned long addr = 0x0;
module_param(addr, ulong, 0);

static int obj_mask = 0;
module_param(obj_mask, int, 0);

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

int my_printk(const char *fmt, ...) {
	va_list args;
	int r;
	char *dest = (char *)kmalloc(sizeof(char)*(strlen(uptime) + strlen(fmt) + 3), GFP_KERNEL);

	sprintf(dest, "[%s] %s", uptime, fmt);
	va_start(args, fmt);
	r = vprintk(dest, args);
	va_end(args);
	kfree(dest);

	return r;
}

static int __init dump_proc_info_init(void)
{
	struct task_struct *tsk	= NULL;

	if (!pid)
		goto err;

	tsk	= search_task_struct_by_pid(pid);
	if (!tsk) {
		my_printk("proc_debugger: task struct not found\n");
		goto err;
	}

	if (obj_mask == 0 && addr == 0)
		goto err;

	if (obj_mask & OBJ_MASK_PGD) {
		my_printk("proc_debugger: #######\n");
		my_printk("proc_debugger: # PGD #\n");
		my_printk("proc_debugger: #######\n");
		dump_pgd(tsk);
	}
	if (obj_mask & OBJ_MASK_VMA) {
		my_printk("proc_debugger: #######\n");
		my_printk("proc_debugger: # VMA #\n");
		my_printk("proc_debugger: #######\n");
		dump_tsk_vma(tsk);
	}
	if (obj_mask & OBJ_MASK_MEMREG) {
		my_printk("proc_debugger: #################\n");
		my_printk("proc_debugger: # Memory region #\n");
		my_printk("proc_debugger: #################\n");
		dump_mm_params(tsk);
	}
	if (obj_mask & OBJ_MASK_TLS) {
		my_printk("proc_debugger: ########################\n");
		my_printk("proc_debugger: # Thread Local Storage #\n");
		my_printk("proc_debugger: ########################\n");
		dump_tls(tsk);
	}
	if (obj_mask & OBJ_MASK_FILE) {
		my_printk("proc_debugger: ####################\n");
		my_printk("proc_debugger: # File Descriptors #\n");
		my_printk("proc_debugger: ####################\n");
		dump_files(tsk);
	}
	if (addr) {
		unsigned long phys;
		phys = page_table_walk(addr, tsk->mm);
		if (phys)
			my_printk("proc_debugger: 0x%lx\t=> 0x%lx\n", addr, phys);
		else
			my_printk("proc_debugger: 0x%lx\t=> no mapping to physical memory\n", addr);
	}
	return 0;

err:
	return 1;
}

static void __exit dump_proc_info_exit(void)
{
	return;
}

module_init(dump_proc_info_init);
module_exit(dump_proc_info_exit);
