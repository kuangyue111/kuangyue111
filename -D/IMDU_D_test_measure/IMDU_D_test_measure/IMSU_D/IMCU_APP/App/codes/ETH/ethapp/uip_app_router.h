
#ifndef __UIP_APP_ROUTER_H__
#define __UIP_APP_ROUTER_H__

#include "..\codes\ETH\ethapp\webserver\httpd.h"

#include "base_def.h"

#define SNMP_SERVER_PORT  161
#define EEM_SERVER_PORT  5050
//#define ZHIXIN_CLIENT_PORT  9444
//#define ZHIXIN_CLIENT_PORT  8056
#define ZHIXIN_CLIENT_PORT gEvChargeInfo.u32NET_PORT
typedef struct httpd_state uip_tcp_appstate_t;

typedef unsigned char uip_udp_appstate_t;

#ifdef   UIP_APPCALL                                                    /* If UIP_APPCALL is already defined from another uIP App   */    
#undef   UIP_APPCALL                                                    /* then undefine it and force uIP apps to register          */
#endif

#ifndef UIP_APPCALL
#define UIP_APPCALL     uip_router_tcp_appcall
#endif

#ifdef   UIP_UDP_APPCALL                                                /* If UIP_UDP_APPCALL is already defined from an uIP App    */    
#undef   UIP_UDP_APPCALL                                                /* then undefine it and redefine it as specified below      */
#endif

#ifndef UIP_UDP_APPCALL
#define UIP_UDP_APPCALL     uip_router_udp_appcall
#endif

/* Finally we define the application function to be called by uIP. */
void uip_router_tcp_appcall(void);
void uip_router_udp_appcall(void);

void uip_router_init(void);


#define   FROM_EEM_PORT  1
#define   NOT_FROM_EEM_PORT    0

#define  GET_EEM_OUTPUT_DATA_SUCCESS  1
#define  GET_EEM_OUTPUT_DATA_FAIL    0


#define make_trap_success   1
#define make_trap_failure  0

 void snmp(void);
int  snmp_trap(void);

void eem_server_init(void);
void Net_Client_appcall(void);

#endif 
