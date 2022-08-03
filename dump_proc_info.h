/* tls.c */
void dump_tls(struct task_struct *tsk);

/* io.c */
void dump_files(struct task_struct *tsk);

/* task_struct.c */
void dump_ids(struct task_struct *tsk);
void dump_pgd(struct task_struct *tsk);
