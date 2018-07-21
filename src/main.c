#include "p4fs.h"


static struct fuse_operations p4fs_ops = {
    .getattr = &p4fs_getattr,
    .open = &p4fs_open,
    .read = &p4fs_read,
    .readdir = &p4fs_readdir,
};

struct string_vector g_depots;
static int
get_depots(void)
{
    string_vector_init(&g_depots);
    struct string_vector res;
    res = p4_run("p4 depots");
    for (int i = 0, n = res.cur; i < n; ++i) {
        if (strncmp(res.arr[i], "Depot ", 6) == 0) {
            int begin = 6, end = 6;
            for (; res.arr[i][end] != ' '; ++end);
            string_vector_push(&g_depots, res.arr[i] + begin, end - begin);
        }
    }
    return 0;
}

static char const *spaces = "                                                ";
static void
iterate(struct tree_node *node, int level)
{
    if (node == 0)
        return;
    struct tree_node *cur;
    for (cur = node; cur != 0; cur = cur->next) {
        int is_file = (cur->attr.mode & S_IFREG) ? 1 : 0;
        printf("%.*s%s%s\n", level * 2, spaces, cur->name, is_file ? "" : "/");
        iterate(cur->child, level + 1);
    }
}

int
main(int argc, char **argv)
{
    if (p4_init())
        return 1;

    if (get_depots())
        return 1;

#if 0
    printf(" === DEPOTS ===\n");
    for (int i = 0; i < g_depots.cur; ++i) {
        printf("  - %s\n", g_depots.arr[i]);
    }
    printf(" ==============\n");

    printf(" === FILES ===\n");
    struct tree_node *root = get_depot_root("zc");
    iterate(root, 0);
    printf(" =============\n");
#endif

    int c = 0;
    c = fuse_main(argc, argv, &p4fs_ops, 0);
    p4_shutdown();
    return c;
}
