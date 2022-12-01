#include <linux/sched.h>
#include <linux/fdtable.h>
#include <linux/mm_types.h>
#include <linux/slab.h>
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

void dump_mm_params(struct task_struct *tsk)
{
	struct mm_struct *mm = tsk->mm;

	pr_info("dump_proc_mm_struct: => start_code = 0x%lx, end_code = 0x%lx, start_data = 0x%lx, end_data = 0x%lx\n",
					mm->start_code, mm->end_code, mm->start_data, mm->end_data);
	pr_info("dump_proc_mm_struct: => start_brk = 0x%lx, brk = 0x%lx, start_stack = 0x%lx\n",
					mm->start_brk, mm->brk, mm->start_stack);
	pr_info("dump_proc_mm_struct: => arg_start = 0x%lx, arg_end = 0x%lx, env_start = 0x%lx, env_end = 0x%lx\n",
					mm->arg_start, mm->arg_end, mm->env_start, mm->env_end);
}

void dump_tsk_vma(struct task_struct *tsk)
{
	struct vm_area_struct *vma = tsk->mm->mmap;
	unsigned long start_addr_prev = 0;
	char *filepath, *tmp = (char *)kmalloc(PAGE_SIZE, GFP_KERNEL);

	while(vma != NULL && start_addr_prev < vma->vm_start) {
		if (vma->vm_file) {
			filepath = d_path(&vma->vm_file->f_path, tmp, PAGE_SIZE);
			pr_info("dump_proc_vma: vm_start = 0x%lx, vm_end = 0x%lx, f_pos = %llx, vm_pgoff = %lx, d_iname = %s\n",
						vma->vm_start, vma->vm_end, vma->vm_file->f_pos, vma->vm_pgoff, filepath);
		} else {
			pr_info("dump_proc_vma: vm_start = 0x%lx, vm_end = 0x%lx\n", vma->vm_start, vma->vm_end);
		}
		start_addr_prev = vma->vm_start;
		vma = vma->vm_next;
	}

	kfree(tmp);
}
