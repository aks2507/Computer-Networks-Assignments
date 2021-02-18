obj-m += basic.o

KERNELDIR ?= /lib/modules/$(shell uname -r)/build

all:
	$(MAKE) -C $(KERNELDIR) M=$(shell pwd) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean