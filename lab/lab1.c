#include <linux/module.h>
#include <linux/random.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("s2xxx");

#define MOD_NAME "Module lab1 "
#define MIN_VAL	0
#define MAX_VAL	100

#define CHECK_VAL(_v) ((MIN_VAL<(_v))&&(MAX_VAL>(_v)))

int init_module(void)
{
	int val = get_random_u8();
	while (!CHECK_VAL(val))
	{
		val = get_random_u8();
	}

	printk(KERN_INFO MOD_NAME ": value %i \n", val);

	return 0;
}

void cleanup_module(void)
{
	printk(KERN_INFO MOD_NAME ": exit\n");
	return;
}
