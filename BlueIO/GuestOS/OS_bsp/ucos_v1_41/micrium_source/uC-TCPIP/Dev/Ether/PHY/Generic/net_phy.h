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
*                                   NETWORK PHYSICAL LAYER DRIVER
*
*                                        GENERIC ETHERNET PHY
*
* Filename      : net_phy.h
* Version       : V3.03.02.00
* Programmer(s) : EHS
*                 HMS
*********************************************************************************************************
* Note(s)       : (1) Assumes uC/TCP-IP V2.02 (or more recent version) is included in the project build.
*
*                 (2) The (R)MII interface port is assumed to be part of the host EMAC.  Therefore, (R)MII
*                     reads/writes MUST be performed through the network device API interface via calls to
*                     function pointers 'Phy_RegRd()' & 'Phy_RegWr()'.
*
*                 (3) Interrupt support is Phy specific, therefore the generic Phy driver does NOT support
*                     interrupts.  However, interrupt support is easily added to the generic Phy driver &
*                     thus the ISR handler has been prototyped and & populated within the function pointer
*                     structure for example purposes.
*
*                 (4) Does NOT support 1000Mbps Phy.
*********************************************************************************************************
*/

#include  <IF/net_if.h>
#include  <IF/net_if_ether.h>

/*
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This network physical layer header file is protected from multiple pre-processor inclusion
*               through use of the network physical layer module present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef  NET_PHY_MODULE_PRESENT                                 /* See Note #1.                                         */
#define  NET_PHY_MODULE_PRESENT
#ifdef   NET_IF_ETHER_MODULE_EN

/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                 NETWORK PHYSICAL LAYER ERROR CODES
*
* Note(s) : (1) ALL physical layer-independent error codes #define'd in      'net_err.h';
*               ALL physical layer-specific    error codes #define'd in this 'net_phy_&&&.h'.
*
*           (2) Network error code '12,000' series reserved for network physical layer drivers.
*               See 'net_err.h  NETWORK PHYSICAL LAYER ERROR CODES' to ensure that physical layer-
*               specific error codes do NOT conflict with physical layer-independent error codes.
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          GLOBAL VARIABLES
*********************************************************************************************************
*/

extern  const  NET_PHY_API_ETHER  NetPhy_API_Generic;


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                        CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif  /* NET_IF_ETHER_MODULE_EN */
#endif  /* NET_PHY_MODULE_PRESENT */

