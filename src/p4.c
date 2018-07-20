#include "p4fs.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

NS_BEGIN

void 
string_vector_init(struct string_vector *v)
{
    v->arr = malloc(sizeof(char *) * 32);
    assert(v->arr);
    v->len = 32;
    v->cur = 0;
}

void 
string_vector_push(struct string_vector *v, char const *s)
{
    if ((v->cur + 1) == v->cur) {
        v->len *= 2;
        v->arr = realloc(v->arr, sizeof(char *) * v->len);
        assert(v->arr);
    }

    v->arr[v->cur++] = strdup(s);
}

void 
string_vector_free(struct string_vector *v)
{
    int i, n;
    for (i = 0, n = v->cur; i < n; ++i)
        free(v->arr[i]);
    free(v->arr);
    v->arr = v->len = v->cur = 0;
}


int
p4_init(void)
{
    FILE *f;
    char *line = 0;
    size_t len = 0;
    int ret = 1;

    if ((f = popen("p4 info", "r")) == 0)
        return 1;

    while (getline(&line, &len, f) != -1) {
        if (len > 0) {
            ret = 0;
            goto done;
        }
    }

done:
    free(line);
    pclose(f);
    return ret;
}

void
p4_shutdown(void)
{
}

struct string_vector
p4_run(char const *cmd)
{
    FILE *f;
    char *line = 0;
    size_t len = 0;
    struct string_vector ret = {0};
    if ((f = popen(cmd, "r")) == 0)
        return ret;

    string_vector_init(&ret);
    while (getline(&line, &len, f) != -1)
        string_vector_push(&ret, line);

    free(line);
    pclose(f);
    return ret;
}

NS_END
