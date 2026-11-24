/* SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2026 Josh Noble (KN0BLE)
 * open32plus - CW macro expansion
 *
 * Expands templates like "CQ CQ DE {CALL} {CALL} K" for the preset message bank.
 * Unknown or empty fields are left as-is so the operator notices them.
 */
#ifndef O32_MACRO_H
#define O32_MACRO_H

#include <stddef.h>

typedef struct {
    const char *call;   /* {CALL}  own callsign */
    const char *their;  /* {THEIR} other station (e.g. last heard) */
    const char *rst;    /* {RST}   report to send */
    const char *name;   /* {NAME}  operator name */
    const char *grid;   /* {GRID}  Maidenhead locator */
} macro_vars_t;

/* Expand tmpl into out (size n, always NUL-terminated when n > 0).
 * Returns the output length, or -1 if it did not fit (output truncated). */
int macro_expand(const char *tmpl, const macro_vars_t *v, char *out, size_t n);

#endif
