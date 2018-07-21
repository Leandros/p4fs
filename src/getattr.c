#include "p4fs.h"

NS_BEGIN

static int
search_file(struct tree_node *root, char const *path, int n, struct stat *stbuf)
{
    dlog("search_file: %s\n", path);

	int begin, end;
	struct tree_node *parent = root;
	struct tree_node *cur = root;
	for (begin = 1, end = 1; begin < n; begin = ++end) {
		while (path[end] != '/' && end < n) end++;
        if ((cur = tree_find_child(parent, path + begin, end - begin)) == 0)
            return -ENOENT;
        parent = cur;
	}

	stbuf->st_mode = cur->attr.mode;
	stbuf->st_size = cur->attr.size;
    return 0;
}

int
p4fs_getattr(char const *path, struct stat *stbuf)
{
    size_t path_len = strlen(path);
    dlog("getattr: %s\n", path);
    /* The root directory. */
    if (path_len == 1 && path[0] == '/') {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 3;
        return 0;
    }
    /* Our depots. */
    else if (path_len > 1 && path[0] == '/') {
        /* The depot root. */
        for (int i = 0, n = g_depots.cur; i < n; ++i) {
            if (strcmp(path + 1, g_depots.arr[i]) == 0) {
                stbuf->st_mode = S_IFDIR | 0755;
                stbuf->st_nlink = 3;
                return 0;
            }
        }

        /* The depot file tree. */
        for (int i = 0, n = g_depots.cur; i < n; ++i) {
            char *depot = g_depots.arr[i];
            int depot_len = strlen(depot);
            if (strncmp(path + 1, depot, depot_len) == 0) {
                struct tree_node *root = tree_get_depot_root(depot);
                int new_path_len = path_len - depot_len - 1;
                char const *new_path = path + depot_len + 1;
                return search_file(root, new_path, new_path_len, stbuf);
            }
        }

    }

    /* Not found. */
    return -ENOENT;
}

NS_END
