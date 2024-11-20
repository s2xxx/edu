/* Includes -----------------------------------------------------------------*/
#include <linux/module.h>
/* Defines ------------------------------------------------------------------*/
/* Enum, struct, union ------------------------------------------------------*/
	/*!<  */
/* Typedefs -----------------------------------------------------------------*/
	/*!<  */
/* Variables ----------------------------------------------------------------*/
/* Function prototype -------------------------------------------------------*/

MODULE_LICENSE("GPL");
MODULE_AUTHOR("s2xxx");

int my_alert(const char *name, const char *msg)
{
	int rv = -1;
	printk(KERN_ALERT "module '%s' sent:%s\n", name, msg);
	return rv = 0;
}


EXPORT_SYMBOL(my_alert);
