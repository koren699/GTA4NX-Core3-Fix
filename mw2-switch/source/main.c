/*
 * MW2 Switch preflight - NOT the Modern Warfare 2 game.
 * Purpose: verify devkitA64/libnx, NRO packaging, Horizon input, SD access.
 * No Xbox 360 code or copyrighted game resources are bundled.
 */
#include <switch.h>
#include <stdio.h>
#include <sys/stat.h>
#include "mw2_guest_layout.h"

static void print_probe(void) {
    struct stat s;
    const char *path = "sdmc:/switch/mw2/game/default.xex";
    puts("=== Call of Duty MW2: Switch port preflight ===");
    puts("ARM64 / HorizonOS / libnx bootstrap");
    puts("THIS IS A HARDWARE PROBE, NOT A PLAYABLE GAME.");
    printf("Guest VA requirement: %llu MiB (not mapped)\n",
           (unsigned long long)(MW2_GUEST_VA_SIZE / 1048576ULL));
    printf("Xbox 360 physical aperture: %llu MiB x 2 aliases\n",
           (unsigned long long)(MW2_GUEST_PHYS_BYTES / 1048576ULL));
    printf("SDMC check: %s\n", stat(path, &s) == 0 ? "default.xex detected" : "no XEX installed (fine)");
    puts("");
    puts("NEXT: guest VA aliases, GPU (NVK), XEX loader, audio.");
    puts("Press A to repeat probe; + to exit.");
}

int main(int argc, char **argv) {
    (void)argc; (void)argv;
    consoleInit(NULL);
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);
    PadState pad;
    padInitializeDefault(&pad);
    Result sd = fsdevMountSdmc();
    printf("SD mount: %s (0x%08lX)\n", R_SUCCEEDED(sd) ? "ok" : "unavailable", (unsigned long)sd);
    print_probe();
    while (appletMainLoop()) {
        padUpdate(&pad);
        u64 down = padGetButtonsDown(&pad);
        if (down & HidNpadButton_Plus) break;
        if (down & HidNpadButton_A) print_probe();
        consoleUpdate(NULL);
    }
    if (R_SUCCEEDED(sd)) fsdevUnmountDevice("sdmc");
    consoleExit(NULL);
    return 0;
}
