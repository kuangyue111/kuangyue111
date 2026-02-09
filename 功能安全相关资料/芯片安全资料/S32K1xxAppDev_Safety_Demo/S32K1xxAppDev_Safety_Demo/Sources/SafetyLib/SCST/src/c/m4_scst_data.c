/******************************************************************************
*
* Freescale Confidential Proprietary
* Freescale Semiconductor Inc.
* (c) Copyright 2004-2016 Freescale Semiconductor, Inc.
* (c) Copyright 2001-2004 Motorola, Inc.
* ALL RIGHTS RESERVED.
*
******************************************************************************/


/******************************************************************************
* Includes
******************************************************************************/
#include "m4_scst_typedefs.h"
#include "m4_scst_configuration.h"
#include "m4_scst_data.h"
#include "m4_scst_compiler.h"

/*****************************************************************************/
extern m4_scst_uint32_t m4_scst_exception_test_svc(void);
extern m4_scst_uint32_t m4_scst_exception_test_pendsv(void);
extern m4_scst_uint32_t m4_scst_exception_test_systick(void);
extern m4_scst_uint32_t m4_scst_exception_usage_fault(void);
extern m4_scst_uint32_t m4_scst_exception_hard_fault1(void);
extern m4_scst_uint32_t m4_scst_exception_hard_fault2(void);
extern m4_scst_uint32_t m4_scst_exception_mem_fault(void);
extern m4_scst_uint32_t m4_scst_exception_bus_fault(void);
extern m4_scst_uint32_t m4_scst_exception_test_nmihf(void);
extern m4_scst_uint32_t m4_scst_exception_test_tail_chain(void);
extern m4_scst_uint32_t m4_scst_exception_test_masking(void);
extern m4_scst_uint32_t m4_scst_exception_test_handler_thread(void);
extern m4_scst_uint32_t m4_scst_loadstore_test1(void);
extern m4_scst_uint32_t m4_scst_loadstore_test2(void);
extern m4_scst_uint32_t m4_scst_loadstore_test3(void);
extern m4_scst_uint32_t m4_scst_loadstore_test4(void);
extern m4_scst_uint32_t m4_scst_loadstore_test5(void);
extern m4_scst_uint32_t m4_scst_loadstore_test6(void);
extern m4_scst_uint32_t m4_scst_regbank_test1(void);
extern m4_scst_uint32_t m4_scst_regbank_test2(void);
extern m4_scst_uint32_t m4_scst_regbank_test3(void);
extern m4_scst_uint32_t m4_scst_regbank_test4(void);
extern m4_scst_uint32_t m4_scst_regbank_test5(void);
extern m4_scst_uint32_t m4_scst_regbank_test6(void);
extern m4_scst_uint32_t m4_scst_mac_test1(void);
extern m4_scst_uint32_t m4_scst_mac_test2(void);
extern m4_scst_uint32_t m4_scst_alu_test1(void);
extern m4_scst_uint32_t m4_scst_alu_test2(void);
extern m4_scst_uint32_t m4_scst_alu_test3(void);
extern m4_scst_uint32_t m4_scst_alu_test4(void);
extern m4_scst_uint32_t m4_scst_alu_test5(void);
extern m4_scst_uint32_t m4_scst_alu_test6(void);
extern m4_scst_uint32_t m4_scst_alu_test7(void);
extern m4_scst_uint32_t m4_scst_simdsat_test1(void);
extern m4_scst_uint32_t m4_scst_simdsat_test2(void);
extern m4_scst_uint32_t m4_scst_simdsat_test3(void);
extern m4_scst_uint32_t m4_scst_simdsat_test4(void);
extern m4_scst_uint32_t m4_scst_branch_test1(void);
extern m4_scst_uint32_t m4_scst_branch_test2(void);
extern m4_scst_uint32_t m4_scst_branch_test3(void);
extern m4_scst_uint32_t m4_scst_fetch_test(void);
extern m4_scst_uint32_t m4_scst_status_test1(void);
extern m4_scst_uint32_t m4_scst_status_test2(void);
extern m4_scst_uint32_t m4_scst_status_test3(void);


SCST_PLACE_IN_SECTION(m4_scst_rom_data)

const m4_scst_atomic_test_icst_DESC_t m4_scst_atomic_test_desc[M4_SCST_TESTS_NUM] = {
    {&m4_scst_exception_test_svc,               0x00003720U},       /* 0 */
    {&m4_scst_exception_test_pendsv,            0x00001284U},       /* 1 */
    {&m4_scst_exception_test_systick,           0x00000B3EU},       /* 2 */
    {&m4_scst_exception_hard_fault1,            0x0000169BU},       /* 3 */
    {&m4_scst_exception_hard_fault2,            0x00002EBBU},       /* 4 */
    {&m4_scst_exception_usage_fault,            0x000060EEU},       /* 5 */
    {&m4_scst_exception_mem_fault,              0x0000144DU},       /* 6 */
    {&m4_scst_exception_bus_fault,              0x00001025U},       /* 7 */
    {&m4_scst_exception_test_nmihf,             0x00006FB4U},       /* 8 */
    {&m4_scst_exception_test_tail_chain,        0x0000688CU},       /* 9 */
    {&m4_scst_exception_test_masking,           0x00007C98U},       /* 10 */
    {&m4_scst_exception_test_handler_thread,    0x00005071U},       /* 11 */
    {&m4_scst_regbank_test4,                    0x0000C86DU},       /* 12 */
    {&m4_scst_alu_test7,                        0x00002463U},       /* 13 */
    {&m4_scst_branch_test3,                     0x826FB2BDU},       /* 14 */
    {&m4_scst_status_test3,                     0x00004E12U},       /* 15 */
    {&m4_scst_regbank_test6,                    0xCB2C63BEU},       /* 16 */
    {&m4_scst_fetch_test,                       0xD3D74E16U},       /* 17 */
    {&m4_scst_loadstore_test6,                  0x42B00E33U},       /* 18 */
    {&m4_scst_loadstore_test1,                  0x4A440CD5U},       /* 19 */
    {&m4_scst_loadstore_test2,                  0xF2272684U},       /* 20 */
    {&m4_scst_loadstore_test3,                  0xEC808882U},       /* 21 */
    {&m4_scst_loadstore_test4,                  0xA2869232U},       /* 22 */
    {&m4_scst_loadstore_test5,                  0x1A2E219DU},       /* 23 */
    {&m4_scst_regbank_test1,                    0x25E60862U},       /* 24 */
    {&m4_scst_regbank_test2,                    0x797BF153U},       /* 25 */
    {&m4_scst_regbank_test3,                    0x496BE477U},       /* 26 */
    {&m4_scst_regbank_test5,                    0x00002F49U},       /* 27 */
    {&m4_scst_mac_test1,                        0x0001F878U},       /* 28 */
    {&m4_scst_mac_test2,                        0x00032D33U},       /* 29 */
    {&m4_scst_alu_test1,                        0x0000741BU},       /* 30 */
    {&m4_scst_alu_test2,                        0x0001F631U},       /* 31 */
    {&m4_scst_alu_test3,                        0x000041F3U},       /* 32 */
    {&m4_scst_alu_test4,                        0x000194AFU},       /* 33 */
    {&m4_scst_alu_test5,                        0x0000B2EEU},       /* 34 */
    {&m4_scst_alu_test6,                        0x00006066U},       /* 35 */
    {&m4_scst_simdsat_test1,                    0x24CAD19CU},       /* 36 */
    {&m4_scst_simdsat_test2,                    0x894A2010U},       /* 37 */
    {&m4_scst_simdsat_test3,                    0x6D5EEBBDU},       /* 38 */
    {&m4_scst_simdsat_test4,                    0x4CF695A8U},       /* 39 */
    {&m4_scst_branch_test1,                     0x49F863F5U},       /* 40 */
    {&m4_scst_branch_test2,                     0xF5695065U},       /* 41 */
    {&m4_scst_status_test1,                     0x38E401BEU},       /* 42 */
    {&m4_scst_status_test2,                     0x9E6D65B4U}        /* 43 */
};

/*****************************************************************************/
