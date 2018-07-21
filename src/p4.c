#include "p4fs.h"
#include <stdarg.h>

NS_BEGIN

int
p4_init(void)
{
    FILE *f;
    int ret = 1;

    if ((f = popen("p4 info", "r")) == 0)
        return 1;

    char *line = 0;
    size_t len = 0;
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
p4_run(char const *cmd, ...)
{
    char buf[1024];
    FILE *f;
    struct string_vector ret = {0};
    va_list args;
    va_start(args, cmd);
    vsnprintf(buf, 1024, cmd, args);
    buf[1023] = 0;
    va_end(args);

    if ((f = popen(buf, "r")) == 0)
        return ret;

    char *line = 0;
    size_t len = 0;
    string_vector_init(&ret);
    while (getline(&line, &len, f) != -1) {
        int n = strlen(line);
        if (line[n - 1] == '\n') n -= 1;
        string_vector_push(&ret, line, n);
    }

    free(line);
    pclose(f);
    return ret;
}

NS_END
