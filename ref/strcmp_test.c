static  __inline__ void __set_bit(int nr, volatile void * addr)  
{   
    __asm__(  
            "btsl %1,%0"  
            :"=m" (addr)  
            :"Ir" (nr));  
}

int __strcmp(const char * cs,const char * ct)
{
    int __res;
     __asm__("cld\n"
	"1:\tlodsb\n\t"
	"scasb\n\t"
	"jne 2f\n\t"
	"testb %%al,%%al\n\t"
	"jne 1b\n\t"
	"xorl %%eax,%%eax\n\t"
	"jmp 3f\n"
	"2:\tmovl $1,%%eax\n\t"
	"jl 3f\n\t"
	"negl %%eax\n"
	"3:"
	:"=a" (__res):"D" (cs),"S" (ct)//:"si","di"
    );
    return __res;
}

int ms_strcmp (const char * src, const char * dst)
{
    int ret = 0 ;
    while( ! (ret = *(unsigned char *)src - *(unsigned char *)dst) && *dst)
        ++src, ++dst;
    if ( ret < 0 )
        ret = -1 ;
    else if ( ret > 0 )
        ret = 1 ;
    return( ret );
}

#include <stdio.h>
void test_strcmp()
{
    #define max_count 1024*1024*1024
    int i = max_count;
    while(i--){
        __strcmp("stringssssssssppp", "stringssssssss");
        //strcmp("stringssssssss", "stringssss");
        //ms_strcmp("stringssssssss", "stringssssssss");
    }
}
