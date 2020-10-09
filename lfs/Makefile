ifneq ($(KERNELRELEASE),)
obj-m :=lfs.o
else
KDIR :=/lib/modules/$(shell uname -r)/build
all:
	make -C $(KDIR) M=$(PWD) modules
install:
	make -C $(KDIR) M=$(PWD) modules_install
help:
	make -C $(KDIR) M=$(PWD) help
clean:
	make -C $(KDIR) M=$(PWD) clean
	#rm -f *.ko *.o *.mod.o *.mod.c *.symvers *.order
endif
