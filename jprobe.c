#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/kprobes.h>
#include<linux/ptrace.h>
MODULE_LICENSE("GPL");
MODULE_AUTHOR("shashank parsi");
MODULE_DESCRIPTION("A simple module to demonstrate jprobe functionality");

static long jdo_fork(unsigned long clone_flags, unsigned long stack_start,
	      struct pt_regs *regs, unsigned long stack_size,
	      int __user *parent_tidptr, int __user *child_tidptr)
{
	printk(KERN_INFO "jprobe: clone_flags = 0x%lx, stack_size = 0x%lx,"
			" regs = 0x%p\n",
	       clone_flags, stack_size, regs);
	jprobe_return();
	return 0;
}


static struct jprobe my_jprobe = {
	.entry = jdo_fork,
        .kp = {
		.symbol_name = "do_fork",
		},
};

static int __init test_hello_init(void)
{
	int ret;
	ret = register_jprobe(&my_jprobe);
	if(ret < 0)
	{
		pr_info("registering jprobe has been failed\n");
		return -1;
	}
	printk(KERN_INFO "registered jprobe at : %p and handler addr : %p\n", my_jprobe.kp.addr,my_jprobe.entry);
	return 0;
}

static void __exit test_hello_exit(void)
{
	unregister_jprobe(&my_jprobe);
	printk(KERN_INFO "unregistering the jprobe at %p\n",my_jprobe.kp.addr);
	
}

module_init(test_hello_init);
module_exit(test_hello_exit);

