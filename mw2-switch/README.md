# MW2 Xbox 360 -> Nintendo Switch (research / bootstrap)

**Current milestone:** A native ARM64 Horizon/libnx NRO diagnostic and a GitHub Actions build.
**Not a Modern Warfare 2 port yet.** This does not execute MW2 guest PPC code or render the game.

## Source projects
- https://github.com/PaulCombal/mw2-recompiled (XenonRecomp, Xbox 360 guest + Vulkan host runtime)
- https://github.com/vrcnx/mw2-recompiled-ios (ARM64 port research, review licensing and patches)
- https://github.com/jhuz0/nvk-switch (experimental NVK for Tegra X1)
- https://github.com/switchbrew/switch-examples (libnx application build template)
- https://github.com/bluecyborg-dev/cod_bo1-nx (game-specific Xbox 360 to Switch port research)

## Build
Run \`make -C mw2-switch\` in an environment with \`DEVKITPRO\` / devkitA64 / libnx installed.
GitHub Actions uses \`devkitpro/devkita64:20260219\` and uploads \`mw2-switch-preflight.nro\`.
It may be launched via the Homebrew Menu, preferably in title-override mode.
**A successful NRO build does not mean the game runs.**

## Actual next source changes needed (in order)
1. **Cross toolchain / generated PPC:** preserve XenonRecomp's PPC-to-C++ output, remove x86-only \`-march\` assumptions, select AArch64/SIMDe backend. Compile guest-generated object code with devkitA64. Upstream's generated \`ppc/\` is not checked in.
2. **Horizon kernel:** adapt \`runtime/platform.cpp\`, thread creation, page faults, networking, file paths, and error handling. POSIX/Linux and Windows code cannot simply be linked against libnx.
3. **Guest VA mapping:** upstream \`runtime/guest_memory.cpp\` reserves a 4 GiB virtual address window and maps the same 512 MiB physical bank at two guest addresses (0xA0000000 and 0xC0000000). Design and test a suitable 39-bit-address-space Horizon mapping before running guest code. This is a blocker, not a resolved feature.
4. **GPU:** upstream's SDL3/Vulkan presenter and Xenos shader translator depend on a working Vulkan 1.2-compatible environment. Investigate whether NVK supports the required features on GM20B; replace the SDL window/surface integration and verify real shader pipelines on hardware.
5. **Audio:** adapt XMA decoding/audio delivery using libnx or available audio libraries.
6. **Controller + lifecycle:** map Joy-Con/Pro Controller to XInput semantics, suspend/resume, input focus, saves.
7. **Performance:** profile on original Switch, control thread scheduling and render scale; 30 fps is a target, **not a promise**.

## Game files (not in this repository)
The upstream build requires a matching retail Xbox 360 copy, especially \`default.xex\`, later \`*.ff\`, \`*.pak\`, \`*.bik\`. The preflight NRO needs none. No copyrighted game files, generated copyrighted code, or ISO should be committed or uploaded to public CI.

## Milestone definitions
- M0: CI produces \`mw2-switch-preflight.nro\` (this branch).
- M1: libnx guest-memory and runtime tests on physical Switch.
- M2: generated MW2 guest code cross-compiles with Horizon stubs.
- M3: renderer produces first MW2 frame/menu, no FPS expectation.
- M4: playable single-player first level with sound and controls.
