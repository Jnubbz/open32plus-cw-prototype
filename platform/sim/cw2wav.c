/* SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2026 Josh Noble (KN0BLE)
 * cw2wav - render text to a mono 16-bit WAV using the open32plus Morse engine.
 *
 * Keying uses a raised-cosine envelope (default 5 ms) - the same shaping the
 * radio must apply to avoid key clicks.
 *
 *   cw2wav [-w wpm] [-f farnsworth_wpm] [-t tone_hz] [-r ramp_ms]
 *          [-c call] [-v] -o out.wav "TEXT with {CALL} macros"
 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../src/core/macro.h"
#include "../../src/core/morse.h"

#define RATE 8000

static void put_u32(FILE *f, unsigned v) { unsigned char b[4] = {v, v >> 8, v >> 16, v >> 24}; fwrite(b, 1, 4, f); }
static void put_u16(FILE *f, unsigned v) { unsigned char b[2] = {v, v >> 8}; fwrite(b, 1, 2, f); }

static void usage(void)
{
    fprintf(stderr, "usage: cw2wav [-w wpm] [-f farns_wpm] [-t tone_hz] [-r ramp_ms] "
                    "[-c call] [-v] -o out.wav \"TEXT\"\n");
    exit(2);
}

int main(int argc, char **argv)
{
    int wpm = 20, farns = 0, tone = 600, verbose = 0;
    double ramp_ms = 5.0;
    const char *outpath = NULL, *call = "N0CALL", *text = NULL;

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-w") && i + 1 < argc) wpm = atoi(argv[++i]);
        else if (!strcmp(argv[i], "-f") && i + 1 < argc) farns = atoi(argv[++i]);
        else if (!strcmp(argv[i], "-t") && i + 1 < argc) tone = atoi(argv[++i]);
        else if (!strcmp(argv[i], "-r") && i + 1 < argc) ramp_ms = atof(argv[++i]);
        else if (!strcmp(argv[i], "-c") && i + 1 < argc) call = argv[++i];
        else if (!strcmp(argv[i], "-o") && i + 1 < argc) outpath = argv[++i];
        else if (!strcmp(argv[i], "-v")) verbose = 1;
        else if (argv[i][0] != '-' && !text) text = argv[i];
        else usage();
    }
    if (!outpath || !text || wpm <= 0 || tone <= 0 || tone >= RATE / 2) usage();

    char msg[512];
    macro_vars_t vars = { .call = call };
    if (macro_expand(text, &vars, msg, sizeof msg) < 0) {
        fprintf(stderr, "message too long\n");
        return 1;
    }

    morse_cfg_t cfg = { (unsigned short)wpm, (unsigned short)farns };
    morse_t m;
    if (!morse_init(&m, &cfg)) usage();

    FILE *f = fopen(outpath, "wb");
    if (!f) { perror(outpath); return 1; }

    /* WAV header with placeholder sizes, patched at the end. */
    fwrite("RIFF", 1, 4, f); put_u32(f, 0); fwrite("WAVEfmt ", 1, 8, f);
    put_u32(f, 16); put_u16(f, 1); put_u16(f, 1); put_u32(f, RATE);
    put_u32(f, RATE * 2); put_u16(f, 2); put_u16(f, 16);
    fwrite("data", 1, 4, f); put_u32(f, 0);

    const double ramp_s = ramp_ms / 1000.0;
    const double step = 2.0 * M_PI * tone / RATE;
    double phase = 0.0;
    unsigned long samples = 0;
    morse_elem_t e;

    if (verbose) printf("sending: %s\nunit %u us, char gap %u us, word gap %u us\n",
                        msg, m.unit_us, m.char_gap_us, m.word_gap_us);

    morse_load(&m, msg);
    while (morse_next(&m, &e)) {
        unsigned long n = (unsigned long)((double)e.dur_us * RATE / 1e6 + 0.5);
        for (unsigned long i = 0; i < n; i++) {
            double env = 0.0;
            if (e.key_down) {
                double t = (double)i / RATE, rem = (double)(n - i) / RATE;
                env = 1.0;
                if (t < ramp_s)   env = 0.5 - 0.5 * cos(M_PI * t / ramp_s);
                if (rem < ramp_s) env *= 0.5 - 0.5 * cos(M_PI * rem / ramp_s);
            }
            short s = (short)(env * 0.6 * 32767.0 * sin(phase));
            phase += step;
            if (phase > 2.0 * M_PI) phase -= 2.0 * M_PI;
            put_u16(f, (unsigned short)s);
            samples++;
        }
    }
    /* trailing silence so players don't clip the last element */
    for (int i = 0; i < RATE / 4; i++, samples++) put_u16(f, 0);

    unsigned data_bytes = (unsigned)(samples * 2);
    fseek(f, 4, SEEK_SET);  put_u32(f, 36 + data_bytes);
    fseek(f, 40, SEEK_SET); put_u32(f, data_bytes);
    fclose(f);

    printf("%s: %.2f s at %d wpm%s\n", outpath, (double)samples / RATE, wpm,
           farns && farns < wpm ? " (Farnsworth)" : "");
    return 0;
}
