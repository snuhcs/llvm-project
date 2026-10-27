# HMX MAC numerical correctness (hexagon-sim)

Optional lit suite: production-shaped HMX MAC tiles compiled with **in-tree
`llc`**, linked and run under SDK **QuRT + `hexagon-sim --mhmx`**, checked
against checked-in goldens. Covers:

| Family | Recipes | Clear / act / wei / drain |
| --- | --- | --- |
| **hf** | `hmx_matmul_32x32x32` | `mxclracc.hf` / `blk.sm.act.hf` / `wei.hf` / `mxcvtr.sat.hf` |
| **ub × weight-tag** | `ubxb` `ubxn` `ubxc` `ubxubit` `ubxsbit` `ubxsc` `ubxsm` `ubxn2x` | `mxclracc` / `blk.sm.act.ub` / `wei.<tag>` / `mxcvtr.sat.ub` |
| **f8** | `hmx_matmul_f8` (gated) | `mxclracc` / `blk.sm.act.f8` / `wei.f8` / `cvt.f8=acc` + `mxmem.f8=cvt` |

Asm / packetizer regressions stay in the parent `CodeGen/Hexagon/` directory
(`hmx_*.ll`). Attention/linear layout ladders and MetaSchedule stay in TVM
(`tests/python/contrib/test_hexagon/test_hmx_matmul_device_correctness.py`).
TVM production remains **fp16-only**; these lits lock LLVM dtype coverage on
sim.

Act-mode × wei-mode cartesian (`:deep` / `:dilate` / `:single` / …) shares
accumulator math and stays as sibling FileCheck packetizer tests, not separate
goldens. Cross-family pairs (`act.hf` × `wei.b`) are out of numerical scope.

Recipe / Rt / layout notes: [`Inputs/RECIPES.md`](Inputs/RECIPES.md).

## Requirements

| Piece | Role |
| --- | --- |
| In-tree `llc` with Hexagon + HMX | Codegen under test (pairing / packetizer) |
| SDK `hexagon-clang` / `hexagon-ar` / `hexagon-sim` | Link CRT, extract HAP client, run sim |
| QuRT `computev79` `runelf.pbn` + `run_main_on_hexagon_sim` | Enable HMX (`qurt_hmx_lock2`) and load the `.so` |
| `utils/sim_utils/.../test_util.a` (`sysmon_vtcm_mgr_client.o`) | `HAP_request_VTCM` (QDI into runelf’s VTCM manager) |
| `libncurses.so.5` (or compatible) | Dynamic dependency of `hexagon-sim` |

Env (any one tools root is enough):

```bash
export HEXAGON_SDK_ROOT=/path/to/Hexagon_SDK/6.4.0.1
# optional overrides:
export HEXAGON_TOOLS=$HEXAGON_SDK_ROOT/tools/HEXAGON_Tools/19.0.04/Tools
```

If the probe in `lit.local.cfg` fails (no Tools, missing ncurses, no QuRT),
the suite is **UNSUPPORTED** and default Hexagon lit CI stays green. The f8
lit additionally needs feature `hexagon-sim-hmx-f8` (V81 drain on `--mhmx`).

## Run

```bash
# from an LLVM build that includes this fork’s llc:
llvm-lit 3rdparty/llvm-project/llvm/test/CodeGen/Hexagon/hmx/
# or one test:
llvm-lit 3rdparty/llvm-project/llvm/test/CodeGen/Hexagon/hmx/hmx_matmul_32x32x32.test
```

hf expects `PASS 0x69f0`. ub uniform goldens expect `PASS 0x0` (even-length
XOR of identical halfwords). Regenerate tiles with `Inputs/gen_tiles.py`.

## Memory placement

`mxmem` needs **VTCM**, not DDR `.bss`. Under QuRT user PD a linker script at
`--l2tcm_base` is not enough (TLBMISS). The harness:

1. `HAP_request_VTCM(32768, 1)` → VTCM VA (typically `0xd9……`)
2. Places **bias** at a 2048-aligned base, then **A / B / C tiles** at
   `+2048 / +4096 / +6144` (tiles must be **2048-byte aligned**)
3. Runs sim with `--l2tcm_base 0xd800` so the physical L2TCM backing exists

`Inputs/l2tcm.ld` documents the bare-metal VMA idea; the lit path does not
link with it.

## Kernel contract

Every recipe `.ll` exports `hmx_mac_tile(bias, A, B, C)`. The QuRT main reads
`recipe.txt` for tile sizes / filenames (see `Inputs/hmx_matmul_main.c`).

**hf** (see `Inputs/hmx_matmul_32x32x32.ll`):

1. `llvm.hexagon.M8.mxmem2.bias` — identity bias (256 B)
2. `llvm.hexagon.M8.mxclracc.hf`
3. `llvm.hexagon.M8.mxmem.blk.sm.act.hf` Rt `0x7FFF`
4. `llvm.hexagon.M8.mxmem.wei.hf` Rt `0x780` (same packet as act)
5. `llvm.hexagon.M8.mxcvtr.sat.hf` Rt `0`

AH/WH layout: hexkl row-pair halfword interleave; on a packed 32×32 tile
WH ≡ AH (`Inputs/gen_tiles.py`).

**ub**: same bias + Rt placeholders; clear is `mxclracc`; act/drain are
`.ub`; wei is the tag intrinsic (`wei.b1` = ubit, `wei.sb1` = sbit).
