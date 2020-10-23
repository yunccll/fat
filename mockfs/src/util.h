#ifndef  UTIL_H 
#define  UTIL_H

#include "linux/types.h"


//#define is_even(cluster_no)   (((cluster_no)&0x01) == 0)
//#define cluster_offset_even(cluster_no)     (((cluster_no)*3)>>1)
//#define is_odd(cluster_no)   (((cluster_no)&0x01) == 0x01)
//#define cluster_offset_odd(cluster_no)      (((((cluster_no)-1)*3)>>1)+1)
//#define byte_array_get_uint16(addr, offset)  (*((unsigned short*)(((char*)(addr)) + offset))

//TODO: Unit test for all
static inline int is_even(sector_t cluster_no){  //0 2
    return (cluster_no & 0x01) == 0x00;
}
static inline int is_odd(sector_t cluster_no){ //1 3
    return (cluster_no & 0x01) == 0x01;
}
static uint64_t offset_of_cluster_even(sector_t cluster_no){
    assert(is_even(cluster_no));
    return (cluster_no * 3) >> 1;
}
static inline uint16_t byte_array_get_uint16_even(uint8_t * addr, uint64_t offset){
    assert(addr != NULL);
    return *(uint16_t*)(addr + offset) & 0xfff;
}
static inline void byte_array_set_uint16_even(uint8_t *  addr, uint64_t offset, uint64_t val){
    *(uint16_t*)(addr+ offset) = ((uint16_t)(val & 0xfff)) | (*(uint16_t*)(addr + offset) & 0xf000);
}
static inline uint64_t offset_of_cluster_odd(sector_t cluster_no){
    assert(is_odd(cluster_no));
    return (((cluster_no - 1) *3 ) >> 1) + 1;
}
static inline uint16_t byte_array_get_uint16_odd(uint8_t * addr, uint64_t offset){
    assert(addr != NULL);
    return *(uint16_t*)(addr + offset) >> 4;
}
static inline void byte_array_set_uint16_odd(uint8_t *  addr, uint64_t offset, uint64_t val){
    *(uint16_t*)(addr+ offset) = ((uint16_t(val&0xfff)) << 4) | (*((uint16_t*)(addr + offset)) & 0x0f);
}

#endif   /* UTIL_H */
