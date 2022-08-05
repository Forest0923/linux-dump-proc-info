/* tls.c */
void dump_tls(struct task_struct *tsk);

/* io.c */
void dump_files(struct task_struct *tsk);

/* task_struct.c */
void dump_ids(struct task_struct *tsk);
void dump_pgd(struct task_struct *tsk);
void dump_mm_params(struct task_struct *tsk);
void dump_tsk_vma(struct task_struct *tsk);

/* syscall.c */
void check_sys_clone(void);

/* current.c */
void dump_current(void);

/* page_table_walk.c */
unsigned long page_table_walk(unsigned long addr, struct mm_struct *mm_str);
