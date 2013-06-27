
typedef struct fat_boot fat_boot_t;
struct fat_boot_ops {
    void (*print)(fat_boot_t *pb);
};
typedef struct fat_boot_ops fat_boot_ops_t;

struct fat_boot {
   int i; 
   fat_boot_ops_t * ops;
}__attribute__((__packed__));
typedef struct fat_boot fat_boot_t;


void fat_boot_print(fat_boot_t * pb);

void fat_boot_init(fat_boot_t * boot);


int fat_boot_read(fat_boot_t * pb, const char * file, int offset);
int fat_boot_write(fat_boot_t * pb, const char * file, int offset);
