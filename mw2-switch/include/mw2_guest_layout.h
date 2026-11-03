#pragma once
/* Xbox 360 guest virtual address layout used by MW2 Recompiled.
 * Constants only; mapping these ranges on Horizon is NOT implemented yet.
 * Source: PaulCombal/mw2-recompiled runtime/guest_memory.h
 */
#define MW2_GUEST_VA_SIZE   (0x100000000ULL)
#define MW2_GUEST_HEAP_BASE (0x84600000u)
#define MW2_GUEST_HEAP_END  (0xA0000000u)
#define MW2_GUEST_PHYS_BASE (0xA0000000u)
#define MW2_GUEST_PHYS_END  (0xC0000000u)
#define MW2_GUEST_ALIAS_BASE (0xC0000000u)
#define MW2_GUEST_PHYS_BYTES (0x20000000ULL)
