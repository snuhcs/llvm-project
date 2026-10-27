# HMX MAC recipe notes (hexagon-sim)

Shared harness: `hmx_matmul_main.c` + `recipe_<name>.txt`. Every recipe
exports `hmx_mac_tile(bias, A, B, C)`. Tiles stay **2048-byte aligned** in
VTCM (`HAP_request_VTCM`). Feature gate: `hexagon-sim-hmx`.

## Bias

`mxmem2.bias` loads 256 B (32 channels × 64-bit). Scale lives in the low
fp16 of each channel’s lower 32-bit word. **Identity** = fp16 `1.0` at
`bias[2*ch]` for `ch in 0..31` (same layout as `InsertHMXBiasSetup` /
`identity_bias.bin`). Byte-fill `0x01` is *not* identity and makes A/B
appear inert.

## hf

- Clear: `mxclracc.hf`
- Act/wei/drain: `blk.sm.act.hf` / `wei.hf` / `mxcvtr.sat.hf`
- Rt: act `0x7FFF`, wei `0x780`, drain `0`
- Layout: AH/WH row-pair halfword interleave (`gen_tiles.py`)
- Golden: host fp32 matmul → fp16 → AH

## ub × weight-tag

- Clear: `mxclracc` (not `.hf`)
- Act/drain: `blk.sm.act.ub` / `mxcvtr.sat.ub`
- Wei intrinsic names: `wei.b`, `wei.n`, `wei.c`, `wei.b1` (ubit),
  `wei.sb1` (sbit), `wei.sc`, `wei.sm`, `wei.n.2x`
- Rt placeholders: same as hf (`0x7FFF` / `0x780` / `0`) — sufficient for
  uniform-tile smoke on Tools 19.0.04 `--mhmx`
- Tiles: **uniform byte fills** (see table). With identity bias, a uniform
  `act.ub × wei.b` (and `wei.sm`) tile drains to `(A*B)>>4` across all 2048
  bytes. Other tags need tag-specific fills (nibble/bit packing); goldens
  are the observed uniform drain values, not a closed-form matmul.

| recipe | wei | A fill | B fill | C fill |
| --- | --- | --- | --- | --- |
| ubxb | b | `0x08` | `0x08` | `0x04` |
| ubxn | n | `0x11` | `0x11` | `0x01` |
| ubxc | c | `0x55` | `0x55` | `0x05` |
| ubxubit | b1 | `0xFF` | `0x01` | `0x01` |
| ubxsbit | sb1 | `0x88` | `0x88` | `0x04` |
| ubxsc | sc | `0x11` | `0x11` | `0x01` |
| ubxsm | sm | `0x08` | `0x08` | `0x04` |
| ubxn2x | n.2x | `0x11` | `0x11` | `0x12` |

There is no in-tree hexkl ub reference; these lit goldens lock codegen +
sim behavior for the chosen fills.

## f8

- Act/wei exist at v79 (`blk.sm.act.f8` / `wei.f8`); AsmStrings fixed to
  print `activation.f8` (were corrupted as `activation.hf`)
- Drain is V81: `cvt.f8=acc(Rs)` (`M8_cvt_rs_f8`) then
  `mxmem(Rs,Rt).f8=cvt` (`M8_mxmem_f8`); IR + ISel landed from Tools
  19.0.04 `-mhmx` encodings
- Clear: `mxclracc` (same as ub) for the smoke that reached the drain
- **Sim status (Tools 19.0.04):** QuRT + `--mhmx` on `-mv79na_1` and
  `-mv81` both take a precise exception on the drain (BADVA in the VTCM
  window). `hmx_matmul_f8.test` therefore requires feature
  `hexagon-sim-hmx-f8`, which stays off until a manual smoke prints
  `PASS` and `HEXAGON_HMX_F8_SIM=1` is set (or the probe is updated).
  Do not treat a green FileCheck of the `.ll` asm as numerical coverage.
