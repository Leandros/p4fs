#include "p4fs.h"

NS_BEGIN

int
p4fs_open(char const *path, struct fuse_file_info *fi)
{
    dlog("--- OPEN ---\n");
    return -EACCES;
}

NS_END
