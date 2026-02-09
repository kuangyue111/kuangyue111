/*
********************************************************************************
*                                         TI TMS470R1A288
*                                   TMS470R1A288 NGMC Bootloader
*
*                                   (c) Copyright 2007, ENPG
*                                        All Rights Reserved
*
*
* File : HTTP_DOWNLOAD.H
* By   : Albert You
********************************************************************************
*/

#ifndef _HTTP_DOWNLOAD_H
#define _HTTP_DOWNLOAD_H

/*
 * Return value of the buffering functions that indicates that a
 * buffer was not filled by incoming data.
 *
 */
#define BUF_NOT_FULL 0
#define BUF_NOT_FOUND 0

/*
 * Return value of the buffering functions that indicates that a
 * buffer was completely filled by incoming data.
 *
 */
#define BUF_FULL 1

/*
 * Return value of the buffering functions that indicates that an
 * end-marker byte was found.
 *
 */
#define BUF_FOUND 2

#define BUF_FOUND_STR 8

#define MAX_BOUNDARY_LENGTH 42


u8_t strbuffer(register struct psock *psock, const char *needle);

#endif  /* _HTTP_DOWNLOAD_H */

/*******************(C)COPYRIGHT 2006 ENPG *****END OF FILE****/
