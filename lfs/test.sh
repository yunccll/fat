#!/bin/bash


MOD_NAME=lfs

MOUNT_POINT=/mnt

date  

#insert module
sudo insmod $MOD_NAME.ko 

#mount 
sudo mount -t lfs ~/zero.img $MOUNT_POINT -o loop

#umount 
#sudo umount $MOUNT_POINT


#stop
#sudo rmmod $MOD_NAME && dmesg -T  | tail -10
