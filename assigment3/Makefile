ifeq ($(KERNELRELEASE),)
	# Assume the source tree is where the running kernel was built
	# You should set KERNELDIR in the environment if it's elsewhere
	KERNELDIR ?= /lib/modules/$(shell uname -r)/build

	# The current directory is passed to sub-makes as argument
	PWD := $(shell pwd)

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

modules_install:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules_install
	
clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions
	rm userProcessList
.PHONY: modules modules_install clean
load:
	sudo insmod ./kernelProcessList.ko
unload:
	sudo rmmod kernelProcessList
access:
	sudo chmod 777 /dev/process_list
run:userProcessList.c
	gcc -o userProcessList userProcessList.c
	./userProcessList
else
	# called from kernel build system: just declare what our modules are
	obj-m := kernelProcessList.o   
endif