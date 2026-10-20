# GTA4NX Core 3 Fix — Nintendo Switch

**Community-tested audio and performance improvement** for the [GTA IV native Switch port](https://github.com/kabba536/Grand-Theft-Auto-IV-Native-Port-Nintendo-Switch).

**[Українська інструкція / Ukrainian instructions](README_UA.md)**

> Independent community patch, not an official release by the original port developer. Requires a working GTA4NX HOME Menu forwarder with Title ID `03DB149512496000`. No copyrighted GTA IV game data, translation content, or game NRO is included.

## Download

**[Download Atmosphère-ready ZIP](GTA4NX_Core3_Atmosphere_Fix.zip)** (use GitHub's Download raw file option).

### Install

1. Fully close GTA IV. **Back up** your existing `atmosphere/contents/03DB149512496000` directory if present.
2. Extract the ZIP into the **root of the microSD card**; the file should land at `sdmc:/atmosphere/contents/03DB149512496000/exefs/main.npdm`.
3. Reboot Atmosphère. Start the same GTA4NX HOME Menu forwarder. A different Title ID is not supported.
4. Test audio stability and frame rate in busy city scenes.

**Rollback:** remove only `sdmc:/atmosphere/contents/03DB149512496000/exefs/main.npdm` and reboot. This patch may have no effect if your launch method does not load ExeFS overrides.

## What this changes

The original forwarder NPDM grants the process cores 0–2. In its two kernel capability tables (ACI0 and ACID) this patch changes only the ThreadInfo `MaxCoreNumber` from **2 to 3**, so the port can place its XAudio2 game-audio thread on CPU Core 3 when supported. It does **not** overclock the Switch or modify the game executable.

- Tested SHA-256 (`main.npdm`): `79e356394589662cae4a8b8cf593d3a0c6f73467ab8c91d2dca74e0b8c2d8ca0`
- [Standalone NPDM](atmosphere/contents/03DB149512496000/exefs/main.npdm) and [NPDM verification notes](docs/TESTED_NPDM_DETAILS.txt)
- Community tester reported that audio stuttering stopped and the game felt smoother. **Constant 30 FPS has not been demonstrated.** Your results may differ.

## Overclock used for current community testing

| Hardware clock | Frequency |
| --- | ---: |
| CPU | **1963 MHz** (earlier sys-clk-OC screenshot) |
| GPU | **921 MHz** (current tester-reported setting) |
| RAM | **2131 MHz** (earlier sys-clk-OC screenshot) |

**GPU 921 MHz is the tester's current overclock setting.** The older screenshot showed **GPU 768 MHz** because a configured temperature/overclock limit capped it; the tester has since corrected that setting on the OLED. CPU 1963 MHz and RAM 2131 MHz are from the earlier screenshot, so the table should not be interpreted as a single fully re-measured benchmark session. These frequencies are **reference settings, not mandatory requirements**, and do not guarantee 30 FPS. Higher GPU clocks are only appropriate where the individual console supports them and remains stable with adequate cooling, charging and safe temperatures. Do not disable thermal safeguards. Lower clocks may be necessary on other devices.

## Russian Text Mod compatibility

Users with a separately installed Russian translation should preserve the following setting in `sdmc:/switch/gta4nx/LibertyRecomp/native.toml`:

```toml
gta4_native_vector_fonts = false
```

Without it, the text may turn into unreadable glyphs after replacing the config. The translation uses the Italian language slot. **No translation files are included.**

- [RU text-compatible baseline](config/native_RU_font_fix.toml)
- [RU baseline without detailed profiling](config/native_RU_perf_baseline_NO_PROFILER.toml) (benchmarking)
- [Experimental main-thread priority 46](experimental/CPU_MAIN_PRIORITY_46_RU.toml) (**not console-validated**)
- [Build the patch from your own forwarder NPDM](tools/patch_npdm.py)
- [Performance development roadmap](docs/PERFORMANCE_NEXT_STEPS.md)

## Credit

Original Switch port: [kabba536](https://github.com/kabba536/Grand-Theft-Auto-IV-Native-Port-Nintendo-Switch). Based on [LibertyRecompAndroid](https://github.com/vaduur/LibertyRecompAndroid) and [Liberty Recompiled](https://github.com/OZORDI/LibertyRecomp). This repo distributes only a compatibility fix, configurations and an open patching utility.
