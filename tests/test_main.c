/* SPDX-License-Identifier: GPL-3.0-only
 * Minimal test runner - no external dependencies.
 */
#include <stdio.h>
#include <string.h>

#include "../src/core/macro.h"
#include "../src/core/morse.h"

static int g_fail, g_pass;

#define CHECK(cond) do { \
    if (cond) g_pass++; \
    else { g_fail++; printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); } \
} while (0)

#define NEAR(a, b, tol) ((a) > (b) ? (a) - (b) <= (tol) : (b) - (a) <= (tol))

/* Total duration of a message in microseconds. */
static unsigned long long total_us(const morse_cfg_t *cfg, const char *text)
{
    morse_t m;
    morse_elem_t e;
    unsigned long long t = 0;
    morse_init(&m, cfg);
    morse_load(&m, text);
    while (morse_next(&m, &e)) t += e.dur_us;
    return t;
}

/* Render a message as a string: '=' per unit of mark, '_' per unit of space. */
static void render(const char *text, char *buf, size_t n)
{
    morse_cfg_t cfg = {20, 0};
    morse_t m;
    morse_elem_t e;
    size_t o = 0;
    morse_init(&m, &cfg);
    morse_load(&m, text);
    while (morse_next(&m, &e)) {
        unsigned units = e.dur_us / m.unit_us;
        for (unsigned i = 0; i < units && o + 1 < n; i++)
            buf[o++] = e.key_down ? '=' : '_';
    }
    buf[o] = '\0';
}

static void test_paris_standard(void)
{
    /* "PARIS " = 50 units = 3000 ms at 20 wpm. Engine omits trailing gap,
     * so add one word gap (7 units). */
    morse_cfg_t cfg = {20, 0};
    morse_t m;
    morse_init(&m, &cfg);
    CHECK(m.unit_us == 60000);
    CHECK(total_us(&cfg, "PARIS") + m.word_gap_us == 3000000ULL);
}

static void test_paris_farnsworth(void)
{
    /* 20 wpm characters, 10 wpm overall: "PARIS " takes 6000 ms. */
    morse_cfg_t cfg = {20, 10};
    morse_t m;
    morse_init(&m, &cfg);
    unsigned long long t = total_us(&cfg, "PARIS") + m.word_gap_us;
    CHECK(NEAR(t, 6000000ULL, 1000ULL));
    /* character spacing:word spacing stays 3:7 */
    CHECK(NEAR(m.char_gap_us * 7ULL, m.word_gap_us * 3ULL, 10ULL));
    /* element timing unchanged */
    CHECK(m.unit_us == 60000);
}

static void test_patterns(void)
{
    char b[256];
    render("E", b, sizeof b);   CHECK(strcmp(b, "=") == 0);
    render("A", b, sizeof b);   CHECK(strcmp(b, "=_===") == 0);
    render("EE", b, sizeof b);  CHECK(strcmp(b, "=___=") == 0);
    render("E E", b, sizeof b); CHECK(strcmp(b, "=_______=") == 0);
    render("e   e", b, sizeof b); CHECK(strcmp(b, "=_______=") == 0); /* spaces collapse, case-insensitive */
    render("E#E", b, sizeof b); CHECK(strcmp(b, "=___=") == 0);       /* unknown skipped */
}

static void test_prosign(void)
{
    char a[256], b[256];
    render("<AR>", a, sizeof a);
    render("+", b, sizeof b);          /* AR == .-.-. == '+' */
    CHECK(strcmp(a, b) == 0);
    render("<SK>", a, sizeof a);
    CHECK(strcmp(a, "=_=_=_===_=_===") == 0); /* ...-.- */
    render("K <AR> K", a, sizeof a);
    render("K + K", b, sizeof b);
    CHECK(strcmp(a, b) == 0);
}

static void test_invalid_and_stop(void)
{
    morse_t m;
    morse_elem_t e;
    morse_cfg_t bad = {0, 0}, ok = {25, 0};
    CHECK(!morse_init(&m, &bad));
    CHECK(morse_init(&m, &ok));
    CHECK(!morse_next(&m, &e));        /* nothing loaded */
    morse_load(&m, "TEST");
    CHECK(morse_next(&m, &e) && e.key_down);
    morse_stop(&m);
    CHECK(!morse_next(&m, &e));
    morse_load(&m, "");
    CHECK(!morse_next(&m, &e));
}

static void test_macros(void)
{
    char out[64];
    macro_vars_t v = { .call = "KN0BLE", .their = "W1AW", .rst = "599" };
    CHECK(macro_expand("CQ DE {CALL} {CALL} K", &v, out, sizeof out) > 0);
    CHECK(strcmp(out, "CQ DE KN0BLE KN0BLE K") == 0);
    macro_expand("{THEIR} DE {CALL} UR {RST} <BK>", &v, out, sizeof out);
    CHECK(strcmp(out, "W1AW DE KN0BLE UR 599 <BK>") == 0);
    macro_expand("NAME {NAME} {BOGUS} {", &v, out, sizeof out);   /* unset/unknown kept */
    CHECK(strcmp(out, "NAME {NAME} {BOGUS} {") == 0);
    char tiny[8];
    CHECK(macro_expand("{CALL} {CALL}", &v, tiny, sizeof tiny) == -1);
    CHECK(strcmp(tiny, "KN0BLE ") == 0);                         /* truncated, terminated */
}

int main(void)
{
    test_paris_standard();
    test_paris_farnsworth();
    test_patterns();
    test_prosign();
    test_invalid_and_stop();
    test_macros();
    printf("%d passed, %d failed\n", g_pass, g_fail);
    return g_fail ? 1 : 0;
}
