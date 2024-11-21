#include "hello.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("s2xxx");

int init_module(void)
{
	my_alert(MOD_NAME, "In Hello-module");
	return 0;
}
