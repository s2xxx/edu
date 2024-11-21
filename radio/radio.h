#ifndef RADIO_RADIO_H_
#define RADIO_RADIO_H_

/* Includes -----------------------------------------------------------------*/
#include <linux/module.h>
//#include <linux/uaccess.h>
#include <linux/fs.h>
/* Defines ------------------------------------------------------------------*/
#define DEV_NAME "radio"
#define BUF_SIZE	(1024)
/* Enum, struct, union ------------------------------------------------------*/
	/*!<  */
/* Typedefs -----------------------------------------------------------------*/
	/*!<  */
/* Variables ----------------------------------------------------------------*/
extern char r_buff[BUF_SIZE];
/* Function prototype -------------------------------------------------------*/

int radio_open(struct inode*, struct file*);
int radio_release(struct inode*, struct file*);

ssize_t radio_read(struct file*, char*, size_t, loff_t*);
ssize_t radio_write(struct file*, const char*, size_t, loff_t*);

#endif /* RADIO_RADIO_H_ */
