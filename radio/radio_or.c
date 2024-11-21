/* Includes -----------------------------------------------------------------*/
#include "radio.h"
/* Defines ------------------------------------------------------------------*/
/* Enum, struct, union ------------------------------------------------------*/
	/*!<  */
/* Typedefs -----------------------------------------------------------------*/
/* Variables ----------------------------------------------------------------*/
static int radio_cnt = 0;
/* Function prototype -------------------------------------------------------*/

/*
 * \purpose	 	Open device
 * \param	 	inode, file
 * \returns	 	status operation
 * \comments
 */
int radio_open(struct inode *inode, struct file *file)
{
	int rv = -EPERM;
	int minor = iminor(file->f_inode);

	if (0 == radio_cnt)
	{
		radio_cnt++;
		printk(KERN_INFO DEV_NAME ": opened radio %d\n", minor);
		rv = 0;
	}
	else
	{
		rv = -EBUSY;
	}

	return rv;
}

/*
 * \purpose	 	Close device
 * \param	 	inode, file
 * \returns	 	status operation
 * \comments
 */
int radio_release(struct inode *inode, struct file *file)
{
	int rv = -EPERM;
	int minor = iminor(file->f_inode);

	radio_cnt--;

	printk(KERN_INFO DEV_NAME ": released radio %d\n", minor);

	rv = 0;

	return rv;
}
