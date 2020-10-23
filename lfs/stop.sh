#!/bin/bash
MOD_NAME=lfs
sudo rmmod $MOD_NAME && dmesg -T  | tail -10
