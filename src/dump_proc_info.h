#include <linux/pgtable.h>
#include <linux/fs.h>
#include <asm/io.h>
#include <linux/fdtable.h>

#define OBJ_MASK_PGD	0x01
#define OBJ_MASK_VMA	0x02
#define OBJ_MASK_MEMREG	0x04
#define OBJ_MASK_TLS	0x08
#define OBJ_MASK_FILE	0x10

int my_printk(const char *fmt, ...);

void dump_tls(struct task_struct *tsk)
{
	int i;
	struct thread_struct *t = &tsk->thread;
	struct desc_struct *arr = tsk->thread.tls_array;

	my_printk("proc_debugger: => tls_array");
	for (i = 0; i < GDT_ENTRY_TLS_ENTRIES; i++) {
		my_printk("proc_debugger:    [%d] {%x, %x, %x, %x, %x, %x, %x, %x, %x, %x, %x, %x, %x}", \
			i, arr[i].limit0, arr[i].base0, arr[i].base1, arr[i].type, \
			arr[i].s, arr[i].dpl, arr[i].p, arr[i].limit1, arr[i].avl, \
			arr[i].l, arr[i].d, arr[i].g, arr[i].base2);
	}
	my_printk("proc_debugger: => fsbase = 0x%lx\n", t->fsbase);
	my_printk("proc_debugger: => gsbase = 0x%lx\n", t->gsbase);
}

void dump_files(struct task_struct *tsk)
{
	int idx;
	struct files_struct *f	= tsk->files;
	char *filepath, *tmp = (char *)kmalloc(PAGE_SIZE, GFP_KERNEL);

	my_printk("proc_debugger: => count = %d, next_fd = %d\n", f->count.counter, f->next_fd);
	for (idx = 0; idx < NR_OPEN_DEFAULT; idx++) {
		if (f->fd_array[idx]) {
			filepath = d_path(&f->fd_array[idx]->f_path, tmp, PAGE_SIZE);
			my_printk("proc_debugger: => fd_array[%d] = %s,\tf_pos = 0x%llx\n", \
				idx, filepath, f->fd_array[idx]->f_pos);
		}
	}
	kfree(tmp);
}

void dump_pgd(struct task_struct *tsk)
{
	unsigned long cr3 = pv_ops.mmu.read_cr3();

	my_printk("proc_debugger: => cr3      = 0x%lx\n", cr3);
	my_printk("proc_debugger: => *pgd     = 0x%lx\n", (unsigned long)tsk->mm->pgd);
	my_printk("proc_debugger: => phys_pgd = 0x%llx\n", virt_to_phys(tsk->mm->pgd));
}

void dump_mm_params(struct task_struct *tsk)
{
	struct mm_struct *mm = tsk->mm;

	my_printk("proc_debugger: => start_code  = 0x%lx, end_code = 0x%lx\n",
					mm->start_code, mm->end_code);
	my_printk("proc_debugger: => start_data  = 0x%lx, end_data = 0x%lx\n",
					mm->start_code, mm->end_code);
	my_printk("proc_debugger: => start_brk   = 0x%lx, brk      = 0x%lx\n",
					mm->start_brk, mm->brk);
	my_printk("proc_debugger: => start_stack = 0x%lx\n",
					mm->start_stack);
	my_printk("proc_debugger: => arg_start   = 0x%lx, arg_end  = 0x%lx\n",
					mm->arg_start, mm->arg_end);
	my_printk("proc_debugger: => env_start   = 0x%lx, env_end  = 0x%lx\n",
					mm->env_start, mm->env_end);
}

void dump_tsk_vma(struct task_struct *tsk)
{
	struct vm_area_struct *vma = tsk->mm->mmap;
	unsigned long start_addr_prev = 0;
	char *filepath, *tmp = (char *)kmalloc(PAGE_SIZE, GFP_KERNEL);

	while(vma != NULL && start_addr_prev < vma->vm_start) {
		if (vma->vm_file) {
			filepath = d_path(&vma->vm_file->f_path, tmp, PAGE_SIZE);
			my_printk("proc_debugger: vm_start = 0x%lx, vm_end = 0x%lx, f_pos = %llx, vm_pgoff = %lx, d_iname = %s\n",
						vma->vm_start, vma->vm_end, vma->vm_file->f_pos, vma->vm_pgoff, filepath);
		} else {
			my_printk("proc_debugger: vm_start = 0x%lx, vm_end = 0x%lx\n", vma->vm_start, vma->vm_end);
		}
		start_addr_prev = vma->vm_start;
		vma = vma->vm_next;
	}

	kfree(tmp);
}

#define PAGE_OFFSET_MASK (~(~0UL << PAGE_SHIFT))

unsigned long page_table_walk(unsigned long addr, struct mm_struct *mm_str)
{
	pgd_t *pgd;
	p4d_t *p4d;
	pud_t *pud;
	pmd_t *pmd;
	pte_t *pte;
	unsigned long pfn, phys_page_addr, page_offset, phys_addr;

	pgd = pgd_offset(mm_str, addr);
	if (pgd_none(*pgd))
		return 0;

	p4d = p4d_offset(pgd, addr);
	if (p4d_none(*p4d))
		return 0;

	pud = pud_offset(p4d, addr);
	if (pud_none(*pud))
		return 0;

	pmd = pmd_offset(pud, addr);
	if (pmd_none(*pmd))
		return 0;

	pte = pte_offset_map(pmd, addr);
	if (!pte || !pte_present(*pte) || pte_none(*pte) || is_zero_pfn(pte_pfn(*pte)))
	{
		return 0;
	}

	pfn = pte_pfn(*pte);
	phys_page_addr = PFN_PHYS(pfn);
	page_offset = addr & PAGE_OFFSET_MASK;
	phys_addr = phys_page_addr | page_offset;

	pte_unmap(pte);

	return phys_addr;
}

