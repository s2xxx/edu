#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENCE("GPL");
MODULE_AUTHOR("s2xxx");


static int __init hello_init(void)
{
	printk(KERN_ALERT "In Hello-module\n");
	return 0;
}

static void __init hello_exit(void)
{
	printk(KERN_ALERT "Out from hello-module\n");
	return;
}

module_init(hello_init);
module_exit(hello_exit);
