#ifndef  BUFFER_H
#define  BUFFER_H


#define BUFFER_LEN		16*1024*1024

#define  BLOCK_SIZE	 	1024
#define NR_HASH		307

#define BIOS_DISPLAY_MEMORY_START  	640*1024
#define BIOS_DISPLAY_MEMORY_END  	1024*1024


typedef struct buffer_head {
	char * b_data;          /*  pointer to data block (1024 bytes) */

	unsigned long b_blocknr;    /*  block number */
	unsigned short b_dev;       /*  device (0 = free) */

	unsigned char b_uptodate;
	unsigned char b_dirt;       /*  0-clean,1-dirty */

	unsigned char b_count;      /*  users using this block */
	unsigned char b_lock;       /*  0 - ok, 1 -locked */
	void * b_wait; //struct task_struct * b_wait;

	struct buffer_head * b_prev;
	struct buffer_head * b_next;
	struct buffer_head * b_prev_free;
	struct buffer_head * b_next_free;
} buffer_head_t;



typedef struct buffer{
	buffer_head_t * free_list;
	unsigned int nr_buffer;
	buffer_head_t * hash_table[NR_HASH];
	char buf[BUFFER_LEN];
}buffer_t ;

#define buffer_create()     ((buffer_t*)calloc(1, sizeof(buffer_t)))
#define buffer_free(ptr)   do { \
        if(ptr){ free(ptr); ptr = NULL;} \
    }while(0) 

void buffer_show(buffer_t * ptr);
void buffer_head_show(buffer_head_t * bh);

buffer_head_t * buffer_get_block(buffer_t * ptr, int dev, int nrblock);


void buffer_init(buffer_t * ptr);
buffer_head_t * buffer_head_get(buffer_t * ptr);


#endif   /* BUFFER_H */
