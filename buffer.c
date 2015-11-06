#include "buffer.h"
#include <stddef.h>
#include <stdio.h>

#define DEBUG_PRINT		printf

// common memory 16M,  buffer_end -> 4M
// [0]<----kernel-- ->[buffer_start]<----buffer--->[BIOS&DisplayMem]<--buffer-->[buffer_end]<------high Mem ----->[mem_end]
// [0]<----kernel-- ->[		   192K]<----buffer--->[640K<------->1M]<--buffer-->[        4M]<------high Mem ----->[    16M]
// [0]<----kernel-- ->[     0x30000]<----buffer--->[A0000<-->100000]<--buffer-->[    400000]<------high Mem ----->[1000000]
// assume the  buffer_head in (192k~640K)
void buffer_init(buffer_t * ptr){
	buffer_head_t * head = (buffer_head_t *)ptr->buf;
	unsigned long end = (long)(ptr->buf + BUFFER_LEN);

	while( (unsigned long)(head + 1) <= (end -= BLOCK_SIZE)){
		if(end >= (unsigned long)(ptr->buf+BIOS_DISPLAY_MEMORY_END) || end <= (unsigned long)(ptr->buf+BIOS_DISPLAY_MEMORY_START)){
			head->b_data = (char*)end;
			head->b_blocknr = 0;
			head->b_dev = 0;
			head->b_uptodate = 0;
			head->b_dirt = 0;

			head->b_prev = NULL;
			head->b_next = NULL;

			head->b_prev_free = head -1;
			head->b_next_free = head + 1;

			++(ptr->nr_buffer);
			++head;
		}
		else{
			end = (unsigned long)(ptr->buf+BIOS_DISPLAY_MEMORY_START);
		}
	}
	head --;
	ptr->free_list = (buffer_head_t*)(ptr->buf);
	ptr->free_list->b_prev_free = head;
	head->b_next_free = ptr->free_list;

	int i;
	for( i=0 ; i < NR_HASH; ++i){
		ptr->hash_table[i] = NULL;
	}
}

static void buffer_head_show_with_offset(buffer_head_t * bh, unsigned long addr){
    printf("\tbuffer address: [0x%08lx]    offset [0x%08lx]\n", (unsigned long)(bh)               ,(unsigned long)(bh)-addr              );
    printf("\tb_data          [0x%08lx]    offset [0x%08lx]\n", (unsigned long)bh->b_data         ,(unsigned long)(bh->b_data) - addr    );
    printf("\tb_prev          [0x%08lx]    offset [0x%08lx]\n", (unsigned long)bh->b_prev         ,(unsigned long)(0)    );
    printf("\tb_next          [0x%08lx]    offset [0x%08lx]\n", (unsigned long)bh->b_next         ,(unsigned long)(0)    );
    printf("\tb_prev_free     [0x%08lx]    offset [0x%08lx]\n", (unsigned long)bh->b_prev_free    ,(unsigned long)(bh->b_prev_free) - addr   );
    printf("\tb_next_free     [0x%08lx]    offset [0x%08lx]\n", (unsigned long)bh->b_next_free    ,(unsigned long)(bh->b_next_free) - addr   );
}
void buffer_head_show(buffer_head_t * bh){
    printf("\tbuffer address: [0x%08lx]\n", (unsigned long)(bh)               );
    printf("\tb_data          [0x%08lx]\n", (unsigned long)bh->b_data         );
    printf("\tb_prev          [0x%08lx]\n", (unsigned long)bh->b_prev         );
    printf("\tb_next          [0x%08lx]\n", (unsigned long)bh->b_next         );
    printf("\tb_prev_free     [0x%08lx]\n", (unsigned long)bh->b_prev_free    );
    printf("\tb_next_free     [0x%08lx]\n", (unsigned long)bh->b_next_free    );
}

void buffer_show(buffer_t * ptr){
	printf("free_list       [0x%08lx]\n",        (unsigned long)ptr->free_list);
	printf("nr_buffer       [0x%08x] [%u]\n",    ptr->nr_buffer,     ptr->nr_buffer);	
	printf("hash_table      [0x%08lx]\n",        (unsigned long)ptr->hash_table);
	printf("buf first addr  [0x%08lx]\n",        (unsigned long)ptr->buf);
    printf("show buf content: ....\n");
    
    buffer_head_t * h = (buffer_head_t*)ptr->buf;
    long i;
    for(i = 0; i < ptr->nr_buffer; ++i, h++){
        printf("[%ld] buffer head :\n", i + 1);
        buffer_head_show_with_offset(h, (unsigned long)ptr->buf);
    }
}
