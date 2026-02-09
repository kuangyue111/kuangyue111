/*
********************************************************************************
*                                         TI TMS470R1A288
*                                   TMS470R1A288 NGMC Bootloader
*
*                                   (c) Copyright 2007, ENPG
*                                        All Rights Reserved
*
*
* File : HTTP_DOWNLOAD.C
* By   : Albert You
********************************************************************************
*/

#include "main.h"
#include "psock.h"
#include "http_download.h"
#define STATE_NONE 0
#define STATE_ACKED 1
#define STATE_READ 2

/*******************************************************************************
* Function Name  : strbuffer
* Description    : This function is the standard string search. The Keil library
*                  does not provide it.  It looks for one string in another string
*                  and returns a pointer to it if found, otherwise returns NULL.
* Input          : psock - psock *psock
*                  string - char* needle
* Output         : None
* Return         : position of string - char*
*******************************************************************************/
u8_t strbuffer(register struct psock *psock, const char *needle)
{
    char *ptr1;
    char *ptr2;
    char *haystack;
//    int i, t1, t2;

    // Protect against NULL pointer
    if (*needle == 0)
    {
        return BUF_NOT_FOUND;
    }
#if 0
////////////////////////////////////////////////////////////////////////////////
    psock->buf.ptr = (u8_t *)psock->bufptr;
    psock->buf.left = psock->bufsize;

    if(psock->readlen == 0)
    {
        if(!(psock_newdata(psock)))
        {
            return PT_WAITING;
        }
        psock->state = STATE_READ;
        psock->readptr = (u8_t *)uip_appdata;
        psock->readlen = uip_datalen();
    }
////////////////////////////////////////////////////////////////////////////////
#endif

    haystack = (char *)psock->readptr;

    for(; (psock->readlen > 0); haystack++)
    {
//        i++;
        // Look for needle in haystack.  If there is a
        // match then this will continue all the way
        // until ptr1 reaches the NULL at the end of needle
	for(ptr1 = (char *)needle, ptr2 = haystack; *ptr1 && (*ptr1 == *ptr2); ptr1++, ptr2++);
						
        // If there is a match then return pointer to needle in haystack
        if(*ptr1 == 0)
        {
          return(BUF_FOUND_STR);
        }
/*
        if (i == 250)
        {
          i = 0;
        }
*/
        (psock->readlen)--;
//        t1 = psock->readlen;
        (psock->readptr)++;
//        t2 = (int)psock->readptr;
    }
#if 0
////////////////////////////////////////////////////////////////////////////////
    psock->buf.left = uip_datalen() - psock->readlen;
    psock->buf.ptr = psock->readptr;
    if(psock_datalen(psock) == 0)
    {
        psock->state = STATE_NONE;
        PT_RESTART(&psock->psockpt);
    }
////////////////////////////////////////////////////////////////////////////////
#endif

    return (BUF_NOT_FOUND); // no matching string found
}


/* get the boundary value when POST multipart/form-data.
 Netscape:
Content-type: multipart/form-data; boundary=---------------------------78292114016353
Content-type: multipart/form-data; boundary=---------------------------9217206358374
                                            -----------------------------9217206358374
 IE:
Content-Type: multipart/form-data; boundary=---------------------------7d038e12e70
Content-Type: multipart/form-data; boundary=---------------------------7d038e12e70
                                            -----------------------------7d038e12e70
Content-Type: multipart/form-data; boundary=---------------------------7d038426e70
                                            -----------------------------7d038426e70
*/
//static int GetMultipartBoundary(char *pHead, char *pszBoundary )
//{
//    static char szBoundaryFlagNS[] = "Content-type: multipart/form-data; boundary=";
//    static char szBoundaryFlagIE[] = "Content-Type: multipart/form-data; boundary=";
//
//    char *p;
//    int  nLenBound = 0;
//
//    p = strstr( pHead, szBoundaryFlagIE );
//    if( p == NULL )
//    {
//        p = strstr( pHead, szBoundaryFlagNS );
//    }
//    if( p != NULL )
//    {
//        char    *pCRLF;
//
//        p       += sizeof(szBoundaryFlagIE) - 1;/* È¥µô '\0' */
//        pCRLF   = strstr( p, "\r\n" );
//         /* not found or too long, maybe error, return NULL */
//        if( pCRLF != NULL)
//        {
//            nLenBound = pCRLF - p;
//            if( (nLenBound < (MAX_BOUNDARY_LENGTH - 4)) && (nLenBound > 0) )
//            {
//                pszBoundary[0] = '-';
//                pszBoundary[1] = '-';
//
//                nLenBound += 2; /* including \r\n */
//                memmove( pszBoundary+2, p, nLenBound );
//
//                nLenBound += 2; /* add length of the first '--' */
//                pszBoundary[nLenBound] = 0;
//            }
//        }
//    }
//    else
//        *pszBoundary = 0;
//
//    return nLenBound;
//}


/*******************(C)COPYRIGHT 2006 ENPG *****END OF FILE****/
