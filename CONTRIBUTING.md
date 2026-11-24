# Contributing to open32plus

Thanks for helping. Because the goal is firmware that is *genuinely* open, how code gets
written matters as much as what it does.

## Clean-room rules (mandatory)

open32plus must contain only code we have the right to license under the MIT license.
Contributions are accepted under MIT. Every source file needs SPDX headers:

```c
/* SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2026 Your Name (CALLSIGN) */
```

Third-party code is allowed only under a permissive license (MIT, BSD, ISC, Apache-2.0)
and must keep its own headers. **No GPL, non-commercial, or vendor-SDK code**, so the
project stays compatible with other permissive open firmware projects.

**Allowed sources of knowledge**
- Chip datasheets and application notes
- Your own measurements of your own radio (logic analyzer, SDR, debugger, photos)
- Your own analysis of protocols observed on the wire (e.g. the programming cable)
- Public, non-code documentation (codeplug maps, forum posts describing behaviour)

**Not allowed**
- Copying, translating or "paraphrasing line by line" code from the stock Baofeng firmware,
  OpenGD77, OpenDMR, OpenDM32 or any other source-available/proprietary project.
- Pasting decompiled stock firmware into the repo.
- Writing code for a subsystem while you have another project's source for that same
  subsystem open. If you have studied such code, say so in your PR and let someone else
  implement from the written spec.

**Workflow**
1. Facts about the hardware go into `docs/hardware/` *in your own words*.
2. Every fact gets an entry in [docs/hardware/PROVENANCE.md](docs/hardware/PROVENANCE.md)
   saying where it came from.
3. Code is written from those notes.

## Developer Certificate of Origin

Every commit must be signed off, certifying the
[DCO](https://developercertificate.org/):

```sh
git commit -s -m "core: add keyer state machine"
```

## Code style

- C11, no dynamic allocation in `src/core` (firmware-friendly).
- `src/core` must never include platform headers – only `src/hal/`.
- Every core module gets unit tests in `tests/`. `make test` must pass.
- Compile cleanly with `-Wall -Wextra -Werror`.

## Hardware testing

Never transmit untested code on the air. See [docs/SAFETY.md](docs/SAFETY.md).
