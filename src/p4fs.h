#ifndef _P4FS_INCLUDE_
#define _P4FS_INCLUDE_
#pragma once

#include <osxfuse/fuse.h>
#include <errno.h>
#include <stdio.h>

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
void string_vector_push(struct string_vector *, char const *);
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
struct string_vector p4_run(char const *cmd);

NS_END

#endif /* _P4FS_INCLUDE_ */

