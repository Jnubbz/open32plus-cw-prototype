# Roadmap

Analog-first: no patent questions, and CW gets on the air soonest.

| Phase | Goal | Done when |
|---|---|---|
| 0 | **Hardware discovery** | Board photos, every chip identified, both SPI flash chips backed up, debug port located, stock update/bootloader protocol documented |
| 1 | **Bring-up** | Our image boots: display text, keypad input, backlight, battery voltage |
| 2 | **Analog radio** | FM RX/TX, channels, VFO, scan, CTCSS/DCS |
| 3 | **CW v1** | Keyboard-to-CW from message editor, macros, MCW over FM, CW ID & beacon, sidetone, speed/Farnsworth settings |
| 4 | **CW v2** | True carrier CW (shaped, click-free), CW/SSB receive, decoder into inbox, Morse menu prompts, Koch trainer, paddle input |
| 5 | **Web tooling** | WebSerial flasher, browser codeplug editor, ZIP/county channel generator |
| 6 | **DMR** | Only after a written review of the vocoder/AMBE patent situation |

## Cross-cutting, every phase
- PC simulator keeps parity with the radio UI.
- Unit tests + CI.
- On-air test checklist per feature (dummy load + SDR first).
