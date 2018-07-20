#include "p4fs.h"

NS_BEGIN

int
p4fs_readdir(
    char const *path,
    void *buf,
    fuse_fill_dir_t filler,
    off_t offset,
    struct fuse_file_info *fi)
{
    return -ENOENT;
}

NS_END
