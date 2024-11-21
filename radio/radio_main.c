/* Includes -----------------------------------------------------------------*/
#include "radio.h"
#include <linux/fs.h>
/* Defines ------------------------------------------------------------------*/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("s2xxx");
/* Enum, struct, union ------------------------------------------------------*/
	/*!<  */
/* Typedefs -----------------------------------------------------------------*/
/* Variables ----------------------------------------------------------------*/
static struct file_operations fops;
static int radio_major = 0;
char r_buff[BUF_SIZE] = {0};
/* Function prototype -------------------------------------------------------*/

int init_module(void)
{
	int rv = -EPERM;
	memset(&fops, 0, sizeof(fops));
	fops.owner = THIS_MODULE;
	fops.open = radio_open;
	fops.release = radio_release;
	fops.read = radio_read;
	fops.write = radio_write;

	radio_major = register_chrdev(0, DEV_NAME, &fops);

	if (0 < radio_major)
	{
		printk(KERN_INFO DEV_NAME ": major number %d\n", radio_major);
		rv = 0;
	}
	else
	{
		printk(KERN_ERR DEV_NAME ":Registering the character device failed with %d\n", radio_major);
	}

	return rv;
}

void cleanup_module(void)
{
	unregister_chrdev(radio_major, DEV_NAME);
	printk(KERN_INFO DEV_NAME ": unregister_chrdev\n");

	return;
}
