#ifndef _P4FS_INCLUDE_
#define _P4FS_INCLUDE_
#pragma once

#include <osxfuse/fuse.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "tree.h"

#ifdef __cplusplus
#define NS_BEGIN extern "C" {
#define NS_END }
#else
#define NS_BEGIN
#define NS_END
#endif


NS_BEGIN

/* String Vector */
struct string_vector {
    char **arr;
    int len, cur;
};
void string_vector_init(struct string_vector *);
void string_vector_set(struct string_vector *, int, char const *);
void string_vector_push(struct string_vector *, char const *, int);
void string_vector_free(struct string_vector *);

/* File System API */
int p4fs_getattr(char const *path, struct stat *stbuf);
int p4fs_open(char const *path, struct fuse_file_info *fi);
int p4fs_read(
    char const *path,
    char *buf,
    size_t size,
    off_t offset,
    struct fuse_file_info *fi);
int p4fs_readdir(
    char const *path,
    void *buf,
    fuse_fill_dir_t filler,
    off_t offset,
    struct fuse_file_info *fi);

/* Perforce API */
int p4_init(void);
void p4_shutdown(void);
struct string_vector p4_run(char const *cmd, ...);

/* Debug API */
void dlog(char const *fmt, ...);

/* Retrieval API */
struct tree_node *tree_get_depot_root(char const *name);
struct tree_node *tree_find_child(struct tree_node *node, char const *s, int n);

/* Global data */
extern struct string_vector g_depots;

NS_END

#endif /* _P4FS_INCLUDE_ */

