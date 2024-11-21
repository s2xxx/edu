#include <linux/module.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <asm/segment.h>
#include <asm/uaccess.h>
#include <linux/buffer_head.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("s2xxx");

#define MOD_NAME "Module Lab3 "

#define M_SIZE_MAX (10)
#define OUTPUT_FILE_NAME "/tmp/output.txt"

static int m_size = 2;
static int m_min_val = 0;
static int m_max_val = 100;

module_param(m_size, int, 0644);
MODULE_PARM_DESC(m_size, "Massiv size (1..10)"); // todo M_SIZE_MAX

module_param(m_min_val, int, 0644);
MODULE_PARM_DESC(m_min_val, "Massiv minimum value");

module_param(m_max_val, int, 0644);
MODULE_PARM_DESC(m_max_val, "Massiv maximum value");

#define CHECK_VAL(_min, _max, _v) ((_min<(_v))&&(_max>(_v)))

int init_module(void)
{
	int rv = -EPERM, i = 0, cnt = 0, sum = 0, val = 0;
	loff_t pos = 0;
	char *m = NULL;
	struct file *o_fp;
	if ((0 < m_size) && (M_SIZE_MAX >= m_size) &&
			(0 <= m_min_val) && (0 < m_max_val) &&
			(m_min_val < m_max_val)
			)
	{
		cnt = m_size * m_size;
		if (NULL != (m = kmalloc(cnt, GFP_KERNEL)))
		{
			printk(KERN_INFO MOD_NAME ": massiv size %i\n", m_size);

			for (i = 0; i < cnt; i++)
			{
				val = get_random_u8();
				while (!CHECK_VAL(m_min_val, m_max_val, val))
				{
					val = get_random_u8();
				}
				m[i] = val;

				/* Print now for prevent also one for-loop */
				if (0 == (i % cnt))
				{
					printk(KERN_INFO MOD_NAME "\n");
				}
				printk(KERN_INFO MOD_NAME "%i", m[i]);

				sum += m[i];
			}

			printk(KERN_INFO MOD_NAME "\n summ is %i\n", sum);

			o_fp = filp_open(OUTPUT_FILE_NAME, O_RDWR | O_CREAT, 0644);
			if (!IS_ERR(o_fp)){
				printk(KERN_INFO "output file open error/n");
				pos = 0;
				kernel_write(o_fp, m, cnt, &pos);
				kernel_write(o_fp, &sum, sizeof(sum), &pos);
				filp_close(o_fp, NULL);
			}
			else
			{
				printk(KERN_WARNING MOD_NAME "Can`t access to %s\n", OUTPUT_FILE_NAME);
			}

			kfree(m);
			rv = 0;
		}
	}
	else
	{
		printk(KERN_ALERT MOD_NAME "Invalid PARAM\n");
	}

	return rv;
}

void cleanup_module(void)
{
	printk(KERN_INFO MOD_NAME ": exit\n");
	return;
}
