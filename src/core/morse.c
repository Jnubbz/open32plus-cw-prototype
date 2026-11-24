/* SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2026 Josh Noble (KN0BLE)
 * open32plus - Morse timing engine
 *
 * Timing:
 *   unit (dit) = 1.2 s / char_wpm            (PARIS standard, 50 units/word)
 *   standard:    char gap = 3 units, word gap = 7 units
 *   Farnsworth (ARRL, KE3Z): with c = char_wpm, s = eff_wpm,
 *     ta = (60c - 37.2s) / (s*c) seconds of total spacing per word
 *     char gap = 3*ta/19,  word gap = 7*ta/19
 */
#include "morse.h"

#include <stddef.h>

static const char *const k_alpha[26] = {
    ".-",   "-...", "-.-.", "-..",  ".",    "..-.", "--.",  "....", "..",
    ".---", "-.-",  ".-..", "--",   "-.",   "---",  ".--.", "--.-", ".-.",
    "...",  "-",    "..-",  "...-", ".--",  "-..-", "-.--", "--..",
};

static const char *const k_digit[10] = {
    "-----", ".----", "..---", "...--", "....-",
    ".....", "-....", "--...", "---..", "----.",
};

static const struct { char c; const char *p; } k_punct[] = {
    {'.', ".-.-.-"}, {',', "--..--"}, {'?', "..--.."}, {'\'', ".----."},
    {'/', "-..-."},  {'(', "-.--."},  {')', "-.--.-"}, {':', "---..."},
    {'=', "-...-"},  {'+', ".-.-."},  {'-', "-....-"}, {'"', ".-..-."},
    {'@', ".--.-."}, {'!', "-.-.--"},
};

const char *morse_pattern(char c)
{
    if (c >= 'a' && c <= 'z') c = (char)(c - 'a' + 'A');
    if (c >= 'A' && c <= 'Z') return k_alpha[c - 'A'];
    if (c >= '0' && c <= '9') return k_digit[c - '0'];
    for (size_t i = 0; i < sizeof k_punct / sizeof k_punct[0]; i++)
        if (k_punct[i].c == c) return k_punct[i].p;
    return NULL;
}

bool morse_init(morse_t *m, const morse_cfg_t *cfg)
{
    if (!m || !cfg || cfg->char_wpm == 0) return false;

    uint32_t c = cfg->char_wpm;
    m->unit_us = 1200000u / c;

    if (cfg->eff_wpm == 0 || cfg->eff_wpm >= c) {
        m->char_gap_us = 3u * m->unit_us;
        m->word_gap_us = 7u * m->unit_us;
    } else {
        uint32_t s = cfg->eff_wpm;
        /* ta_us = (60c - 37.2s) * 1e6 / (s c) = (600c - 372s) * 1e5 / (s c) */
        uint64_t ta_us = ((uint64_t)(600u * c - 372u * s) * 100000u) / (s * c);
        m->char_gap_us = (uint32_t)((3u * ta_us) / 19u);
        m->word_gap_us = (uint32_t)((7u * ta_us) / 19u);
    }
    morse_stop(m);
    return true;
}

void morse_load(morse_t *m, const char *text)
{
    morse_stop(m);
    m->text = text;
}

void morse_stop(morse_t *m)
{
    m->text = NULL;
    m->pat = NULL;
    m->in_prosign = false;
    m->pending_gap_us = 0;
}

/* Decide the gap after a completed character by looking ahead in the text. */
static uint32_t gap_after_char(morse_t *m)
{
    for (;;) {
        char c = *m->text;
        if (c == '\0') return 0;
        if (c == '>') { m->in_prosign = false; m->text++; continue; }
        if (m->in_prosign) return m->unit_us;      /* run-together letters */
        if (c == ' ') {
            while (*m->text == ' ') m->text++;
            return *m->text ? m->word_gap_us : 0;
        }
        return m->char_gap_us;
    }
}

bool morse_next(morse_t *m, morse_elem_t *out)
{
    if (!m || !out || !m->text) return false;

    for (;;) {
        if (m->pending_gap_us) {
            out->key_down = false;
            out->dur_us = m->pending_gap_us;
            m->pending_gap_us = 0;
            return true;
        }

        if (m->pat && *m->pat) {
            char el = *m->pat++;
            out->key_down = true;
            out->dur_us = (el == '-') ? 3u * m->unit_us : m->unit_us;
            if (*m->pat) {
                m->pending_gap_us = m->unit_us;    /* intra-character gap */
            } else {
                m->pat = NULL;
                m->pending_gap_us = gap_after_char(m);
            }
            return true;
        }

        /* Fetch the next sendable character. */
        char c = *m->text;
        if (c == '\0') { m->text = NULL; return false; }
        m->text++;
        if (c == '<') { m->in_prosign = true; continue; }
        if (c == '>') { m->in_prosign = false; continue; }
        if (c == ' ') continue;                    /* leading spaces */
        m->pat = morse_pattern(c);                 /* NULL => skipped */
    }
}
