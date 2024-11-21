#include <linux/module.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <asm/segment.h>
#include <asm/uaccess.h>
#include <linux/buffer_head.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("s2xxx");

#define MOD_NAME "Module Lab3_1 "

#define M_SIZE_MAX (10)
#define OUTPUT_FILE_NAME "/tmp/output.txt"
#define UINT32_MAX_STR	(12) /* 4294967295 & space */

static int m_size = 2;
static int m_min_val = 0;
static int m_max_val = 100;

module_param(m_size, int, 0644);
MODULE_PARM_DESC(m_size, "Massiv size (1..10)"); /* todo M_SIZE_MAX as param */

module_param(m_min_val, int, 0644);
MODULE_PARM_DESC(m_min_val, "Massiv minimum value");

module_param(m_max_val, int, 0644);
MODULE_PARM_DESC(m_max_val, "Massiv maximum value");

#define CHECK_VAL(_min, _max, _v) ((_min<(_v))&&(_max>(_v)))

int init_module(void)
{
	int rv = -EPERM, i = 0, cnt = 0, sum = 0, val = 0, len;
	loff_t pos = 0;
	char *m = NULL; /* TODO: m is not need cause all operation is calculate imediatly */
	struct file *o_fp = NULL;
	char outbuff[UINT32_MAX_STR] = {0};
	if ((0 < m_size) && (M_SIZE_MAX >= m_size) &&
			(0 <= m_min_val) && (0 < m_max_val) &&
			(m_min_val < m_max_val)
			)
	{
		cnt = m_size * m_size;
		if (NULL != (m = kmalloc(cnt, GFP_KERNEL)))
		{
			printk(KERN_INFO MOD_NAME ": massiv size %i\n", m_size);

			o_fp = filp_open(OUTPUT_FILE_NAME, O_RDWR | O_CREAT, 0644);
			if (IS_ERR(o_fp))
			{
				printk(KERN_WARNING MOD_NAME "Can`t access to %s\n", OUTPUT_FILE_NAME);
			}

			for (i = 0; i < cnt; i++)
			{
				/* TODO: need find more good solution */
				val = get_random_u32();
				while (!CHECK_VAL(m_min_val, m_max_val, val))
				{
					val = get_random_u32();
				}
				m[i] = val;

				/* Print now for prevent also one for-loop */
				if ((0 != i) && (0 == (i % cnt))) /* 0 != i for skip first iteration */
				{
					len = sprintf(outbuff, "\n");
					kernel_write(o_fp, outbuff, len, &pos);
				}
				if (!IS_ERR(o_fp))
				{
					len = sprintf(outbuff, "%d ", val);
					kernel_write(o_fp, outbuff, len, &pos);
				}
				sum += m[i];
			}

			printk(KERN_INFO MOD_NAME "\n summ is %i\n", sum);

			if (!IS_ERR(o_fp))
			{
				len = sprintf(outbuff, "\n");
				kernel_write(o_fp, outbuff, len, &pos);
				len = sprintf(outbuff, "sum:");
				kernel_write(o_fp, outbuff, len, &pos);
				len = sprintf(outbuff, "%d ", sum);
				kernel_write(o_fp, outbuff, len, &pos);
				filp_close(o_fp, NULL);
			}

			kfree(m);
			rv = 0;
		}
		else
		{
			printk(KERN_ALERT MOD_NAME "Can`t allocate memory %d\n", cnt);
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
