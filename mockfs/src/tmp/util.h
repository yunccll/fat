#ifndef  UTIL_H 
#define  UTIL_H

#include "base.h"


//#define is_even(cluster_no)   (((cluster_no)&0x01) == 0)
//#define cluster_offset_even(cluster_no)     (((cluster_no)*3)>>1)
//#define is_odd(cluster_no)   (((cluster_no)&0x01) == 0x01)
//#define cluster_offset_odd(cluster_no)      (((((cluster_no)-1)*3)>>1)+1)
//#define byte_array_get_uint16(addr, offset)  (*((unsigned short*)(((char*)(addr)) + offset))

int is_even(sector_t cluster_no);
int is_odd(sector_t cluster_no);
uint64_t offset_of_cluster_even(sector_t cluster_no);
uint16_t byte_array_get_uint16_even(uint8_t * addr, uint64_t offset);
void byte_array_set_uint16_even(uint8_t *  addr, uint64_t offset, uint64_t val);
uint64_t offset_of_cluster_odd(sector_t cluster_no);
uint16_t byte_array_get_uint16_odd(uint8_t * addr, uint64_t offset);
void byte_array_set_uint16_odd(uint8_t *  addr, uint64_t offset, uint64_t val);

#endif   /* UTIL_H */
