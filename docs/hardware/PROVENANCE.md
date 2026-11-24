# Provenance log

Every hardware fact used by the code is logged here. If you can't fill in "Source", the fact
doesn't go in.

| Date | Fact | Source (link / "own measurement") | Who |
|---|---|---|---|
| 2026-11-23 | Board revisions V1.2 (2024-08-05) and V1.4 (2024-12-20) exist | README of github.com/M7OCM/DM-32UV (docs, not code) | KN0BLE |
| 2026-11-23 | DM-32 has 25Q16 and 25Q128 SPI flash to back up | Published DM-32 OpenGD77 install guide (docs, not code) | KN0BLE |
| 2026-11-23 | Another project builds DM-32 firmware with the Xuantie/C-SKY `csky-elfabiv2` GCC toolchain → MCU is likely C-SKY | README of github.com/va3bpf/OpenDMR (docs, not code) – **unverified on our board** | KN0BLE |
| 2026-11-23 | Stock codeplug memory map | dmr-tools.github.io/codeplugs/dm32uv_V1.1.46.html | KN0BLE |
| 2026-11-23 | Morse timing: dit = 1.2/WPM s (PARIS = 50 units); Farnsworth per ARRL (KE3Z): ta=(60c−37.2s)/(sc), tc=3ta/19, tw=7ta/19 | ARRL "A Standard for Morse Timing Using the Farnsworth Technique"; morsecode.world | KN0BLE |
