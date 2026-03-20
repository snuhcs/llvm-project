def tc_M8_mx_acc_control : InstrItinClass;
def tc_M8_mxmem_load : InstrItinClass;
def tc_M8_mxmem_store : InstrItinClass;
def tc_M8_mxmem_bias_load : InstrItinClass;
def tc_M8_mxmem_bias_store : InstrItinClass;
def tc_M8_mx_cvt_acc : InstrItinClass;
def tc_M8_mxmem_cvt_store : InstrItinClass;

class DepHMXItinV69 {
  list<InstrItinData> DepHMXItinV69_list = [
    InstrItinData <tc_M8_mx_acc_control,
      [InstrStage<1, [SLOT0]>], [], []>,
    InstrItinData <tc_M8_mxmem_load,
      [InstrStage<1, [SLOT0, SLOT1], 0>,
       InstrStage<1, [CVI_LD]>], [1, 2], 
       [Hex_FWD, Hex_FWD]>,
    InstrItinData <tc_M8_mxmem_store,
      [InstrStage<1, [SLOT0, SLOT1], 0>,
       InstrStage<1, [CVI_ST]>], [1, 2], 
       [Hex_FWD, Hex_FWD]>,
    InstrItinData <tc_M8_mxmem_bias_load,
      [InstrStage<1, [SLOT0], 0>,
       InstrStage<1, [CVI_LD]>], [1], 
       [Hex_FWD]>,
    InstrItinData <tc_M8_mxmem_bias_store,
      [InstrStage<1, [SLOT0], 0>,
       InstrStage<1, [CVI_ST]>], [1], 
       [Hex_FWD]>,
  ];
}

class DepHMXItinV71 {
  list<InstrItinData> DepHMXItinV71_list = [
    InstrItinData <tc_M8_mx_acc_control,
      [InstrStage<1, [SLOT0]>], [], []>,
    InstrItinData <tc_M8_mxmem_load,
      [InstrStage<1, [SLOT0, SLOT1], 0>,
       InstrStage<1, [CVI_LD]>], [1, 2], 
       [Hex_FWD, Hex_FWD]>,
    InstrItinData <tc_M8_mxmem_store,
      [InstrStage<1, [SLOT0, SLOT1], 0>,
       InstrStage<1, [CVI_ST]>], [1, 2], 
       [Hex_FWD, Hex_FWD]>,
    InstrItinData <tc_M8_mxmem_bias_load,
      [InstrStage<1, [SLOT0], 0>,
       InstrStage<1, [CVI_LD]>], [1], 
       [Hex_FWD]>,
    InstrItinData <tc_M8_mxmem_bias_store,
      [InstrStage<1, [SLOT0], 0>,
       InstrStage<1, [CVI_ST]>], [1], 
       [Hex_FWD]>,
    InstrItinData <tc_M8_mx_cvt_acc,
      [InstrStage<1, [SLOT0]>], [1], 
       [Hex_FWD]>,
    InstrItinData <tc_M8_mxmem_cvt_store,
      [InstrStage<1, [SLOT0, SLOT1], 0>,
       InstrStage<1, [CVI_ST]>], [1, 2], 
       [Hex_FWD, Hex_FWD]>,
  ];
}

class DepHMXItinV73 {
  list<InstrItinData> DepHMXItinV73_list = [
    InstrItinData <tc_M8_mx_acc_control,
      [InstrStage<1, [SLOT0]>], [], []>,
    InstrItinData <tc_M8_mxmem_load,
      [InstrStage<1, [SLOT0, SLOT1], 0>,
       InstrStage<1, [CVI_LD]>], [1, 2], 
       [Hex_FWD, Hex_FWD]>,
    InstrItinData <tc_M8_mxmem_store,
      [InstrStage<1, [SLOT0, SLOT1], 0>,
       InstrStage<1, [CVI_ST]>], [1, 2], 
       [Hex_FWD, Hex_FWD]>,
    InstrItinData <tc_M8_mxmem_bias_load,
      [InstrStage<1, [SLOT0], 0>,
       InstrStage<1, [CVI_LD]>], [1], 
       [Hex_FWD]>,
    InstrItinData <tc_M8_mxmem_bias_store,
      [InstrStage<1, [SLOT0], 0>,
       InstrStage<1, [CVI_ST]>], [1], 
       [Hex_FWD]>,
    InstrItinData <tc_M8_mx_cvt_acc,
      [InstrStage<1, [SLOT0]>], [1], 
       [Hex_FWD]>,
    InstrItinData <tc_M8_mxmem_cvt_store,
      [InstrStage<1, [SLOT0, SLOT1], 0>,
       InstrStage<1, [CVI_ST]>], [1, 2], 
       [Hex_FWD, Hex_FWD]>,
  ];
}

class DepHMXItinV75 {
  list<InstrItinData> DepHMXItinV75_list = [
    InstrItinData <tc_M8_mx_acc_control,
      [InstrStage<1, [SLOT0]>], [], []>,
    InstrItinData <tc_M8_mxmem_load,
      [InstrStage<1, [SLOT0, SLOT1], 0>,
       InstrStage<1, [CVI_LD]>], [1, 2], 
       [Hex_FWD, Hex_FWD]>,
    InstrItinData <tc_M8_mxmem_store,
      [InstrStage<1, [SLOT0, SLOT1], 0>,
       InstrStage<1, [CVI_ST]>], [1, 2], 
       [Hex_FWD, Hex_FWD]>,
    InstrItinData <tc_M8_mxmem_bias_load,
      [InstrStage<1, [SLOT0], 0>,
       InstrStage<1, [CVI_LD]>], [1], 
       [Hex_FWD]>,
    InstrItinData <tc_M8_mxmem_bias_store,
      [InstrStage<1, [SLOT0], 0>,
       InstrStage<1, [CVI_ST]>], [1], 
       [Hex_FWD]>,
    InstrItinData <tc_M8_mx_cvt_acc,
      [InstrStage<1, [SLOT0]>], [1], 
       [Hex_FWD]>,
    InstrItinData <tc_M8_mxmem_cvt_store,
      [InstrStage<1, [SLOT0, SLOT1], 0>,
       InstrStage<1, [CVI_ST]>], [1, 2], 
       [Hex_FWD, Hex_FWD]>,
  ];
}

class DepHMXItinV79 {
  list<InstrItinData> DepHMXItinV79_list = [
    InstrItinData <tc_M8_mx_acc_control,
      [InstrStage<1, [SLOT0]>], [], []>,
    InstrItinData <tc_M8_mxmem_load,
      [InstrStage<1, [SLOT0, SLOT1], 0>,
       InstrStage<1, [CVI_LD]>], [1, 2], 
       [Hex_FWD, Hex_FWD]>,
    InstrItinData <tc_M8_mxmem_store,
      [InstrStage<1, [SLOT0, SLOT1], 0>,
       InstrStage<1, [CVI_ST]>], [1, 2], 
       [Hex_FWD, Hex_FWD]>,
    InstrItinData <tc_M8_mxmem_bias_load,
      [InstrStage<1, [SLOT0], 0>,
       InstrStage<1, [CVI_LD]>], [1], 
       [Hex_FWD]>,
    InstrItinData <tc_M8_mxmem_bias_store,
      [InstrStage<1, [SLOT0], 0>,
       InstrStage<1, [CVI_ST]>], [1], 
       [Hex_FWD]>,
    InstrItinData <tc_M8_mx_cvt_acc,
      [InstrStage<1, [SLOT0]>], [1], 
       [Hex_FWD]>,
    InstrItinData <tc_M8_mxmem_cvt_store,
      [InstrStage<1, [SLOT0, SLOT1], 0>,
       InstrStage<1, [CVI_ST]>], [1, 2], 
       [Hex_FWD, Hex_FWD]>,
  ];
}
