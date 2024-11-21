#ifndef RADIO_RADIO_H_
#define RADIO_RADIO_H_

/* Includes -----------------------------------------------------------------*/
#include <linux/module.h>
/* Defines ------------------------------------------------------------------*/
#define DEV_NAME "radio"
#define MOD_NAME "Module-Radio "
/* Enum, struct, union ------------------------------------------------------*/
	/*!<  */
/* Typedefs -----------------------------------------------------------------*/
	/*!<  */
/* Variables ----------------------------------------------------------------*/
/* Function prototype -------------------------------------------------------*/

int radio_open(struct inode*, struct file*);
int radio_release(struct inode*, struct file*);

#endif /* RADIO_RADIO_H_ */
