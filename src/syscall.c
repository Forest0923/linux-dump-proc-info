#include <linux/printk.h>
#include <asm/unistd.h>

void check_sys_clone(void)
{
#ifdef __ARCH_WANT_SYS_CLONE
	pr_info("dump_proc_sys_clone: __ARCH_WANT_SYS_CLONE is defined\n");
#ifdef CONFIG_CLONE_BACKWARDS
	pr_info("dump_proc_sys_clone: CONFIG_CLONE_BACKWARDS is defined\n");
#elif defined(CONFIG_CLONE_BACKWARDS2)
	pr_info("dump_proc_sys_clone: CONFIG_CLONE_BACKWARDS2 is defined\n");
#elif defined(CONFIG_CLONE_BACKWARDS3)
	pr_info("dump_proc_sys_clone: CONFIG_CLONE_BACKWARDS3 is defined\n");
#else
	pr_info("dump_proc_sys_clone: CONFIG_CLONE_BACKWARDS* is NOT defined\n");
#endif
#else
	pr_info("dump_proc_sys_clone: __ARCH_WANT_SYS_CLONE is NOT defined\n");
#endif
}
