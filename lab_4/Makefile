CURRENT = $(shell uname -r)
KERNEL_DIR = /lib/modules/$(CURRENT)/build
PWD = $(shell pwd)

obj-m := my_rand.o

my_rand-objs := lab_4.o 

default:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules
	
clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) clean
	
prog: my_rand_reader.o

my_rand_reader.o: my_rand_reader.c
	gcc my_rand_reader.c -o my_rand_reader