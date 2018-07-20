#include "p4fs.h"

static struct fuse_operations p4fs_ops = {
    .getattr = &p4fs_getattr,
    .open = &p4fs_open,
    .read = &p4fs_read,
    .readdir = &p4fs_readdir,
};

int
main(int argc, char **argv)
{

    if (p4_init())
        return 1;

    {
        int i;
        struct string_vector depots;
        depots = p4_run("p4 depots");
        for (i = 0; i < depots.cur; ++i)
            printf("%s\n", depots.arr[i]);
    }

    int c = 0;
    /* int c = fuse_main(argc, argv, &p4fs_ops, 0); */
    p4_shutdown();
    return c;
}
