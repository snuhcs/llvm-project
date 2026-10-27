/* Optional bare-metal L2TCM placement (not used by the QuRT lit path).
 *
 * hexagon-sim --l2tcm_base 0xd800 maps physical L2TCM at 0xd8000000. A
 * standalone ELF can place tiles here; under QuRT user PD the same VA is not
 * mapped — the lit harness uses HAP_request_VTCM instead (see README.md).
 */
MEMORY {
  L2TCM (rwx) : ORIGIN = 0xd8001000, LENGTH = 64K
}
SECTIONS {
  .l2tcm.data 0xd8001000 : {
    KEEP(*(.l2tcm.data))
  } > L2TCM
}
