#include <linux/slab.h>
#include <linux/fdtable.h>

void dump_files(struct task_struct *tsk)
{
	int idx;
	struct files_struct *f	= tsk->files;
	char *filepath, *tmp = (char *)kmalloc(PAGE_SIZE, GFP_KERNEL);

	pr_info("dump_proc_files: => count = %d, next_fd = %d\n", f->count.counter, f->next_fd);
	for (idx = 0; idx < NR_OPEN_DEFAULT; idx++) {
		if (f->fd_array[idx]) {
			filepath = d_path(&f->fd_array[idx]->f_path, tmp, PAGE_SIZE);
			pr_info("dump_proc_files: => fd_array[%d] = %s,\tf_pos = 0x%llx\n", \
				idx, filepath, f->fd_array[idx]->f_pos);
		}
	}
	kfree(tmp);
}
