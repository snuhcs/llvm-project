#!/usr/bin/env python3
"""Regenerate HMX lit tiles: hf AH/WH golden + ub uniform-fill recipes.

hf: hexkl row-pair halfword interleave (packed-tile WH == AH). Host
reference is fp32 matmul → fp16 → AH. Identity bias matches
InsertHMXBiasSetup (32× (1,0) + zeros).

ub: uniform byte fills with observed hexagon-sim drain values (see
RECIPES.md). No closed-form ub layout reference in-tree.
"""
from __future__ import annotations

import argparse
from pathlib import Path

import numpy as np

SEED = 0x484D58

# name -> (A_fill, B_fill, C_fill)
UB_RECIPES: dict[str, tuple[int, int, int]] = {
    "ubxb": (0x08, 0x08, 0x04),
    "ubxn": (0x11, 0x11, 0x01),
    "ubxc": (0x55, 0x55, 0x05),
    "ubxubit": (0xFF, 0x01, 0x01),
    "ubxsbit": (0x88, 0x88, 0x04),
    "ubxsc": (0x11, 0x11, 0x01),
    "ubxsm": (0x08, 0x08, 0x04),
    "ubxn2x": (0x11, 0x11, 0x12),
}


def rm_to_ah(rm: np.ndarray) -> np.ndarray:
    assert rm.shape == (32, 32)
    ah = np.empty(1024, dtype=np.float16)
    for p in range(16):
        for c in range(32):
            ah[64 * p + 2 * c] = rm[2 * p, c]
            ah[64 * p + 2 * c + 1] = rm[2 * p + 1, c]
    return ah


def identity_bias() -> np.ndarray:
    bias = np.zeros(128, dtype=np.float16)
    for ch in range(32):
        bias[2 * ch] = np.float16(1.0)
    return bias


def checksum_u16(buf: bytes) -> int:
    cs = 0
    for i in range(0, len(buf) - 1, 2):
        cs ^= buf[i] | (buf[i + 1] << 8)
    if len(buf) & 1:
        cs ^= buf[-1]
    return cs


def write_hf(out: Path, seed: int) -> None:
    rng = np.random.default_rng(seed)
    a_rm = rng.standard_normal((32, 32), dtype=np.float32).astype(np.float16)
    b_rm = rng.standard_normal((32, 32), dtype=np.float32).astype(np.float16)
    c_rm = (a_rm.astype(np.float32) @ b_rm.astype(np.float32)).astype(np.float16)

    a_ah = rm_to_ah(a_rm)
    b_wh = rm_to_ah(b_rm)  # packed contiguous WH ≡ AH
    c_ah = rm_to_ah(c_rm)
    bias = identity_bias()

    a_ah.tofile(out / "tile_a_ah.f16")
    b_wh.tofile(out / "tile_b_wh.f16")
    c_ah.tofile(out / "tile_c_ah_expected.f16")
    bias.tofile(out / "identity_bias.bin")

    cs = 0
    for x in c_ah.view(np.uint16):
        cs ^= int(x)
    print(f"hf checksum_u16_xor=0x{cs:04x}")


def write_ub(out: Path) -> None:
    identity_bias().tofile(out / "identity_bias.bin")
    for name, (af, bf, cf) in UB_RECIPES.items():
        a = bytes([af]) * 2048
        b = bytes([bf]) * 2048
        c = bytes([cf]) * 2048
        (out / f"tile_a_{name}.bin").write_bytes(a)
        (out / f"tile_b_{name}.bin").write_bytes(b)
        (out / f"tile_c_{name}_expected.bin").write_bytes(c)
        (out / f"recipe_{name}.txt").write_text(
            "has_bias 1\n"
            "bias_bytes 256\n"
            "a_bytes 2048\n"
            "b_bytes 2048\n"
            "c_bytes 2048\n"
            "bias_file identity_bias.bin\n"
            f"a_file tile_a_{name}.bin\n"
            f"b_file tile_b_{name}.bin\n"
            f"c_file tile_c_{name}_expected.bin\n"
        )
        print(f"{name}: A={af:#x} B={bf:#x} C={cf:#x} PASS 0x{checksum_u16(c):x}")


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument(
        "-o",
        "--out-dir",
        type=Path,
        default=Path(__file__).resolve().parent,
        help="Directory for identity_bias.bin / tile_*",
    )
    ap.add_argument("--seed", type=int, default=SEED)
    ap.add_argument(
        "--recipes",
        choices=("all", "hf", "ub"),
        default="all",
        help="Which tile families to regenerate",
    )
    args = ap.parse_args()
    out: Path = args.out_dir
    out.mkdir(parents=True, exist_ok=True)
    if args.recipes in ("all", "hf"):
        write_hf(out, args.seed)
    if args.recipes in ("all", "ub"):
        write_ub(out)
    print(f"wrote tiles to {out}")


if __name__ == "__main__":
    main()
