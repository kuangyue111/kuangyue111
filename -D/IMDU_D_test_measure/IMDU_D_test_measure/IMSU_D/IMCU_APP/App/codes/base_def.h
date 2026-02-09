
/*=============================================================================*
 *         Copyright(c) INFYPOWER Co., Ltd.
 *         ALL RIGHTS RESERVED
 *============================================================================*/

#ifndef __BASE_TYPE_H_
#define __BASE_TYPE_H_

/*0: MOTOROLA mode(big-endian mode) */
/*1: INTEL mode(little-endian mode) */

#define __BIG_ENDIAN			0
#define __LITTLE_ENDIAN			1

#if (__LITTLE_ENDIAN__)		// =1: IAR for little endian mode
#define __CPU_ENDIAN_MODE		__LITTLE_ENDIAN
#else						// =0: big endian mode
#define __CPU_ENDIAN_MODE		__BIG_ENDIAN
#endif


#ifdef __cplusplus
#define __EXTERN_C_BEGIN extern "C" {
#define __EXTERN_C_END   }
#else
#define __EXTERN_C_BEGIN
#define __EXTERN_C_END
#endif


/*===== 0bit base types  ====================*/
#define VOID                void

/*===== 8bit base types  ====================*/
typedef   signed char       CHAR;
typedef unsigned char       UCHAR;
typedef          UCHAR      BYTE;


/*===== 16bit base types  ===================*/
typedef   signed short      SHORT;
typedef unsigned short      USHORT;
typedef          USHORT     WORD;


/*===== 32bit base types  ===================*/
typedef unsigned long       DWORD;
typedef unsigned long       *HANDLE;


/*===== float base types  ===================*/
typedef          float      FLOAT32;
typedef         double      FLOAT64;



/*===== integer base types  =============*/
typedef          CHAR       INT8;
typedef          UCHAR      UINT8;
typedef          SHORT      INT16;
typedef          USHORT     UINT16;
typedef signed long         INT32;
typedef unsigned long       UINT32;

/*===== Boolean types  ======================*/
//typedef         INT32		BOOL;
//#define         FALSE       0
//#define         TRUE        1

/*===== Data storage flags ===================*/
#define EXTERN		extern		// import global data/function
#define GLOBAL					// export/define global data/function
#define LOCAL		static		// static
#define VOLATILE	volatile
#define INLINE		inline		// inline api		
#define NOINIT		__no_init

#define CONST		const
#define IN						// input arg
#define OUT						// output arg


/*===== function unused arg====================*/
#define UNUSED(x)	((x)=(x))

/*===== misc macros       ====================*/
#define itemof(array_prt)	(sizeof((array_prt))/sizeof((array_prt)[0]))


/************************************************************************/
/* WORD/DWORD swap, split and merge                                     */
/************************************************************************/
#define SWAP_WORD_BYTE(w)			\
	( ((((WORD)(w))>>8) & (WORD)0x00ff) + \
	  ((((WORD)(w))<<8) & (WORD)0xff00) )

#define SWAP_DWORD_BYTE(dw)		\
	( ((((DWORD)(dw))>>24) & (DWORD)0x000000ff) + \
	  ((((DWORD)(dw))<<24) & (DWORD)0xff000000) + \
	  ((((DWORD)(dw))>> 8) & (DWORD)0x0000ff00) + \
	  ((((DWORD)(dw))<< 8) & (DWORD)0x00ff0000) )

#define MK_WORD(Lo, Hi)  ((WORD)(((BYTE)((DWORD)(Lo)  & 0xff))   | (((WORD)((BYTE)((DWORD)(Hi)  & 0xff)))   << 8)))
#define MK_DWORD(Lo, Hi) ((DWORD)(((WORD)((DWORD)(Lo) & 0xffff)) | (((DWORD)((WORD)((DWORD)(Hi) & 0xffff))) << 16)))
#define LO_WORD(l)       ((WORD)((DWORD)(l)  & 0xffff))
#define HI_WORD(l)       ((WORD)((DWORD)(l) >> 16))
#define LO_BYTE(w)       ((BYTE)((DWORD)(w)  & 0xff))
#define HI_BYTE(w)       ((BYTE)((DWORD)(w) >> 8))
#define LOBYTE(w)       ((BYTE)((DWORD)(w)  & 0xff))
#define HIBYTE(w)       ((BYTE)((DWORD)(w) >> 8))


#endif