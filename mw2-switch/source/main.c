/*
 * MW2 Switch preflight v0.2 - NOT the Modern Warfare 2 game.
 * Checks Horizon/libnx input, filesystem and guest address layout math.
 * No title code or game assets are bundled.
 */
#include <switch.h>
#include <stdio.h>
#include <sys/stat.h>
#include "mw2_guest_layout.h"

static bool sd_mounted_here = false;
static bool sd_usable = false;
static Result sd_mount_result = 0;

static void init_sd(void) {
    /* hbmenu/libnx startup may have already mounted sdmc:.
     * A second fsdevMountSdmc() returns LibnxError_OutOfMemory (0x559).
     * Do NOT interpret that as an SD card failure, or unmount someone
     * else's mount when the program exits.
     */
    if (fsdevGetDeviceFileSystem("sdmc") != NULL) {
        sd_usable = true;
        return;
    }
    sd_mount_result = fsdevMountSdmc();
    if (R_SUCCEEDED(sd_mount_result)) {
        sd_mounted_here = true;
        sd_usable = true;
    }
}

static void render_probe(void) {
    struct stat st;
    const char *xex = "sdmc:/switch/mw2/game/default.xex";
    const bool valid_guest_layout =
        MW2_GUEST_VA_SIZE == 0x100000000ULL &&
        MW2_GUEST_PHYS_BYTES == 0x20000000ULL &&
        (uint64_t)(MW2_GUEST_PHYS_END - MW2_GUEST_PHYS_BASE) == MW2_GUEST_PHYS_BYTES &&
        (uint64_t)(MW2_GUEST_ALIAS_BASE - MW2_GUEST_PHYS_BASE) == MW2_GUEST_PHYS_BYTES;

    consoleClear();
    puts("=== MW2 Switch port: preflight v0.2 ===");
    puts("NOT THE GAME - Horizon / libnx test");
    puts("");
    puts("CPU: ARM64    Input: working (A)");
    if (sd_usable)
        puts("SDMC: already mounted / accessible");
    else
        printf("SDMC: failed (0x%08lX)\n", (unsigned long)sd_mount_result);

    if (sd_usable) {
        if (stat("sdmc:/", &st) == 0)
            puts("SD root: readable");
        else
            puts("SD root: cannot stat - check file access");
        printf("MW2 XEX: %s\n", stat(xex, &st) == 0 ? "found" : "not installed (expected)");
    }

    puts("");
    printf("Guest VA layout: %s (math only)\n", valid_guest_layout ? "OK" : "FAILED");
    puts("Requested VA: 4096 MiB (NOT reserved)");
    puts("Physical bank: 512 MiB, two aliases (NOT mapped)");
    puts("");
    puts("Next: real Horizon VA mapping and NVK.");
    puts("Press A: refresh | +: exit");
}

int main(int argc, char **argv) {
    (void)argc; (void)argv;
    consoleInit(NULL);
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);
    PadState pad;
    padInitializeDefault(&pad);
    init_sd();
    render_probe();

    while (appletMainLoop()) {
        padUpdate(&pad);
        const u64 down = padGetButtonsDown(&pad);
        if (down & HidNpadButton_Plus) break;
        if (down & HidNpadButton_A) render_probe();
        consoleUpdate(NULL);
    }

    if (sd_mounted_here)
        fsdevUnmountDevice("sdmc");
    consoleExit(NULL);
    return 0;
}
