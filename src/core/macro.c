/* SPDX-License-Identifier: GPL-3.0-only */
#include "macro.h"

#include <stddef.h>
#include <string.h>

static const char *lookup(const char *name, size_t len, const macro_vars_t *v)
{
    static const struct { const char *key; size_t off; } map[] = {
        {"CALL",  offsetof(macro_vars_t, call)},
        {"THEIR", offsetof(macro_vars_t, their)},
        {"RST",   offsetof(macro_vars_t, rst)},
        {"NAME",  offsetof(macro_vars_t, name)},
        {"GRID",  offsetof(macro_vars_t, grid)},
    };
    if (!v) return NULL;
    for (size_t i = 0; i < sizeof map / sizeof map[0]; i++) {
        if (strlen(map[i].key) == len && memcmp(map[i].key, name, len) == 0) {
            const char *val = *(const char *const *)((const char *)v + map[i].off);
            return (val && *val) ? val : NULL;
        }
    }
    return NULL;
}

int macro_expand(const char *tmpl, const macro_vars_t *v, char *out, size_t n)
{
    size_t o = 0;
    int overflow = 0;

    if (!out || n == 0) return -1;

#define PUT(ch) do { if (o + 1 < n) out[o++] = (ch); else overflow = 1; } while (0)

    for (const char *p = tmpl; p && *p; p++) {
        if (*p == '{') {
            const char *end = strchr(p + 1, '}');
            if (end) {
                const char *val = lookup(p + 1, (size_t)(end - p - 1), v);
                if (val) {
                    for (const char *q = val; *q; q++) PUT(*q);
                    p = end;
                    continue;
                }
            }
        }
        PUT(*p);
    }
#undef PUT

    out[o] = '\0';
    return overflow ? -1 : (int)o;
}
