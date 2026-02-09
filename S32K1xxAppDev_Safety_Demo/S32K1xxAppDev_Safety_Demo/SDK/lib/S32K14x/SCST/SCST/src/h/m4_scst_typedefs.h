/******************************************************************************
*
* Freescale Confidential Proprietary
* Freescale Semiconductor Inc.
* (c) Copyright 2004-2016 Freescale Semiconductor, Inc.
* (c) Copyright 2001-2004 Motorola, Inc.
* ALL RIGHTS RESERVED.
*
******************************************************************************/
#ifndef M4_SCST_TYPEDEFS_H
#define M4_SCST_TYPEDEFS_H


    typedef signed char m4_scst_int8_t;
    typedef unsigned char m4_scst_uint8_t;
    typedef volatile signed char m4_scst_vint8_t;
    typedef volatile unsigned char m4_scst_vuint8_t;

    typedef signed short m4_scst_int16_t;
    typedef unsigned short m4_scst_uint16_t;
    typedef volatile signed short m4_scst_vint16_t;
    typedef volatile unsigned short m4_scst_vuint16_t;

    typedef signed int m4_scst_int32_t;
    typedef unsigned int m4_scst_uint32_t;
    typedef volatile signed int m4_scst_vint32_t;
    typedef volatile unsigned int m4_scst_vuint32_t;
    
    #ifndef TRUE                /* conditional check */
        #define TRUE      1
    #endif
    
    #ifndef FALSE               /* conditional check */
        #define FALSE     0
    #endif

#endif    /* _M4_SCST_TYPEDEFS_H */


