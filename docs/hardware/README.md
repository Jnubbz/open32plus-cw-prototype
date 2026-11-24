# DM-32UV hardware notes

Everything here must be written in our own words and logged in
[PROVENANCE.md](PROVENANCE.md). Mark anything unconfirmed on *our own* board as **TBD** or
**unverified**.

## Board revisions

| Silkscreen | Notes | Confirmed by us? |
|---|---|---|
| `DM32_UV_V1.2 2024-08-05` | reported by community archive | no |
| `DM32_UV_V1.4 2024-12-20` | reported by community archive; later boards may differ | no |

## Major components

| Function | Part | Bus / pins | Confirmed by us? |
|---|---|---|---|
| Main MCU | TBD (clue: a C-SKY / Xuantie core is suggested by another project's build toolchain) | – | no |
| RF transceiver | TBD | TBD | no |
| DMR baseband | TBD | TBD | no |
| SPI flash (small) | 25Q16 family (reported) | SPI | no |
| SPI flash (large) | 25Q128 family (reported) | SPI | no |
| Display | TBD | TBD | no |
| Audio amp | TBD | – | no |
| PA / TX enable | TBD | TBD | no |

## Debug / programming

| Item | Finding |
|---|---|
| Debug port (JTAG/SWD/C-SKY CDS) | TBD |
| Bootloader entry (button combo) | TBD |
| Firmware update protocol over cable | TBD – capture with logic analyzer / USB sniffer |

## Codeplug

A public map of the stock codeplug for CPS V1.1.46 exists at
<https://dmr-tools.github.io/codeplugs/dm32uv_V1.1.46.html>. Our firmware will define its
own format, but reading/importing stock codeplugs is a goal.

## Phase 0 checklist
- [ ] Photograph both sides of the main board (high-res, every chip marking legible)
- [ ] Identify each chip and link its datasheet
- [ ] Dump 25Q16 and 25Q128 with an external programmer (clip, radio unpowered); hash and store
- [ ] Find and document the debug port
- [ ] Capture a stock firmware update over the cable
- [ ] Measure: which MCU pins key TX, control PA power, select bands
