/*
 * Safety_SCST.h
 *
 *  Created on: 2021Äê9ÔÂ27ÈÕ
 *      Author: zsy342
 */

#ifndef SAFTYCHECK_SAFETYLIB_SAFETY_SCST_H_
#define SAFTYCHECK_SAFETYLIB_SAFETY_SCST_H_

typedef enum
{
    m4_scst_exception_test_svc = 0x000012E0,       			 	  /* 0 */
    m4_scst_exception_test_pendsv = 0x00000E7C,                   /* 1 */
    m4_scst_exception_test_systick = 0x000006EA,                  /* 2 */
    m4_scst_exception_hard_fault1 = 0x00000D89,                   /* 3 */
    m4_scst_exception_hard_fault2 = 0x00001A69,                   /* 4 */
    m4_scst_exception_usage_fault = 0x0000205A,                   /* 5 */
    m4_scst_exception_mem_fault = 0x00000C3B,                     /* 6 */
    m4_scst_exception_bus_fault = 0x00000FE3,                     /* 7 */
    m4_scst_exception_test_nmihf = 0x0000256C,                    /* 8 */
    m4_scst_exception_test_tail_chain = 0x00002784,               /* 9 */
    m4_scst_exception_test_masking = 0x00002B88,                  /* 10 */
    m4_scst_exception_test_handler_thread = 0x0000302F,           /* 11 */
    m4_scst_regbank_test4 = 0x000047DB,                           /* 12 */
    m4_scst_alu_test7 = 0x00001C21,                               /* 13 */
    m4_scst_branch_test3 = 0x7E256E6B,                            /* 14 */
    m4_scst_status_test3 = 0x00003A0E,                            /* 15 */
    m4_scst_regbank_test6 = 0xB91BDE95,                           /* 16 */
    m4_scst_fetch_test = 0xB14D3A0D,                              /* 17 */
    m4_scst_loadstore_test6 = 0xC19005EE,                         /* 18 */
    m4_scst_loadstore_test1 = 0xC63C044C,                         /* 19 */
    m4_scst_loadstore_test2 = 0xAE1D1D83,                         /* 20 */
    m4_scst_loadstore_test3 = 0x5B80787E,                         /* 21 */
    m4_scst_loadstore_test4 = 0x618271EE,                         /* 22 */
    m4_scst_loadstore_test5 = 0xF61A1F74,                         /* 23 */
    m4_scst_regbank_test1 = 0xE35DF821,                           /* 24 */
    m4_scst_regbank_test2 = 0x28D6AF31,                           /* 25 */
    m4_scst_regbank_test3 = 0xC726A3D2,                           /* 26 */
    m4_scst_regbank_test5 = 0x00001AC7,                           /* 27 */
    m4_scst_mac_test1 = 0x0000A828,                               /* 28 */
    m4_scst_mac_test2 = 0x00011B11,                               /* 29 */
    m4_scst_alu_test1 = 0x00002C09,                               /* 30 */
    m4_scst_alu_test2 = 0x0000ADEF,                               /* 31 */
    m4_scst_alu_test3 = 0x00003F51,                               /* 32 */
    m4_scst_alu_test4 = 0x00008C65,                               /* 33 */
    m4_scst_alu_test5 = 0x00006E5A,                               /* 34 */
    m4_scst_alu_test6 = 0x00002022,                               /* 35 */
    m4_scst_simdsat_test1 = 0x1C464F74,                           /* 36 */
    m4_scst_simdsat_test2 = 0x78C61FF0,                           /* 37 */
    m4_scst_simdsat_test3 = 0x24CA596B,                           /* 38 */
    m4_scst_simdsat_test4 = 0xC4527367,                           /* 39 */
    m4_scst_branch_test1 = 0x38A82153,                            /* 40 */
    m4_scst_branch_test2 = 0x53273023,                            /* 41 */
    m4_scst_status_test1 = 0x17A3FF6A,                            /* 42 */
    m4_scst_status_test2 = 0x75DB236C,                            /* 43 */
}e_M4_scst;

void SCST_SelfTest(void);

uint8_t GetScstSelfTestStatus(void);

#endif /* SAFTYCHECK_SAFETYLIB_SAFETY_SCST_H_ */
