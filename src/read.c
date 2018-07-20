#include "p4fs.h"

NS_BEGIN

int
p4fs_read(
    char const *path,
    char *buf,
    size_t size,
    off_t offset,
    struct fuse_file_info *fi)
{
    return 0;
}

NS_END
