/*
*********************************************************************************************************
*                                             uC/TCP-IP
*                                      The Embedded TCP/IP Suite
*
*                         (c) Copyright 2004-2015; Micrium, Inc.; Weston, FL
*
*                  All rights reserved.  Protected by international copyright laws.
*
*                  uC/TCP-IP is provided in source form to registered licensees ONLY.  It is
*                  illegal to distribute this source code to any third party unless you receive
*                  written permission by an authorized Micrium representative.  Knowledge of
*                  the source code may NOT be used to develop a similar product.
*
*                  Please help us continue to provide the Embedded community with the finest
*                  software available.  Your honesty is greatly appreciated.
*
*                  You can find our product's user manual, API reference, release notes and
*                  more information at: https://doc.micrium.com
*
*                  You can contact us at: http://www.micrium.com
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*
*                                          NETWORK MLDP LAYER
*                                (MULTICAST LISTENER DISCOVERY PROTOCOL)
*
* Filename      : net_mldp.h
* Version       : V3.03.02
* Programmer(s) : SL
*                 MM
*********************************************************************************************************
* Note(s)       : (1) Supports Neighbor Discovery Protocol as described in RFC #2710.
*
*                 (2) Only the MLDPv1 is supported. MLDPv2 as described in RFC #3810 is not yet
*                     supported.
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*********************************************************************************************************
*/

#include  "../../Source/net_cfg_net.h"
#include  "../../Source/net_type.h"
#include  "../../Source/net_tmr.h"
#include  "../../Source/net_buf.h"
#include  <lib_math.h>


/*
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) Network MLDP Layer module is required for applications that requires IPv6 services.
*
*               See also 'net_cfg.h  IP LAYER CONFIGURATION'.
*
*           (2) The following IP-module-present configuration value MUST be pre-#define'd in
*               'net_cfg_net.h' PRIOR to all other network modules that require IPv6 Layer
*               configuration (see 'net_cfg_net.h  IP LAYER CONFIGURATION') :
*
*                   NET_MLDP_MODULE_EN
*********************************************************************************************************
*/

#ifndef  NET_MLDP_MODULE_PRESENT
#define  NET_MLDP_MODULE_PRESENT

#ifdef   NET_MLDP_MODULE_EN                                /* See Note #2.                                         */


/*
*********************************************************************************************************
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                      MLDP MESSAGE SIZE DEFINES
*********************************************************************************************************
*/

#define  NET_MLDP_HDR_SIZE_DFLT                            8
#define  NET_MLDP_MSG_SIZE_MIN                            16
#define  NET_MLDP_MSG_SIZE_MAX                           NET_ICMPv6_MSG_LEN_MAX_NONE


/*
*********************************************************************************************************
*********************************************************************************************************
*                                             DATA TYPES
*********************************************************************************************************
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                    MLDP CACHE QUANTITY DATA TYPE
*
* Note(s) : (1) NET_MLDP_CACHE_NBR_MAX  SHOULD be #define'd based on 'NET_MLDP_HOST_GRP_QTY' data type
*               declared.
*********************************************************************************************************
*/

typedef  CPU_INT16U  NET_MLDP_HOST_GRP_QTY;

#define  NET_MLDP_HOST_GRP_NBR_MIN                         1
#define  NET_MLDP_HOST_GRP_NBR_MAX                       DEF_INT_16U_MAX_VAL


/*
*********************************************************************************************************
*                                   MLDP HOST GROUP STATE DATA TYPE
*
*                                         -------------------
*                                         |                 |
*                                         |                 |
*                                         |                 |
*                                         |                 |
*                            ------------>|      FREE       |<------------
*                            |            |                 |            |
*                            |            |                 |            |
*                            |            |                 |            |
*                            |            |                 |            |
*                            |            -------------------            | (1e) STOP LISTENING
*                            |                     |                     |
*                            | (1e) STOP LISTENING | (1a)START LISTENING |
*                            |                     |                     |
*                   -------------------            |            -------------------
*                   |                 |<------------            |                 |
*                   |                 |                         |                 |
*                   |                 |<------------------------|                 |
*                   |                 |  (1c) QUERY  RECEIVED   |                 |
*                   |    DELAYING     |                         |      IDLE       |
*                   |                 |------------------------>|                 |
*                   |                 |  (1b) REPORT RECEIVED   |                 |
*                   |                 |                         |                 |
*                   |                 |------------------------>|                 |
*                   -------------------  (1d) TIMER  EXPIRED    -------------------
*
*
* Note(s) : (1) See RFC #2710
*********************************************************************************************************
*/

typedef enum net_mldp_host_grp_state {
    NET_MLDP_HOST_GRP_STATE_NONE      =  0u,
    NET_MLDP_HOST_GRP_STATE_FREE      =  1u,
    NET_MLDP_HOST_GRP_STATE_DELAYING  =  2u,
    NET_MLDP_HOST_GRP_STATE_IDLE      =  3u,
} NET_MLDP_HOST_GRP_STATE;


/*
*********************************************************************************************************
*                                       MLDP HEADERS DATA TYPE
*
* Note(s) : (1) See RFC #2710 Section #3 for MLDP message header format.
*********************************************************************************************************
*/

                                                                        /* -------------- NET MLDP V1 HDR -------------- */
typedef  struct  net_mldp_v1_hdr {
    CPU_INT08U        Type;                                             /* MLDP msg type.                                */
    CPU_INT08U        Code;                                             /* MLDP msg code.                                */
    CPU_INT16U        ChkSum;                                           /* MLDP msg chk sum.                             */
    CPU_INT16U        MaxResponseDly;                                   /* MLDP max response dly.                        */
    CPU_INT16U        Reserved;                                         /* MLDP reserved bits.                           */
    NET_IPv6_ADDR     McastAddr;                                        /* MLDP mcast addr.                              */
} NET_MLDP_V1_HDR;


/*
*********************************************************************************************************
*                                IPv6 MULTICAST GROUP DATA TYPES
*
* Note(s) : (1) Structure holding the group membership information of a IPv6 multicast address
*
*********************************************************************************************************
*/

typedef  struct  net_mldp_host_grp  NET_MLDP_HOST_GRP;

struct net_mldp_host_grp {
    NET_MLDP_HOST_GRP        *PrevListPtr;                      /* Ptr to PREV MLDP host grp.                           */
    NET_MLDP_HOST_GRP        *NextListPtr;                      /* Ptr to NEXT MLDP host grp.                           */

    NET_MLDP_HOST_GRP        *PrevIF_ListPtr;                   /* Ptr to PREV MLDP host grp of same IF.                */
    NET_MLDP_HOST_GRP        *NextIF_ListPtr;                   /* Ptr to NEXT MLDP host grp of same IF.                */

    NET_TMR                  *TmrPtr;                           /* Pointer to MDLP delay timer.                         */
    CPU_INT32U                Delay_ms;                         /* Delay value.                                         */

    NET_IF_NBR                IF_Nbr;                           /* IF nbr attached to the MDLP group.                   */
    NET_IPv6_ADDR             AddrGrp;                          /* Multicast address of the group.                      */

    NET_MLDP_HOST_GRP_STATE   State;                            /* MLDP   host grp state.                               */
    CPU_INT16U                RefCtr;                           /* MLDP   host grp ref ctr.                             */
    CPU_INT16U                Flags;                            /* MLDP   host grp flags.                               */
};


/*
*********************************************************************************************************
*********************************************************************************************************
*                                          GLOBAL VARIABLES
*********************************************************************************************************
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*********************************************************************************************************
*                                               MACRO'S
*********************************************************************************************************
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             PUBLIC API
*********************************************************************************************************
*/

CPU_BOOLEAN         NetMLDP_HostGrpJoin         (NET_IF_NBR         if_nbr,
                                                 NET_IPv6_ADDR     *p_addr,
                                                 NET_ERR           *p_err);

CPU_BOOLEAN         NetMLDP_HostGrpLeave        (NET_IF_NBR         if_nbr,
                                                 NET_IPv6_ADDR     *p_addr,
                                                 NET_ERR           *p_err);


/*
*********************************************************************************************************
*                                         INTERNAL FUNCTIONS
*********************************************************************************************************
*/

void                NetMLDP_Init                (void);

NET_MLDP_HOST_GRP  *NetMLDP_HostGrpJoinHandler  (      NET_IF_NBR         if_nbr,
                                                 const NET_IPv6_ADDR     *p_addr,
                                                       NET_ERR           *p_err);

CPU_BOOLEAN         NetMLDP_HostGrpLeaveHandler (      NET_IF_NBR         if_nbr,
                                                 const NET_IPv6_ADDR     *p_addr,
                                                       NET_ERR           *p_err);

CPU_BOOLEAN         NetMLDP_IsGrpJoinedOnIF     (      NET_IF_NBR         if_nbr,
                                                 const NET_IPv6_ADDR     *p_addr_grp);

void                NetMLDP_Rx                  (      NET_BUF           *p_buf,
                                                       NET_BUF_HDR       *p_buf_hdr,
                                                       NET_MLDP_V1_HDR   *p_ndp_hdr,
                                                       NET_ERR           *p_err);


/*
*********************************************************************************************************
*********************************************************************************************************
*                                        CONFIGURATION ERRORS
*********************************************************************************************************
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif  /* NET_MLDP_MODULE_EN       */
#endif  /* NET_MLDP_MODULE_PRESENT  */

