#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("s2xxx");

#define MOD_NAME "Module Lab2 "

#define MIN_VAL	0
#define MAX_VAL	100

static int m_size = 2;

module_param(m_size, int, 0644);
MODULE_PARM_DESC(m_size, "Massiv size (1..100)");

int init_module(void)
{
	int rv = -EPERM, i = 0, cnt = 0, sum = 0;
	char *m = NULL;
	if ((MIN_VAL < m_size) && (MAX_VAL > m_size))
	{
		cnt = m_size * m_size;
		if (NULL != (m = kmalloc(cnt, GFP_KERNEL)))
		{
			printk(KERN_INFO MOD_NAME ": massiv size %i\n", m_size);
			for (i = 0; i < cnt; i++)
			{
				m[i] = get_random_u8();

				/* Print now for prevent also one for-loop */
				if (0 == (i % cnt))
				{
					printk(KERN_INFO MOD_NAME "\n");
				}
				printk(KERN_INFO MOD_NAME "%i", m[i]);

				sum += m[i];
			}

			printk(KERN_INFO MOD_NAME "\n summ is %i\n", sum);

			kfree(m);
			rv = 0;
		}
	}

	return rv;
}

void cleanup_module(void)
{
	printk(KERN_INFO MOD_NAME ": exit\n");
	return;
}
