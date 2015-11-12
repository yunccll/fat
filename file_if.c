#include "file_if.h"

#include "common.h"
#include "fat_util.h"


#define MAX_PATH_LEN            (4*1024)
#define PATH_SEP_STR            "/"
#define PATH_SEP_CHAR           '/'

//error -1 
    //1. parse the path;
    //2. iterator the path_item;
    //3. find the dir entry
    //4. see the dir is exist or not
int fat_file_is_exist(const char * full_path){

    if(full_path[0] == PATH_SEP_CHAR){

        char path[MAX_PATH_LEN] = {0};
        strncpy(path, full_path+1, sizeof(path)-1);

        char * fld = 0;
        char * ppath = path;
        while( (fld = _strsep(&ppath, PATH_SEP_STR)) != NULL){
            if(strlen(fld) > 0)
                printf("fld : %s   \n", fld); 
        }
        return 0;
    }
    FAT_ERROR("full_path[%s] is not prefix with [root_dir /] \n", full_path);
    return -1;
}
