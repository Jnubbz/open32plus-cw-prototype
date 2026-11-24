# Safety & legal

## Before touching a radio
1. **Use a radio you own and that is not used on a licensed commercial system.**
   Radios on Part 90 (business/public-safety) systems must keep their certified firmware.
2. **Back up both SPI flash chips** (the DM-32 has a 25Q16 and a 25Q128) and keep the
   backups off the radio. Calibration data lives there; lose it and the radio is junk.
3. Record your board revision (silkscreen, e.g. `DM32_UV_V1.4`). Boards differ.

## Transmit testing
1. Dummy load first, always.
2. Watch the output on an SDR / spectrum analyzer: check frequency accuracy, deviation,
   spurs and, for CW, key clicks (keying envelope rise/fall time).
3. Only then on the air, within your license privileges, with proper station ID.

## Amateur use only
This firmware is for amateur radio experimentation. You are responsible for complying
with the rules where you operate. No encryption features will be accepted.

## Disclaimer
Provided WITHOUT WARRANTY (see LICENSE). Flashing custom firmware can brick a radio.
