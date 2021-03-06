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
*                                       NETWORK CRYPTO SHA1 UTILITY
*
* Filename      : net_sha1.h
* Version       : V3.03.02
* Programmer(s) : AL
*********************************************************************************************************
* Note(s)       : (1) This is the header file for code which implements the Secure Hashing Algorithm 1 as
*                     defined in FIPS PUB 180-1 published April 17, 1995.
*
*                     Many of the variable names in this code, especially the single character names, were
*                     used because those were the names used in the publication.
*
*                     Please read the file net_sha1.c for more information.
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*********************************************************************************************************
*/

#include  <Source/net.h>
#include  <lib_mem.h>


/*
*********************************************************************************************************
*********************************************************************************************************
*                                               MODULE
*********************************************************************************************************
*********************************************************************************************************
*/


#ifndef  NET_SHA1_MODULE_PRESENT
#define  NET_SHA1_MODULE_PRESENT


/*
*********************************************************************************************************
*********************************************************************************************************
*                                                DEFINES
*********************************************************************************************************
*********************************************************************************************************
*/

#define  NET_SHA1_HASH_SIZE                 20

#define  NET_SHA1_INTERMEDIATE_HASH_SIZE    NET_SHA1_HASH_SIZE / 4


/*
*********************************************************************************************************
*********************************************************************************************************
*                                            ENUMERATIONS
*********************************************************************************************************
*********************************************************************************************************
*/

typedef enum
{
    NET_SHA1_ERR_NONE                 = 0,
    NET_SHA1_ERR_PTR_NULL             = 1,
    NET_SHA1_ERR_INPUT_TOO_LONG       = 2,
    NET_SHA1_ERR_STATE_ERROR          = 3,
    NET_SHA1_ERR_CORRUPTION           = 4,

}NET_SHA1_ERR;


/*
*********************************************************************************************************
*********************************************************************************************************
*                                              DATA TYPES
*********************************************************************************************************
*********************************************************************************************************
*/

typedef struct net_sha1_context{

    CPU_INT32U      Intermediate_Hash[NET_SHA1_INTERMEDIATE_HASH_SIZE];

    CPU_INT32U      Length_Low;                                 /* Message length in bits                               */
    CPU_INT32U      Length_High;                                /* Message length in bits                               */

                                                                /* Index into message block array                       */
    CPU_INT16U      Message_Block_Index;
    CPU_INT08U      Message_Block[64];                          /* 512-bit message blocks                               */

    CPU_BOOLEAN     Computed;                                   /* Is the digest computed?                              */
    NET_SHA1_ERR    Corrupted;                                  /* Is the message digest corrupted?                     */

} NET_SHA1_CTX;




/*
*********************************************************************************************************
*********************************************************************************************************
*                                          FUNCTION PROTOTYPES
*********************************************************************************************************
*********************************************************************************************************
*/


CPU_BOOLEAN NetSHA1_Reset     (      NET_SHA1_CTX  *p_ctx,
                                     NET_SHA1_ERR  *p_err);

CPU_BOOLEAN NetSHA1_Input     (      NET_SHA1_CTX  *p_ctx,
                               const CPU_CHAR      *p_msg,
                                     CPU_INT32U     len,
                                     NET_SHA1_ERR  *p_err);

CPU_BOOLEAN NetSHA1_Result    (      NET_SHA1_CTX  *p_ctx,
                                     CPU_CHAR      *p_msg_digest,
                                     NET_SHA1_ERR  *p_err);


/*
*********************************************************************************************************
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*********************************************************************************************************
*/

#endif  /* NET_SHA1_MODULE_PRESENT */
