/* Includes -----------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/fs.h>
/* Defines ------------------------------------------------------------------*/
#define MOD_NAME "Module Lab4 "
#define DEV_NAME "my_rand"

#define M_SIZE_MAX (10)
#define M_DEV_NUM_DEF (442)

#define CHECK_VAL(_min, _max, _v) ((_min<(_v))&&(_max>(_v)))
/* Enum, struct, union ------------------------------------------------------*/
	/*!<  */
/* Typedefs -----------------------------------------------------------------*/
	/*!<  */
/* Variables ----------------------------------------------------------------*/
static int m_size = 2;
static int m_min_val = 0;
static int m_max_val = 100;
static int m_dev_num = M_DEV_NUM_DEF;
static int my_rand_major = 0;
static struct file_operations fops;
static int already_opened = 0;
DECLARE_WAIT_QUEUE_HEAD(WaitQ);
static char *array = NULL;
/* Function prototype -------------------------------------------------------*/
static int m_size_set(const char *val, const struct kernel_param *kp);

static const struct kernel_param_ops kp_ops =
{
	.set = m_size_set,
	.get = param_get_int
};

module_param_cb(m_size, &kp_ops, &m_size, 0644);
MODULE_PARM_DESC(m_size, "Massiv size (1..10)");

module_param(m_dev_num, int, 0644);
MODULE_PARM_DESC(m_dev_num, "Device number");

module_param(m_min_val, int, 0644);
MODULE_PARM_DESC(m_min_val, "Massiv minimum value");

module_param(m_max_val, int, 0644);
MODULE_PARM_DESC(m_max_val, "Massiv maximum value");


/*
 * \purpose	 	Regenerate buffer for output
 * \param
 * \returns
 * \comments	Suppose that all variable valid
 * 				TODO: only char format supported
 */
static void my_rand_regenerate(void)
{
	char val = 0;
	int i = 0;
	for (i = 0; i < m_size; i++)
	{
		val = get_random_u8();
		while (!CHECK_VAL(m_min_val, m_max_val, val))
		{
			val = get_random_u8(); /* bad function cause 0..255 */
		}
		array[i] = val;
	}

	return;
}

/*
 * \purpose	 	Generate buffer for output
 * \param
 * \returns
 * \comments	Suppose that all variable valid
 */
static int my_rand_generate_with_new_size(void)
{
	int rv = -1;

	if (NULL != array)
	{
		kfree(array);
	}

	if (NULL != (array = kmalloc(m_size * sizeof(char), GFP_KERNEL)))
	{
		my_rand_regenerate();
		rv = 0;
	}
	else
	{
		/* TODO: and now??? */
		printk(KERN_ERR DEV_NAME ": can`t allocate memory\n");
		rv = -ENOMEM;
	}

	return rv;
}

/*
 * \purpose		Set new value of m_size
 * \param
 * \returns
 * \comments
 */
static int m_size_set(const char *m_size_str, const struct kernel_param *kp)
{
	int rv = -EINVAL, ret = 0, val = 0;

	ret = kstrtoint(m_size_str, 10, &val);

	if ((0 == ret) && (val > 0) && (val < M_SIZE_MAX))
	{
		if (m_size != val)
		{
			printk(KERN_INFO MOD_NAME "set new m_size = %d\n", val);
			rv = my_rand_generate_with_new_size();
		}
		else
		{
			printk(KERN_INFO MOD_NAME "set new m_size is old %d\n", val);
			my_rand_regenerate();
		}
	}
	else
	{
		printk(KERN_INFO MOD_NAME "Invalid param\n");
	}

	return rv;
}

/*
 * \purpose	 	Open device
 * \param	 	inode, file
 * \returns	 	status operation
 * \comments
 */
static int my_rand_open(struct inode *inode, struct file *file)
{
	int rv = -EPERM, i = 0, is_sig = 0;

	printk(KERN_INFO DEV_NAME ": my_rand_open\n");

	if ((already_opened) && (file->f_flags & O_NONBLOCK))
	{
		rv = -EAGAIN;
	}
	else
	{
		while (already_opened)
		{
			wait_event_interruptible(WaitQ, !already_opened);
			for (i = 0; i <_NSIG_WORDS && !is_sig; i++)
			{
				is_sig = current->pending.signal.sig[i] & ~current->blocked.sig[i];
			}

			/* TODO: work with rv */
			if (is_sig)
			{
				rv = -EINTR;
				break; //from while
			}
		}
		already_opened = 1;
		rv = 0;
	}

	return rv;
}

/*
 * \purpose	 	Close device
 * \param	 	inode, file
 * \returns	 	status operation
 * \comments
 */
static int my_rand_release(struct inode *inode, struct file *file)
{
	int rv = -EPERM;
	printk(KERN_INFO DEV_NAME ": node_release\n");

	rv = already_opened = 0;
	my_rand_regenerate();
	wake_up(&WaitQ);

	return rv;
}

/*
 * \purpose	 	Read
 * \param
 * \returns	 	ssize
 * \comments
 */
ssize_t my_rand_read(struct file *file, char* buff, size_t length, loff_t* offset)
{
	ssize_t rv = -1;
	int ofs = *offset;
	char *p = NULL;

	int minor = iminor(file->f_inode);

	printk(KERN_INFO DEV_NAME ": try read radio %d ofs %d\n", minor, ofs);

	if (0 == minor)
	{
		if (ofs >= m_size)
		{
			printk(KERN_INFO DEV_NAME ": ofs %d >= msg_length %d\n", ofs, m_size);
			rv = 0;
		}
		else
		{
			if (ofs + length > m_size)
			{
				length = m_size - ofs;
			}

			*offset += length;
			rv = length;

			for (p = array + ofs; length > 0; p++, length--, buff++)
			{
				put_user(*p, buff);
			}
		}
	}
	else
	{
		rv = -EINVAL;
	}

	return rv;
}

/*
 * \purpose	 	Write
 * \param
 * \returns	 	ssize
 * \comments
 */
static ssize_t my_rand_write(struct file *file, const char* buff, size_t length, loff_t* offset)
{
	printk(KERN_INFO DEV_NAME ": try my_rand_write (%d)\n", iminor(file->f_inode));
	return -EACCES;
}

/*
 * \purpose	 	Init
 * \param
 * \returns
 * \comments
 */
int init_module(void)
{
	int rv = -EPERM;

	memset(&fops, 0, sizeof(fops));
	fops.owner = THIS_MODULE;
	fops.open = my_rand_open;
	fops.release = my_rand_release;
	fops.read = my_rand_read;
	fops.write = my_rand_write;

	if ((0 < m_size) && (M_SIZE_MAX > m_size) &&
			(0 <= m_min_val) && (0 < m_max_val) &&
			(m_min_val < m_max_val)
	)
	{
		my_rand_major = register_chrdev(m_dev_num, DEV_NAME, &fops);

		if (0 < my_rand_major)
		{
			printk(KERN_INFO DEV_NAME ": major number %d\n", my_rand_major);
			rv = my_rand_generate_with_new_size();
		}
		else
		{
			rv = -EPERM;
			printk(KERN_ERR DEV_NAME ":Registering the character device failed with %d\n", my_rand_major);
		}
	}
	else
	{
		printk(KERN_ALERT MOD_NAME "Invalid PARAM\n");
		rv = -EINVAL;
	}

	return rv;
}

void cleanup_module(void)
{
	unregister_chrdev(my_rand_major, DEV_NAME);
	kfree(array);
	printk(KERN_INFO MOD_NAME ": cleanup (%i)\n", my_rand_major);
	return;
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("s2xxx");
