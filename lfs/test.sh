#!/bin/bash


MOD_NAME=lfs



date  &&  sudo insmod $MOD_NAME.ko 

#sudo rmmod $MOD_NAME && dmesg -T  | tail -10
