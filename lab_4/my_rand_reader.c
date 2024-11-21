/*********************************************************************
*
* (C) Copyright QTECH LLC 2024
*
**********************************************************************
*
* @filename   my_rand_reader.c
*
* @purpose
*
* @comments   none
*
* @create     21 нояб. 2024 г.
*
* @author     s2xxx
* @end
*
**********************************************************************/

/* Includes -----------------------------------------------------------------*/
#include <stdio.h>
/* Defines ------------------------------------------------------------------*/
#define FILENAME "/dev/my_rand0"

#define BUFSIZE (20)
/* Enum, struct, union ------------------------------------------------------*/
	/*!<  */
/* Typedefs -----------------------------------------------------------------*/
	/*!<  */
/* Variables ----------------------------------------------------------------*/
/* Function prototype -------------------------------------------------------*/

int main(void)
{
	FILE *f = NULL;
	size_t rv = 0;
	char buff[BUFSIZE] = {0};
	if (NULL != (f = fopen(FILENAME, "rb")))
	{
		if (1 != (rv = fread(buff, sizeof(buff), 1, f)))
		{
			printf("Error %i:%i", __LINE__, rv);
		}
		else
		{
			printf("OK");
		}

		fclose(f);
	}
	else
	{
		printf("Can`t read file %s \n", FILENAME);
	}
}
