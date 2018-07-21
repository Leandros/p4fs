#include "p4fs.h"

NS_BEGIN

static int
fill_path(
    struct tree_node *root,
    char const *path,
    int n,
    void *buf,
    fuse_fill_dir_t filler)
{
    dlog("fill_path: %s\n", path);

	int begin, end;
	struct tree_node *parent = root;
	struct tree_node *cur = root;
	for (begin = 1, end = 1; begin < n; begin = ++end) {
		while (path[end] != '/' && end < n) end++;
        if ((cur = tree_find_child(parent, path + begin, end - begin)) == 0)
            return -ENOENT;
        parent = cur;
	}

    if (!(cur->attr.mode & S_IFDIR))
        return -ENOENT;

    struct tree_node *node = cur->child;
    for (; node != 0; node = node->next) {
        if (node->name) {
            filler(buf, node->name, 0, 0);
        }
    }
    return 0;
}

int
p4fs_readdir(
    char const *path,
    void *buf,
    fuse_fill_dir_t filler,
    off_t offset,
    struct fuse_file_info *fi)
{
    dlog("readdir: %s\n", path);

    size_t path_len = strlen(path);
    /* Fill the depots. */
    if (path_len == 1 && path[0] == '/') {
        filler(buf, ".", 0, 0);
        filler(buf, "..", 0, 0);
        for (int i = 0, n = g_depots.cur; i < n; ++i)
            filler(buf, g_depots.arr[i], 0, 0);
        return 0;
    }

    /* Content of depots. */
    else if (path_len > 1 && path[0] == '/') {
        for (int i = 0, n = g_depots.cur; i < n; ++i) {
            for (int i = 0, n = g_depots.cur; i < n; ++i) {
                char *depot = g_depots.arr[i];
                int depot_len = strlen(depot);
                if (strncmp(path + 1, depot, depot_len) == 0) {
                    struct tree_node *root = tree_get_depot_root(depot);
                    int new_path_len = path_len - depot_len - 1;
                    char const *new_path = path + depot_len + 1;
                    return fill_path(root, new_path, new_path_len, buf, filler);
                }
            }
        }
    }

    /* Not found. */
    return -ENOENT;
}

NS_END
