#include "boot.h"
#include <stdint.h>

#include "util.h"
#include "config.h"


#pragma pack(1)

struct fat_s {
    unsigned char number_of_driver;   // 00 -> floppy disk, 0x80 -> hard disks
    unsigned char reserved1;        // 0
    unsigned char boot_signature;    // 0x29 -> Extended boot signature 
    uint32_t volumeId;              //
    char volume_label[11];           //
    char file_system_type[8];         // "FAT12   " "FAT16   " "FAT32   "
};

struct fat32_s {
    uint32_t sector_per_fat32;      //only for Fat32
    uint16_t extend_flag;        //bits 0-3: zero-based number of active Fat, 4-6: reserved, 7: 0/1 8-15: reserved
    uint16_t file_system_version; // 0:0
    uint32_t cluster_number_of_root_directory;  // 2
    uint16_t fs_info;
    uint16_t backup_boot_sector;
    char reserved[12];
    unsigned char driver_number;
    unsigned char reserved1;
};
struct bios_parameter_block_s {
    uint16_t bytes_per_sector;        //512
    unsigned char sector_per_cluster; // TODO: ??
    uint16_t reserved_sector_count;    // 1 -> fat12/16  32 -> fat32
    unsigned char number_of_fats;     // 2
    uint16_t root_entry_count;        // 0 --> fat32 512 --> fat16   220 --> fat12 1.44M
    uint16_t total_sector16;         // must 0 --> FAT32,  real-count for fat12/16
    unsigned char media;            // 0xF8 -> fixed media; 0xF0 --> removable media
    uint16_t sector_per_fat;          // 0 -> fat32, 8 -> fat12,
    uint16_t sector_per_track;        // 
    uint16_t number_of_headers;       //
    uint32_t hidden_sectors;         // hidden sectors preceding this partition; 0 -> no partition
    uint32_t total_sector32;         //
    union fatSpecial {
        struct fat_s fat_boot_sector;
        struct fat32_s fat32_boot_sector;
    } special;
};
struct boot_sector_s{
    char jump_to_boot[3];
    char oem_name[8];
    struct bios_parameter_block_s bpb;
};

#pragma pack()

int fat_boot_parser(fat_boot_t * ptr, char * block_offset, int bytes){
    if(bytes >= DEFAULT_BLOCK_SIZE){
        struct boot_sector_s * boot = (struct boot_sector_s*)block_offset;
        ptr->_total_sector      = boot->bpb.total_sector16;
        ptr->_bytes_per_sector  = boot->bpb.bytes_per_sector;
        ptr->_reserved_sector_count = boot->bpb.reserved_sector_count;
        ptr->_number_of_fats    = boot->bpb.number_of_fats;
        ptr->_sector_per_fat    = boot->bpb.sector_per_fat;

        ptr->_sector_per_cluster = boot->bpb.sector_per_cluster;
        ptr->_root_entry_count  = boot->bpb.root_entry_count;
        ptr->_bytes_per_entry   = 32;                               // 32 as default TODO: ???
        ptr->_media             = boot->bpb.media;
        ptr->_first_sector_of_data = (1 + 9 + 9 + 14);  // 1 + 9 + 9 + 14  TODO:???
        memcpy(ptr-> _file_system_type, &(boot->bpb.special.fat_boot_sector.file_system_type), 8);
        return 0;
    }
    ERROR_PRINT("Error: boot block size < 512bytes \n");
    return -1;
}


fat_boot_t * fat_boot_create(){
    fat_boot_t * ptr = (fat_boot_t *)calloc(sizeof(fat_boot_t), 1);
    IF_TRUE_RETURN(ptr == NULL, NULL, "no enough memory\n");
    return ptr;
}

void fat_boot_destroy(fat_boot_t * ptr){
    if(ptr){
        free(ptr);
    }
}



void fat_boot_print(fat_boot_t * ptr){
    TRACE_PRINT("total_sector:%lld ", ptr->_total_sector);
    TRACE_PRINT(",bytes_per_sector:%d ", ptr->_bytes_per_sector);
    TRACE_PRINT(",reserved_sector_count:%d ", ptr->_reserved_sector_count);
    TRACE_PRINT(",number_of_fats:%d ", ptr->_number_of_fats);
    TRACE_PRINT(",sector_per_fat:%d ", ptr->_sector_per_fat);
    TRACE_PRINT("\n");

    TRACE_PRINT(",sector_per_cluster:%d ", ptr->_sector_per_cluster);
    TRACE_PRINT(",root_entry_count:%d ", ptr->_root_entry_count);
    TRACE_PRINT(",bytes_per_sector:%d ", ptr->_bytes_per_entry);
    TRACE_PRINT("\n");

    TRACE_PRINT(",media:%d ", ptr->_media);
    TRACE_PRINT(",firstSectorOfData: %d ", ptr->_first_sector_of_data);
    TRACE_PRINT(",file_system_type:%s", ptr->_file_system_type);
    TRACE_PRINT("\n");
}
