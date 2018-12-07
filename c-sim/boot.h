
#ifndef  BOOT_H
#define  BOOT_H


typedef struct fat_boot_s fat_boot_t;

struct fat_boot_s {
    long long _total_sector;    //
    int _bytes_per_sector;      //512 as default
    int _reserved_sector_count; // 1 -> fat12/16  32-> fat32
    int _number_of_fats;        // 2
    int _sector_per_fat;        // 8 -> fat12 0 -> fat32

    int _sector_per_cluster;    // 1 - > fat12  ?? -> fat32
    int _root_entry_count;      // 220 -> fat12  512 -> fat16  0 - > fat32
    int _bytes_per_entry;       // 32 as default
    
    int _media;                 //0xf8 -> fixed media, 0xf0 -> removable media
    int _first_sector_of_data;  // 1 + 9 + 9 + 14 
    char _file_system_type[9];
};

fat_boot_t * fat_boot_create();
void fat_boot_destroy(fat_boot_t * ptr);


int fat_boot_parser(fat_boot_t * ptr, char * block_offset, int bytes);

void fat_boot_print(fat_boot_t * ptr);



#endif   /* BOOT_H */
