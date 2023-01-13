/* offsets.js -- offsets for exploitation
 *
 * Copyright (C) 2020 TheFloW, 2022 skgleba
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

SceWebKit_gadgets_v365 = {
  ldm_r0_r0_r1_r4_r8_fp_ip_pc: 0x5490e4,
  ldm_r1_r0_ip_lr_pc: 0x2b120,
  ldm_r1_r0_r2_r3_r8_fp_ip_pc: 0x62b6e0,
  ldm_r4_r3_r8_ip_pc: 0xb2bd4,
  ldm_r8_r1_r6_ip_lr_pc: 0x5579c0,
  str_r0_r3_bx_lr: 0x2431d1,
}

SceWebKit_gadgets_v367 = {
  ldm_r0_r0_r1_r4_r8_fp_ip_pc: 0x5490fc,
  ldm_r1_r0_ip_lr_pc: 0x2b120,
  ldm_r1_r0_r2_r3_r8_fp_ip_pc: 0x62b6f8,
  ldm_r4_r3_r8_ip_pc: 0xb2bd4,
  ldm_r8_r1_r6_ip_lr_pc: 0x5579d8,
  str_r0_r3_bx_lr: 0x2431ed,
}

SceWebKit_gadgets_v368_v374 = {
  ldm_r0_r0_r1_r4_r8_fp_ip_pc: 0x549a80,
  ldm_r1_r0_ip_lr_pc: 0x2b19c,
  ldm_r1_r0_r2_r3_r8_fp_ip_pc: 0x62f808,
  ldm_r4_r3_r8_ip_pc: 0xb2d3c,
  ldm_r8_r1_r6_ip_lr_pc: 0x5587b0,
  str_r0_r3_bx_lr: 0x24367d,
}

SceWebKit_offsets_v365 = {
  SceWebKit_base_off: 0xabb63c,
  SceNet_stub_off: 0x85f3f4,
  SceLibc_stub_off: 0x85f4e4,
  setscrollleft_off: 0x4e,
}

SceWebKit_offsets_v367 = {
  SceWebKit_base_off: 0xabb654,
  SceNet_stub_off: 0x85f40c,
  SceLibc_stub_off: 0x85f4fc,
  setscrollleft_off: 0x4e,
}

SceWebKit_offsets_v368_v374 = {
  SceWebKit_base_off: 0xac3544,
  SceNet_stub_off: 0x866ab8,
  SceLibc_stub_off: 0x866ba8,
  setscrollleft_off: 0x4e,
}

SceLibc_functions_v365_v374 = {
  malloc: 0xfa19,
  free: 0xfa29,
}

SceLibc_gadgets_v365_v374 = {
  setjmp: 0x14071,
  longjmp: 0x14099,
}

SceLibc_offsets_v365_v374 = {
  SceLibc_base_off: 0xfa49,
}

SceNet_functions_v365_v374 = {
  sceNetHtons: 0x23d5,
  sceNetSyscallClose: 0x9f60,
  sceNetSyscallBind: 0x9f80,
  sceNetSyscallIoctl: 0x9f90,
  sceNetSyscallSocket: 0xa030,
  sceNetSyscallGetIfList: 0xa050,
  sceNetSyscallSendmsg: 0xa0b0,
  sceNetSyscallControl: 0xa110,
}

SceNet_offsets_v365_v374 = {
  SceNet_base_off: 0x23ed,
}

SceNetPs_gadgets_v365_v370 = {
  blx_r0_pop_r3_pc: 0x2a11b,
  blx_r3_pop_r3_pc: 0x1959,
  blx_r4_add_sp_c_pop_r4_r5_pc: 0xe361,

  pop_pc: 0x1619f,
  pop_r0_r1_r2_r3_r4_r6_pc: 0x230cd,
  ldm_r0_r0_r1_r2_r3_r4_ip_sp_lr_pc: 0xb734,
  ldr_r0_r4_pop_r4_pc: 0x2a79d,

  str_r0_r4_pop_r4_pc: 0x1693f,

  push_r3_r4_lr_pop_r0_r1_r2_r6_r0_r1_r3_r4_r5_r6_pc: 0x2b067,
  add_r2_r4_pop_r4_r5_str_r2_r1_bx_lr: 0x24c1b,
  orrs_r0_r3_pop_r3_pc: 0x3c2f,
  movs_r0_0_pop_r3_pc: 0xce5,

  ksceKernelFreeMemBlock: 0x2a7a8,
  ksceKernelGetMemBlockBase: 0x2a7e8,
  ksceKernelMemcpyUserToKernel: 0x2a7f8,
  ksceKernelAllocMemBlock: 0x2a818,

  empty_string: 0x22,
}

SceNetPs_gadgets_v371_v374 = {
  blx_r0_pop_r3_pc: 0x2a14b,
  blx_r3_pop_r3_pc: 0x1959,
  blx_r4_add_sp_c_pop_r4_r5_pc: 0xe391,

  pop_pc: 0x161cf,
  pop_r0_r1_r2_r3_r4_r6_pc: 0x230fd,
  ldm_r0_r0_r1_r2_r3_r4_ip_sp_lr_pc: 0xb764,
  ldr_r0_r4_pop_r4_pc: 0x2a7cd,

  str_r0_r4_pop_r4_pc: 0x1696f,

  push_r3_r4_lr_pop_r0_r1_r2_r6_r0_r1_r3_r4_r5_r6_pc: 0x2b097,
  add_r2_r4_pop_r4_r5_str_r2_r1_bx_lr: 0x24c4b,
  orrs_r0_r3_pop_r3_pc: 0x3c2f,
  movs_r0_0_pop_r3_pc: 0xce5,

  ksceKernelFreeMemBlock: 0x2a7d8,
  ksceKernelGetMemBlockBase: 0x2a818,
  ksceKernelMemcpyUserToKernel: 0x2a828,
  ksceKernelAllocMemBlock: 0x2a848,

  empty_string: 0x22,
}

SceNetPs_offsets_v365_v370 = {
  SceNetPs_base_off: -0x8d45,
}

SceNetPs_offsets_v371_v374 = {
  SceNetPs_base_off: -0x8d75,
}

SceSysmem_offsets_v365_v374 = {
  SceSysmem_base_off: -0x810b,
  ksceKernelRemapBlock: 0x7f69,
  ksceKernelCpuDcacheWritebackRange: 0x22fcd,
}

supported_versions = {
  "3.65" : 0x03650011,
  "3.67" : 0x03670011,
  "3.68" : 0x03680011,
  "3.69" : 0x03690011,
  "3.70" : 0x03700011,
  "3.71" : 0x03710011,
  "3.72" : 0x03720011,
  "3.73" : 0x03730011,
  "3.74" : 0x03740011,
}

version_deps = {
  0x03650011: {
    "SceWebKit": {
      gadgets: SceWebKit_gadgets_v365,
      offsets: SceWebKit_offsets_v365,
    },
    "SceLibc": {
      functions: SceLibc_functions_v365_v374,
      gadgets: SceLibc_gadgets_v365_v374,
      offsets: SceLibc_offsets_v365_v374,
    },
    "SceNet": {
      functions: SceNet_functions_v365_v374,
      offsets: SceNet_offsets_v365_v374,
    },
    "SceNetPs": {
      gadgets: SceNetPs_gadgets_v365_v370,
      offsets: SceNetPs_offsets_v365_v370,
    },
    "SceSysmem": {
      offsets: SceSysmem_offsets_v365_v374,
    },
  },
  0x03670011: {
    "SceWebKit": {
      gadgets: SceWebKit_gadgets_v367,
      offsets: SceWebKit_offsets_v367,
    },
    "SceLibc": {
      functions: SceLibc_functions_v365_v374,
      gadgets: SceLibc_gadgets_v365_v374,
      offsets: SceLibc_offsets_v365_v374,
    },
    "SceNet": {
      functions: SceNet_functions_v365_v374,
      offsets: SceNet_offsets_v365_v374,
    },
    "SceNetPs": {
      gadgets: SceNetPs_gadgets_v365_v370,
      offsets: SceNetPs_offsets_v365_v370,
    },
    "SceSysmem": {
      offsets: SceSysmem_offsets_v365_v374,
    },
  },
  0x03680011: {
    "SceWebKit": {
      gadgets: SceWebKit_gadgets_v368_v374,
      offsets: SceWebKit_offsets_v368_v374,
    },
    "SceLibc": {
      functions: SceLibc_functions_v365_v374,
      gadgets: SceLibc_gadgets_v365_v374,
      offsets: SceLibc_offsets_v365_v374,
    },
    "SceNet": {
      functions: SceNet_functions_v365_v374,
      offsets: SceNet_offsets_v365_v374,
    },
    "SceNetPs": {
      gadgets: SceNetPs_gadgets_v365_v370,
      offsets: SceNetPs_offsets_v365_v370,
    },
    "SceSysmem": {
      offsets: SceSysmem_offsets_v365_v374,
    },
  },
  0x03690011: {
    "SceWebKit": {
      gadgets: SceWebKit_gadgets_v368_v374,
      offsets: SceWebKit_offsets_v368_v374,
    },
    "SceLibc": {
      functions: SceLibc_functions_v365_v374,
      gadgets: SceLibc_gadgets_v365_v374,
      offsets: SceLibc_offsets_v365_v374,
    },
    "SceNet": {
      functions: SceNet_functions_v365_v374,
      offsets: SceNet_offsets_v365_v374,
    },
    "SceNetPs": {
      gadgets: SceNetPs_gadgets_v365_v370,
      offsets: SceNetPs_offsets_v365_v370,
    },
    "SceSysmem": {
      offsets: SceSysmem_offsets_v365_v374,
    },
  },
  0x03700011: {
    "SceWebKit": {
      gadgets: SceWebKit_gadgets_v368_v374,
      offsets: SceWebKit_offsets_v368_v374,
    },
    "SceLibc": {
      functions: SceLibc_functions_v365_v374,
      gadgets: SceLibc_gadgets_v365_v374,
      offsets: SceLibc_offsets_v365_v374,
    },
    "SceNet": {
      functions: SceNet_functions_v365_v374,
      offsets: SceNet_offsets_v365_v374,
    },
    "SceNetPs": {
      gadgets: SceNetPs_gadgets_v365_v370,
      offsets: SceNetPs_offsets_v365_v370,
    },
    "SceSysmem": {
      offsets: SceSysmem_offsets_v365_v374,
    },
  },
  0x03710011: {
    "SceWebKit": {
      gadgets: SceWebKit_gadgets_v368_v374,
      offsets: SceWebKit_offsets_v368_v374,
    },
    "SceLibc": {
      functions: SceLibc_functions_v365_v374,
      gadgets: SceLibc_gadgets_v365_v374,
      offsets: SceLibc_offsets_v365_v374,
    },
    "SceNet": {
      functions: SceNet_functions_v365_v374,
      offsets: SceNet_offsets_v365_v374,
    },
    "SceNetPs": {
      gadgets: SceNetPs_gadgets_v371_v374,
      offsets: SceNetPs_offsets_v371_v374,
    },
    "SceSysmem": {
      offsets: SceSysmem_offsets_v365_v374,
    },
  },
  0x03720011: {
    "SceWebKit": {
      gadgets: SceWebKit_gadgets_v368_v374,
      offsets: SceWebKit_offsets_v368_v374,
    },
    "SceLibc": {
      functions: SceLibc_functions_v365_v374,
      gadgets: SceLibc_gadgets_v365_v374,
      offsets: SceLibc_offsets_v365_v374,
    },
    "SceNet": {
      functions: SceNet_functions_v365_v374,
      offsets: SceNet_offsets_v365_v374,
    },
    "SceNetPs": {
      gadgets: SceNetPs_gadgets_v371_v374,
      offsets: SceNetPs_offsets_v371_v374,
    },
    "SceSysmem": {
      offsets: SceSysmem_offsets_v365_v374,
    },
  },
  0x03730011: {
    "SceWebKit": {
      gadgets: SceWebKit_gadgets_v368_v374,
      offsets: SceWebKit_offsets_v368_v374,
    },
    "SceLibc": {
      functions: SceLibc_functions_v365_v374,
      gadgets: SceLibc_gadgets_v365_v374,
      offsets: SceLibc_offsets_v365_v374,
    },
    "SceNet": {
      functions: SceNet_functions_v365_v374,
      offsets: SceNet_offsets_v365_v374,
    },
    "SceNetPs": {
      gadgets: SceNetPs_gadgets_v371_v374,
      offsets: SceNetPs_offsets_v371_v374,
    },
    "SceSysmem": {
      offsets: SceSysmem_offsets_v365_v374,
    },
  },
  0x03740011: {
    "SceWebKit": {
      gadgets: SceWebKit_gadgets_v368_v374,
      offsets: SceWebKit_offsets_v368_v374,
    },
    "SceLibc": {
      functions: SceLibc_functions_v365_v374,
      gadgets: SceLibc_gadgets_v365_v374,
      offsets: SceLibc_offsets_v365_v374,
    },
    "SceNet": {
      functions: SceNet_functions_v365_v374,
      offsets: SceNet_offsets_v365_v374,
    },
    "SceNetPs": {
      gadgets: SceNetPs_gadgets_v371_v374,
      offsets: SceNetPs_offsets_v371_v374,
    },
    "SceSysmem": {
      offsets: SceSysmem_offsets_v365_v374,
    },
  },
};
