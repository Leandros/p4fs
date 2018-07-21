#ifndef _TREE_INCLUDE_H_
#define _TREE_INCLUDE_H_
#pragma once

struct fs_attr {
    int mode;
    int size;
};

struct tree_node {
    char *name;
    struct fs_attr attr;
    struct tree_node *child;
    struct tree_node *next;
};

#define NEW_TREE_NODE() calloc(sizeof(struct tree_node), 1)

#endif /* _TREE_INCLUDE_H_ */

