/******************************************************************************
*
* Freescale Confidential Proprietary
* Freescale Semiconductor Inc.
* (c) Copyright 2004-2016 Freescale Semiconductor, Inc.
* (c) Copyright 2001-2004 Motorola, Inc.
* ALL RIGHTS RESERVED.
*
******************************************************************************/
#ifndef M4_SCST_TEST_DATA_H
#define M4_SCST_TEST_DATA_H


/******************************************************************************
* Includes / Defines
******************************************************************************/
#include "m4_scst_typedefs.h"
  
/******************************************************************************
* Types
******************************************************************************/
  
/* The m4_scst_atomic_test_icst_DESC_t data type specifies a descriptor for individual atomic test. */
typedef struct {
    m4_scst_uint32_t (* atomic_test_start_address) (void);
    m4_scst_uint32_t expected_signature;
} m4_scst_atomic_test_icst_DESC_t;


#endif /* M4_SCST_TEST_DATA_H */
