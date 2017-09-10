Destination:
===========
    write  fat12 from my view 


1. achitecture

mkfs      open   close   read       write 

super       dentry_cache           data_cache

boot    fat    root   data 


        block_device

2. data structure 

super     
 |--boot (1.sect)
 |          fat1    (9.sect)
 |--fat  -- fat2    (9.sect)
 |--root    (14.sect)
 |   |----  entry    0x220
 |--data (2880-33.sect)
