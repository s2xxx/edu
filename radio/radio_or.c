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

	if (0 == radio_cnt)
	{
		radio_cnt++;
		printk(KERN_INFO MOD_NAME ": opened radio %d\n", radio_cnt);
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

	radio_cnt--;

	printk(KERN_INFO MOD_NAME ": released radio %d\n", radio_cnt);

	rv = 0;

	return rv;
}
