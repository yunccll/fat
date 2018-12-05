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



3. design
    
   fs-> member....
        fat1, 
        fat2 -> cluster
        Root_direntry
        container<Data-cluster>
                     
         fat_item,   entry, name,   Container<data-cluster>
   boot; fat1,fat2;  root_direntry; data;  
   container<block>
   device



3.1 fat_print

    print_mbr
    print_fat1
    print_fat2
    print_root_direntry

demo:
    fat_fs * fs = fat_fs_load("a.img");
    fat_fs_print(fs);{
        fat_boot_print(fs);
        fat_fat_print(fs->fat);
        fat_fat_print(fs->fat_bak);
        fat_root_direntry_print(fs->root);
        fat_data_print(fs->data);
    }
    fat_fs_destroy(fs);



3.2 fat_mkfs
    mk_mbr
    mk_fat1
    mk_fat2
    mk_root_direntry



4.app case

4.1 ls / | ls [path]
4.2 tree / tree [path]
4.3 find / -name "name"
4.4.cp file  file
4.5 cp path path
4.6 mkfs_fat if=a.txt  of=a.img
