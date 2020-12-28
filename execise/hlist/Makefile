mod_name:=hash_list

ifneq ($(KERNELRELEASE),)
obj-m  := $(mod_name).o

else


# normal makefile
KDIR ?= /lib/modules/`uname -r`/build

default:
	$(MAKE) -C $(KDIR) M=$$PWD

clean:
	$(MAKE) -C $(KDIR) M=$$PWD clean

	
insert:
	sudo insmod $(mod_name).ko
remove:
	sudo rmmod $(mod_name)

test: clean default insert remove
	dmesg -T -k |tail -n 20
endif
