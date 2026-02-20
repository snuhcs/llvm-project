def tc_M8_mxmem_act : InstrItinClass;
def tc_M8_mxmem_wei : InstrItinClass;

class DepHMXItinV79 {
  list<InstrItinData> DepHMXItinV79_list = [
    InstrItinData <tc_M8_mxmem_act,
      [InstrStage<1, [SLOT0, SLOT1], 0>], [1], [Hex_FWD]>,
    InstrItinData <tc_M8_mxmem_wei,
      [InstrStage<1, [SLOT0], 0>], [1], [Hex_FWD]>,
  ];
}
