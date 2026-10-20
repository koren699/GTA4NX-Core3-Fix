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

## Overclock used in testing

| Hardware clock | Tested frequency |
| --- | ---: |
| CPU | **1963 MHz** |
| GPU | **768 MHz** |
| RAM | **2131 MHz** |

These figures were observed in **sys-clk-OC, Handheld profile**, with the matching Title ID. **They are tested settings, not mandatory minimum clocks, and do not guarantee 30 FPS.** Higher frequencies may help, but only if available on your specific Switch and stable with safe temperatures, charging and cooling. Do not disable thermal safeguards or exceed safe limits. Some consoles may require lower clocks. An OLED console initially showed a 768 MHz GPU cap caused by a configured thermal/overclock limit; that was a profile limit, not evidence of a weaker GPU.

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
