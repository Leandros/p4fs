#include "p4fs.h"
#include <stdarg.h>

NS_BEGIN

void
string_vector_init(struct string_vector *v)
{
    assert(v);
    v->arr = malloc(sizeof(char *) * 32);
    assert(v->arr);
    v->len = 32;
    v->cur = 0;
}

void
string_vector_set(struct string_vector *v, int i, char const *s)
{
    assert(v && v->arr);
    assert(s);
    assert(i < v->cur);
    free(v->arr[i]);
    v->arr[i] = strdup(s);
    assert(v->arr[i]);
}

void
string_vector_push(struct string_vector *v, char const *s, int n)
{
    assert(v && v->arr);
    assert(s);
    if ((v->cur + 1) == v->len) {
        v->len *= 2;
        v->arr = realloc(v->arr, sizeof(char *) * v->len);
        assert(v->arr);
    }

    if (n <= 0) n = strlen(s);
    v->arr[v->cur++] = strndup(s, n);
    assert(v->arr[v->cur - 1]);
}

void
string_vector_free(struct string_vector *v)
{
    assert(v && v->arr);
    for (int i = 0, n = v->cur; i < n; ++i)
        free(v->arr[i]);
    free(v->arr);
    v->arr = v->len = v->cur = 0;
}

void
dlog(char const *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vfprintf(stdout, fmt, args);
    fflush(stdout);
    va_end(args);
}


struct root {
    char *name;
    struct tree_node *root;
};
static struct root roots[32] = {};

struct tree_node *
tree_find_child(struct tree_node *node, char const *s, int n)
{
    struct tree_node *cur = node->child;
    for (; cur != 0; cur = cur->next) {
        if (cur->name && strncmp(cur->name, s, n) == 0)
            return cur;
    }

    return 0;
}

static void
add_to_root(struct tree_node *root, char const *s, int n)
{
    if (s[0] != '/')
        return;

	int begin, end;
	struct tree_node *parent = root;
	struct tree_node *cur = root;
	for (begin = 1, end = 1; begin < n; begin = ++end) {
		while (s[end] != '/' && end < n) end++;
        if ((cur = tree_find_child(parent, s + begin, end - begin)) == 0) {
            cur = NEW_TREE_NODE();
            cur->name = strndup(s + begin, end - begin);
            cur->next = parent->child;
            cur->attr.mode = S_IFDIR | 0755;
            parent->child = cur;
        }

        parent = cur;
	}

    /* Only the last path element is actually a file. */
    cur->attr.mode = S_IFREG | 0644;
    cur->attr.size = 1;
}

static struct tree_node *
construct_tree(char const *name)
{
    int name_len = strlen(name);
    struct tree_node *root = NEW_TREE_NODE();
    root->name = "/";
    root->attr.mode = S_IFDIR | 0755;

    struct string_vector files;
    files = p4_run("p4 files -e //%s/...", name);

    for (int i = 0, n = files.cur; i < n; ++i) {
        char *file = files.arr[i] + 2 + name_len;
        int end = 0;
        for (; file[end] != '#'; ++end);
        add_to_root(root, file, end);
    }

    string_vector_free(&files);
    return root;
}

struct tree_node *
tree_get_depot_root(char const *name)
{
    struct root *cur = &roots[0];
    for (; cur->name != 0; ++cur) {
        if (strcmp(cur->name, name) == 0)
            return cur->root;
    }

    for (int i = 0, n = g_depots.cur; i < n; ++i) {
        if (strcmp(g_depots.arr[i], name) == 0) {
            cur->name = g_depots.arr[i];
            return cur->root = construct_tree(cur->name);
        }
    }

    return 0;
}

NS_END
