# GTA4NX: engineering path toward a measured 30 FPS

30 FPS needs every frame to complete within 33.33 ms. The upstream port README reports ~15–20 FPS in the city (around 17 average) and ~11 FPS in worst spots on an Erista Switch v1. Core 3 improvement is community-observed, but new percentile metrics were not collected; constant 30 FPS remains a **goal, not a claim**.

## Controlled benchmarking

Use identical save, city location, time of day, console mode, game assets, clocks, and temperature conditions. Warm up shaders for ~2 minutes. Measure three 60-second runs driving, walking, and an interior. Log median frame time, 95th/99th-percentile frame time, freezes, audio glitches, SoC temperature and clocks. Disable detailed profiling for performance runs; enable it only during a short focused diagnosis.

## Priority experiments

1. **CPU scheduling:** compare tested Core 3 NPDM baseline with `switch_main_thread_priority=0` against experimental `46`. The source patch documents render/main-thread scheduling bottlenecks; however, no real-console A/B results exist for this change yet.
2. **Render-thread and audio competition:** measure blocked versus running time for guest main, producer, recorder, XAudio2 and decoder threads. Confirm Core 3 use rather than assuming it from the presence of the NPDM file.
3. **GPU stalls and resolves:** inspect native GPU pass profiling in GPU-heavy areas, notably the hospital mentioned in upstream comments. Identify fence wait durations and specific expensive passes instead of blindly reducing resolution.
4. **Readbacks and buffer captures:** the port already optimizes `gta4_native_texture_lock_reuse` and dirty buffer comparisons. Profile remaining synchronization; any texture-readback changes must preserve phone/UI/text accuracy.
5. **Visual quality:** preserve Russian mod compatibility (`gta4_native_vector_fonts=false`) while testing. Compare FSR filters only after CPU bottlenecks are addressed.

Change one thing per test. Keep a backup of the known-good configuration. Do not label perceived smoothness as locked 30 FPS.

Upstream source: https://github.com/kabba536/Grand-Theft-Auto-IV-Native-Port-Nintendo-Switch/tree/main/source
