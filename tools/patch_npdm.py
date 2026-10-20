#!/usr/bin/env python3
"""Produce an Atmosphere Core 3 ExeFS override from your OWN GTA4NX forwarder NPDM.

No third-party Nintendo/Game/Rockstar files are distributed by this tool.
Only the highest allowed core of the ThreadInfo kernel capability is modified.
"""
import argparse
import hashlib
from pathlib import Path
import struct
import sys
import zipfile

TITLE_ID = 0x03DB149512496000
TARGET = Path('atmosphere') / 'contents' / f'{TITLE_ID:016X}' / 'exefs' / 'main.npdm'


def u32(data, off):
    if off < 0 or off + 4 > len(data):
        raise ValueError(f'NPDM offset out of range: 0x{off:X}')
    return struct.unpack_from('<I', data, off)[0]


def u64(data, off):
    if off < 0 or off + 8 > len(data):
        raise ValueError(f'NPDM offset out of range: 0x{off:X}')
    return struct.unpack_from('<Q', data, off)[0]


def region(data, base, size, name):
    if base < 0x80 or size < 0x40 or base + size > len(data):
        raise ValueError(f'Invalid {name} NPDM region')


def get_kc(data, section, sec_off, sec_size):
    """Locate kernel capability table and check bounds against enclosing section."""
    if section == 'ACI0':
        if data[sec_off:sec_off + 4] != b'ACI0':
            raise ValueError('ACI0 header missing')
        rel, size = u32(data, sec_off + 0x30), u32(data, sec_off + 0x34)
    else:
        if sec_size < 0x238 or data[sec_off + 0x200:sec_off + 0x204] != b'ACID':
            raise ValueError('ACID header missing')
        rel, size = u32(data, sec_off + 0x230), u32(data, sec_off + 0x234)
    if not (size >= 4 and size % 4 == 0 and rel >= 0x38 and rel + size <= sec_size):
        raise ValueError(f'Invalid {section} kernel-capability table')
    return sec_off + rel, size


def patch_npdm(content):
    if len(content) < 0x100 or content[:4] != b'META':
        raise ValueError('Not a valid META NPDM')
    data = bytearray(content)
    aci, aci_size = u32(data, 0x70), u32(data, 0x74)
    acid, acid_size = u32(data, 0x78), u32(data, 0x7C)
    region(data, aci, aci_size, 'ACI0')
    region(data, acid, acid_size, 'ACID')
    if aci + aci_size > acid and acid + acid_size > aci:
        raise ValueError('Overlapping NPDM sections')
    program_id = u64(data, aci + 0x10)
    if program_id != TITLE_ID:
        raise ValueError(f'Unexpected program ID {program_id:016X}; requires {TITLE_ID:016X}')
    result = []
    for name, base, size in [('ACI0', aci, aci_size), ('ACID', acid, acid_size)]:
        kc_off, kc_size = get_kc(data, name, base, size)
        entries = [(kc_off + i, u32(data, kc_off + i)) for i in range(0, kc_size, 4)]
        thread_info = [(off, word) for off, word in entries if word & 0xF == 0x7]
        if len(thread_info) != 1:
            raise ValueError(f'{name}: expected 1 ThreadInfo capability, got {len(thread_info)}')
        off, word = thread_info[0]
        min_core, max_core = (word >> 16) & 0xFF, (word >> 24) & 0xFF
        if not (min_core <= 2 and max_core in (2, 3)):
            raise ValueError(f'{name}: unexpected core range {min_core}..{max_core}; refusing patch')
        patched = (word & 0x00FFFFFF) | (3 << 24)
        struct.pack_into('<I', data, off, patched)
        result.append((name, off, min_core, max_core, 3))
    # Re-read all changed positions to avoid silently generating the wrong output.
    for _, off, _, _, expected in result:
        assert (u32(data, off) >> 24) & 0xFF == expected
    return bytes(data), result


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('original_npdm', type=Path, help='main.npdm from your matching GTA4NX forwarder NSP')
    ap.add_argument('--out-dir', type=Path, default=Path('Core3_Output'), help='Folder to create')
    ap.add_argument('--zip', action='store_true', help='Also create an Atmosphere-ready zip')
    args = ap.parse_args()
    src = args.original_npdm.read_bytes()
    output, entries = patch_npdm(src)
    target = args.out_dir / TARGET
    target.parent.mkdir(parents=True, exist_ok=True)
    if target.exists():
        raise ValueError(f'Output already exists: {target}. Move/delete it first.')
    target.write_bytes(output)
    print('Source SHA-256:', hashlib.sha256(src).hexdigest())
    print('Output SHA-256:', hashlib.sha256(output).hexdigest())
    for sec, off, mn, before, after in entries:
        print(f'{sec} @ 0x{off:X}: Core {mn}..{before} -> {mn}..{after}')
    print('Created:', target)
    if args.zip:
        zipped = args.out_dir / 'GTA4NX_Core3_Atmosphere_Override.zip'
        with zipfile.ZipFile(zipped, 'w', zipfile.ZIP_DEFLATED) as z:
            z.write(target, TARGET.as_posix())
        print('Created:', zipped)


if __name__ == '__main__':
    try:
        main()
    except (OSError, ValueError) as exc:
        print('ERROR:', exc, file=sys.stderr)
        sys.exit(1)
