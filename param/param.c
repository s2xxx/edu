#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("s2xxx");

#define MOD_NAME "Module Param "

static int m_count = 1;
static char *m_char = "empty";

module_param(m_count, int, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH);
MODULE_PARM_DESC(m_count, "integer parameter");

module_param(m_char, charp, 0664);
MODULE_PARM_DESC(m_char, "string parameter");

int init_module(void)
{
	if (1 == m_count)
	{
		printk(KERN_INFO MOD_NAME "default integer, string %s\n", m_char);
	}
	else
	{
		printk(KERN_INFO MOD_NAME "integer %i, string %s\n", m_count, m_char);
	}

	return 0;
}

void cleanup_module(void)
{
	printk(KERN_INFO MOD_NAME ": exit\n");
	return;
}
