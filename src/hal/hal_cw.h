/* SPDX-License-Identifier: GPL-3.0-only
 * open32plus - hardware abstraction for CW keying
 *
 * Implemented by platform/sim (PC) and, later, platform/dm32 (radio).
 * Core code only ever talks to hardware through interfaces like this one.
 */
#ifndef O32_HAL_CW_H
#define O32_HAL_CW_H

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    HAL_CW_SIDETONE_ONLY,  /* practice / trainer: no RF */
    HAL_CW_MCW,            /* tone modulated onto FM carrier */
    HAL_CW_CARRIER,        /* true CW: shaped carrier keying (needs Phase 4 HW work) */
} hal_cw_mode_t;

/* Select mode and sidetone/MCW tone frequency. */
void hal_cw_setup(hal_cw_mode_t mode, uint16_t tone_hz);

/* Key down / key up. Implementations must apply rise/fall shaping
 * (~5 ms raised cosine) to avoid key clicks. */
void hal_cw_key(bool down);

#endif
