# open32plus

**Truly open-source, CW-first replacement firmware for the Baofeng DM-32UV.**

> ⚠️ **Status: pre-alpha / hardware discovery.** Nothing here runs on a radio yet.
> Do not flash anything from this repo onto a radio. Read [docs/SAFETY.md](docs/SAFETY.md) first.

## Why another firmware?

Existing alternative firmwares for the DM-32 family are *source-available* but restrict
commercial use, and some depend on pieces of the stock firmware. open32plus is:

1. **Actually open** – GPLv3, clean-room, 100% our own code, reproducible builds.
2. **Simulator-first** – the UI and logic build and run on a PC, so features can be
   developed and tested without flashing a radio.
3. **Reliable** – unit tests + CI on every commit, and an on-air test checklist for every
   feature before release.
4. **Easy to use and update** – a coherent menu design, and (planned) flashing and
   programming straight from a browser via WebSerial.
5. **CW-first** – keyboard-to-CW from the message editor, macros, MCW, true carrier CW,
   CW receive/decode, Morse menu prompts and a built-in trainer.

## What works today

| Component | State |
|---|---|
| `src/core/morse` – non-blocking Morse timing engine (WPM, Farnsworth, prosigns) | ✅ tested |
| `src/core/macro` – CW macro expansion (`{CALL}`, `{THEIR}`, `{RST}`, …) | ✅ tested |
| `platform/sim/cw2wav` – renders text to a WAV with shaped (click-free) keying | ✅ |
| Radio hardware support | ❌ not started – see [docs/hardware](docs/hardware/README.md) |

## Quick start (PC)

```sh
make test                       # build and run unit tests
make sim                        # build the simulator tools
./build/cw2wav -w 20 -o cq.wav "CQ CQ DE KN0BLE KN0BLE <AR> K"
./build/cw2wav -w 20 -f 10 -o slow.wav "PARIS PARIS"   # 20 wpm chars, 10 wpm Farnsworth
```

## Layout

```
src/core/       portable, hardware-independent code (Morse, macros, later UI/state)
src/hal/        hardware abstraction interfaces the core depends on
platform/sim/   PC implementations + tools
platform/dm32/  DM-32UV port (empty until hardware discovery is done)
tests/          unit tests (run on PC and in CI)
docs/           roadmap, safety, hardware notes, provenance log
```

## Roadmap

See [docs/ROADMAP.md](docs/ROADMAP.md). Short version: hardware discovery → bring-up →
analog FM → CW v1 → CW v2 → web tooling → DMR (only after vocoder/patent review).

## Contributing

Please read [CONTRIBUTING.md](CONTRIBUTING.md) – especially the **clean-room rules**.
All commits must be signed off (`git commit -s`).

## License

GPL-3.0-only. See [LICENSE](LICENSE).

73 de KN0BLE
