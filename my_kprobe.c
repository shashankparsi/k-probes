#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/kprobes.h>
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A simple kprobe module");
MODULE_AUTHOR("shashank parsi");

static unsigned int counter=0;
static struct kprobe kp;

static int my_pre_handler(struct kprobe *p,struct pt_regs *regs)
{
	printk(KERN_INFO " %s: counter=%d\n",__func__,counter++);
	printk(KERN_INFO "p->addr: 0x%p, ip=%lx and flags=%lx\n",p->addr,regs->ip,regs->flags);
	//dump_stack();
	return 0;
}

static void my_post_handler(struct kprobe *p,struct pt_regs *regs,unsigned long flags)
{
	printk(KERN_INFO "%s: counter=%d\n",__func__,counter++);
	printk(KERN_INFO "p->addr: 0x%p, ip=%lx and flags=%lx\n",p->addr,regs->ip,regs->flags);
}

static int my_fault_handler(struct kprobe *p,struct pt_regs *regs,int trapnr)
{
	printk(KERN_INFO "%s: counter=%d\n",__func__,counter++);
	printk(KERN_INFO "p->addr: 0x%p, trap : #%d\n", p->addr,trapnr);
	return 0;
}

static int test_hello_init(void)
{
	pr_info("%s: in init function\n",__func__);
	kp.pre_handler = my_pre_handler;
	kp.post_handler = my_post_handler;
	kp.fault_handler = my_fault_handler;
	kp.symbol_name = "_do_fork";
	//kp.symbol_name = "_do_vfs_read";
	//kp.symbol_name = "_do_vfs_write";
	printk(KERN_INFO "registerd kprobe at %p\n",kp.addr);
	register_kprobe(&kp);
	return 0;
}

static void test_hello_exit(void)
{
	pr_info("%s in exit function\n",__func__);
	unregister_kprobe(&kp);
	printk(KERN_INFO "unregistering kprobe at %p\n",kp.addr);
}

module_init(test_hello_init);
module_exit(test_hello_exit);
