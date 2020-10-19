#ifndef  LFS_UTIL_H
#define  LFS_UTIL_H

#define TRUE  1
#define FALSE 0
typedef unsigned long long sector_t;


//#define is_even(cluster_no)   (((cluster_no)&0x01) == 0)
//#define cluster_offset_even(cluster_no)     (((cluster_no)*3)>>1)
//#define is_odd(cluster_no)   (((cluster_no)&0x01) == 0x01)
//#define cluster_offset_odd(cluster_no)      (((((cluster_no)-1)*3)>>1)+1)
//#define byte_array_get_uint16(addr, offset)  (*((unsigned short*)(((char*)(addr)) + offset))

//TODO: Unit test for all
static inline int is_even(sector_t cluster_no){
    return (cluster_no & 0x01) == 0x00;
}
static sector_t cluster_offset_even(sector_t cluster_no){
    return (cluster_no * 3) >> 1;
}
static inline int is_odd(sector_t cluuster_no){
    return (cluster_no & 0x01) == 0x01;
}
static inline sector_t cluster_offset_odd(sector_t cluster_no){
    return (((cluster_no - 1) *3 ) >> 1) + 1;
}
static inline unsigned short byte_array_get_uint16(char * addr, unsigned long long offset){
    return *(unsigned short*)(addr + offset);
}

#endif   /* LFS_UTIL_H */
