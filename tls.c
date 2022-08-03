#include <asm/segment.h>
#include <linux/sched.h>

void dump_tls(struct task_struct *tsk)
{
	int i;
	struct thread_struct *t = &tsk->thread;
	struct desc_struct *arr = tsk->thread.tls_array;

	pr_info("dump_proc_tls: => tls_array");
	for (i = 0; i < GDT_ENTRY_TLS_ENTRIES; i++) {
		pr_info("dump_proc_tls:    [%d] {%x, %x, %x, %x, %x, %x, %x, %x, %x, %x, %x, %x, %x}", \
			i, arr[i].limit0, arr[i].base0, arr[i].base1, arr[i].type, \
			arr[i].s, arr[i].dpl, arr[i].p, arr[i].limit1, arr[i].avl, \
			arr[i].l, arr[i].d, arr[i].g, arr[i].base2);
	}
	pr_info("dump_proc_tls: => fsbase = 0x%lx", t->fsbase);
	pr_info("dump_proc_tls: => gsbase = 0x%lx", t->gsbase);
}
