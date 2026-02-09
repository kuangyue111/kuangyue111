/**
 * Copyright (c) 2001-2005, Adam Dunkels.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * This file is part of the uIP TCP/IP stack.
 *
 * $Id: httpd.h,v 1.2 2006/06/11 21:46:38 adam Exp $
 *
 */
 /*
 *****************************************************************************
 *                                                                           *
 *  Filename:   httpd.h                                              *
 *  Date:       May 1, 2008    
 *  by caili
 *  Description:                                                             *
 *  Version:    V1.00                                                        *
 *  Hardware:                                                                *
 *  Remarks:    For NGC application                                          *
 *                                                                           *
 *****************************************************************************/
#ifndef __HTTPD_H__
#define __HTTPD_H__
//#include "main.h"

#include "psock.h"
#include "httpd-fs.h"

#define WEB_SERVER_PORT  80

#define NONE_DOWNLOAD  0

#define DOWNLOAD_SUCCESS  1
#define DOWNLOAD_FAIL    2
#define UNATHURIZED_DOWNLOAD   3


#define SETTING_DOWNLOAD_FLAG  0
#define CLEAR_DOWNLAOD_FLAG  1


#define NONE_SETTING 0
#define SET_DATA_SUCCESS 1
#define SET_DATA_FAIL  2
#define SET_NOT_RIGHT  3


#define  SETTING_FAIL   0
#define SETTING_SUCCESS   1

#define NOT_PASS_AUTHORIZTON 0
#define PASS_AUTHORIZTON  1
#define ADMIN_USER   2
#define OPERATOR_USER  3



#define  HTTP_SATAE_INPUTBUF_SIZE  80
struct httpd_state {
  unsigned char timer;
  struct psock sin, sout;
  struct pt outputpt, scriptpt;
  char inputbuf[HTTP_SATAE_INPUTBUF_SIZE];
  char filename[20];
  char state;
  struct httpd_fs_file file;
  int len;
  char *scriptptr;
  int scriptlen;
  int uploadflag;
  int passflag;
 

  unsigned short count;
};

struct HTTP_MUL_PART
{
  unsigned char rcv_nxt[4];// The sequence number that we expect to receive next
  unsigned short rec_count_num;// packets counter
  unsigned char multipart_state;// multipart process state
  unsigned char post_state;// Post packet state
  unsigned int rec_bytes; // receiving the download file data numbers
  int multipart_bytes; // the total number of receiving data in post packet
  unsigned short rec_head; // head length in first multipart data packet
  unsigned short rec_tail; // tail length in last multipart data packet
};
//static unsigned char	HexToAscii1(unsigned char Input);
void  set_ip(void);
void httpd_init(void);
void httpd_appcall(void);
//void make_html_file();
void httpd_log(char *msg);
void httpd_log_file(u16_t *requester, char *file);


typedef struct 
{
	unsigned char admin[10];
	unsigned char password1[10];
}password;


#endif /* __HTTPD_H__ */
