/* SPDX-License-Identifier: GPL-3.0-only
 * open32plus - Morse timing engine
 *
 * Turns text into a stream of key-down / key-up elements with durations.
 * Non-blocking and allocation-free: the caller pulls one element at a time
 * (e.g. from a timer interrupt or the simulator) and keys TX / sidetone.
 *
 * Text syntax:
 *   - Letters (any case), digits, ITU punctuation . , ? ' / ( ) : = + - " @ and !
 *   - One or more spaces = one word space
 *   - <AR> <SK> <BT> <KN> ... : letters inside <> are sent run-together (prosign)
 *   - Unknown characters are skipped
 */
#ifndef O32_MORSE_H
#define O32_MORSE_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint16_t char_wpm;  /* character speed (element timing) */
    uint16_t eff_wpm;   /* effective/Farnsworth speed; 0 or >= char_wpm = standard */
} morse_cfg_t;

typedef struct {
    bool     key_down;  /* true = mark (carrier/tone on), false = space */
    uint32_t dur_us;    /* duration in microseconds */
} morse_elem_t;

typedef struct {
    uint32_t    unit_us;      /* one dit */
    uint32_t    char_gap_us;  /* gap between characters */
    uint32_t    word_gap_us;  /* gap between words */
    const char *text;         /* current position in text */
    const char *pat;          /* current pattern ('.'/'-'), NULL if between chars */
    bool        in_prosign;
    uint32_t    pending_gap_us;
} morse_t;

/* Returns false if cfg is invalid (char_wpm == 0). */
bool morse_init(morse_t *m, const morse_cfg_t *cfg);

/* Start sending a NUL-terminated string. The string must outlive sending. */
void morse_load(morse_t *m, const char *text);

/* Get the next element. Returns false when the message is finished. */
bool morse_next(morse_t *m, morse_elem_t *out);

/* Abort the current message. */
void morse_stop(morse_t *m);

/* Pattern for one character ('.'/'-' string) or NULL if unsupported. */
const char *morse_pattern(char c);

#endif
