#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("s2xxx");

#define MOD_NAME "Module Param2 "

static int m_count1 = 1;
static int m_count2 = 1;

module_param(m_count1, int, 0644);
MODULE_PARM_DESC(m_count1, "integer parameter 1");

static int param_val_set(const char *val, const struct kernel_param *kp)
{
	int rv = -EINVAL, ret = 0, res = 0;

	ret = kstrtoint(val, 10, &res);

	if ((0 == ret) && (res > 0) && (res < 100))
	{
		param_set_int(val, kp);
		printk(KERN_INFO MOD_NAME "value1 = %d, value2 = %d \n", m_count1, m_count2);
		rv = 0;
	}
	else
	{
		printk(KERN_INFO MOD_NAME "Invalid param\n");
	}
	return rv;
}

static const struct kernel_param_ops kp_ops =
{
	.set = param_val_set,
	.get = param_get_int
};

module_param_cb(m_count2, &kp_ops, &m_count2, 0644);
MODULE_PARM_DESC(m_count2, "integer parameter 2");

int init_module(void)
{
	printk(KERN_INFO MOD_NAME "integer1 %i, integer2 %i\n", m_count1, m_count2);
	return 0;
}

void cleanup_module(void)
{
	printk(KERN_INFO MOD_NAME ": exit\n");
	return;
}
