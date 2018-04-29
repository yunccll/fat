#  overview 
    A read/write util for fat12 (file allocation table) file system

# function list 
## function mapping
        fs      dir     file
create  Y       Y       Y
delete  Y       Y       Y 
show    Y       Y       Y
update          Y       Y
find            Y       Y
mv              Y       Y
cp              Y       Y

## cfs.fs 
1. (show)       cfs fs show     /path/file.img
2. (check)      cfs fs check    /path/file.img
3. (free)       cfs fs free     /path/file.img
4. (delete)     cfs fs blank    /path/file.img
5. (create)     cfs fs format   /path/file.img

## cfs.file 
### cfs.file.meta
1. (create)    cfs file.meta create    cfs://path/file
2. (delete)    cfs file.meta delete    cfs://path/file
3. (show)      cfs file.meta show      cfs://path/file
4. (find)      cfs file.meta find      cfs://path/file keywords
5. (update)    cfs file.meta update    cfs://path/file keywords
6. (link)      cfs file.meta link      cfs://path/from cfs://path/to

### cfs.file.content
1. (create)    cfs file.content create cfs://path/file as external-from|stdin
2. (delete)    cfs file.content delete cfs://path/file      same as cfs.file.meta.delete
3. (show)      cfs file.content show   cfs://path/file     show the content on stdin
4. (search)    cfs file.content search cfs://path/file keywords
5. (update)    cfs file.content update cfs://path/file stdin
6. (move)      cfs file.content mv     cfs://path/from|external-from  cfs://path/to|external-to
            --  cfs://path/from     cfs://path/to
            --  /external/from      cfs://path/to
            --  cfs://path/from     /external/to
            -- /external/from       /external/to

7. (copy)      cfs file.content copy   cfs://path/from|external-from  cfs://path/to|external-to
            --  cfs://path/from     cfs://path/to
            --  /external/from      cfs://path/to
            --  cfs://path/from     /external/to
            -- /external/from       /external/to

## cfs.dir
### cfs.dir.meta
1. (create)    cfs dir.meta create cfs://path/dir
2. (delete)    cfs dir.meta delete cfs://path/dir
3. (show)      cfs dir.meta show   cfs://path/dir  show the dir on stdin
4. (update)    cfs dir.meta update cfs://path/dir meta-pairs

### cfs.dir.content
1. (create)    cfs dir.content create cfs://path/dir
2. (delete)    cfs dir.content delete cfs://path/dir
3. (show)      cfs dir.content show   cfs://path/dir     show the sub-files in content
4. (find)      cfs dir.content find   cfs://path/dir keywords-of-dir-name-piece  dir:parent-dir
5. (move)      cfs dir.content mv     cfs://path/from|external-from  cfs://path/to|external-to
6. (copy)      cfs dir.content copy   cfs://path/from|external-from  cfs://path/to|external-to
7. (used)      cfs dir.content used   cfs://path/dir


# smallest functions
1. cfs.fs.1. (show)       cfs fs show     /path/file.img
2. cfs.fs.2. (check)      cfs fs check    /path/file.img
3. cfs.fs.3. (free)       cfs fs free     /path/file.img
4. cfs.fs.4. (delete)     cfs fs blank    /path/file.img
5. cfs.fs.5. (create)     cfs fs format   /path/file.img
