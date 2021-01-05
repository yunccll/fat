MOD_NAME:=lfs
MOUNT_POINT:=/mnt
MOUNT_IMG_PATH:=/home/chenglun/zero.img

LIST_MOD:=$(shell sudo lsmod | grep $(MOD_NAME))

ifneq ($(KERNELRELEASE),)
obj-m :=$(MOD_NAME).o
else
KDIR :=/lib/modules/$(shell uname -r)/build
all:
	make -C $(KDIR) M=$(PWD) modules
install:
	make -C $(KDIR) M=$(PWD) modules_install
help:
	make -C $(KDIR) M=$(PWD) help

h:
	@echo " make open \n make list \n make insert/remove \n make mount/umount\n"
	

list:
	@echo $(LIST_MOD)

insert:
	sudo insmod $(MOD_NAME).ko 
mount:
	sudo mount -t $(MOD_NAME) $(MOUNT_IMG_PATH) $(MOUNT_POINT) -o loop
umount:
	sudo umount $(MOUNT_POINT)
remove:
	sudo rmmod $(MOD_NAME) && dmesg -T  | tail -20

open:
	sudo ./test/main $(MOUNT_POINT)/abc.txt

clean:
	make -C $(KDIR) M=$(PWD) clean
	#rm -f *.ko *.o *.mod.o *.mod.c *.symvers *.order



endif
