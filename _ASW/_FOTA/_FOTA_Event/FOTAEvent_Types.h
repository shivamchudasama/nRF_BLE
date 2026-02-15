/**
 * @file          FOTAEvent_Types.h
 * @brief         Header file containing type definitions of FOTA Event.
 * @date          15/02/2026
 * @author        Shivam Chudasama [SC]
 * @copyright     Bajaj Auto Technology Limited (BATL)
 */

#ifndef _FOTA_EVENT_TYPES_H
#define _FOTA_EVENT_TYPES_H

/******************************************************************************/
/*                                                                            */
/*                                  INCLUDES                                  */
/*                                                                            */
/******************************************************************************/
#include <stdint.h>

/******************************************************************************/
/*                                                                            */
/*                                  DEFINES                                   */
/*                                                                            */
/******************************************************************************/
/**
 * @def           <Define name>
 * @brief         <Define details>.
 */

/******************************************************************************/
/*                                                                            */
/*                                   ENUMS                                    */
/*                                                                            */
/******************************************************************************/
/**
 * @enum          FOTAEvent_E
 * @brief         Enumeration of different FOTA Events.
 */
typedef enum
{
   eFE_FOTA_START = 0x01,                    /**< FOTA start packet */
   eFE_METADATA,                             /**< Metadata packet */
   eFE_MANIFEST,                             /**< Manifest packet */
   eFE_FOTA_DATA,                            /**< FOTA data packet */
   // More FOTA Events can be added here as needed
} FOTAEvent_E;

/******************************************************************************/
/*                                                                            */
/*                                 STRUCTURES                                 */
/*                                                                            */
/******************************************************************************/
/**
 * @struct        FOTAStart_T
 * @brief         Structure containing FOTA Start Event details.
 */
typedef struct
{
   uint32_t u32_FOTAStartSignal;             /**< FOTA Start Signal */
} FOTAStart_T;

/**
 * @struct        Metadata_T
 * @brief         Structure containing FOTA Metadata details.
 */
typedef struct
{
   // As of now, this is written as an array of 128 bytes, but it can be modified
   // as per the actual metadata details to be stored.
   uint8_t u8ar_metadataPkt[128];            /**< Metadata packet */
} Metadata_T;

/**
 * @struct        Manifest_T
 * @brief         Structure containing FOTA Manifest details.
 */
typedef struct
{
   // As of now, this is written as an array of 128 bytes, but it can be modified
   // as per the actual manifest details to be stored.
   uint8_t u8ar_manifestPkt[128];            /**< Manifest packet */
} Manifest_T;

/**
 * @struct        FOTAData_T
 * @brief         Structure containing FOTA Data details.
 */
typedef struct
{
   // As of now, this is written as an array of 128 bytes, but it can be modified
   // as per the actual data details to be stored.
   uint8_t u8ar_dataPkt[128];                /**< FOTA Data packet */
} FOTAData_T;

/**
 * @struct        FOTAAbort_T
 * @brief         Structure containing FOTA Abort details.
 */
typedef struct
{
   // As of now, this is written as an array of 128 bytes, but it can be modified
   // as per the actual abort details to be stored.
   uint8_t u8ar_abortPkt[128];               /**< FOTA Abort packet */
} FOTAAbort_T;

/**
 * @struct        FOTAEvent_T
 * @brief         Structure containing FOTA Event details.
 */
typedef struct
{
   FOTAEvent_E e_evt;                        /**< FOTA Event type */
   union FOTAEvent_U
   {
      FOTAStart_T st_FOTAStart;              /**< FOTA Start Event details */
      Metadata_T st_metadata;                /**< Metadata details */
      Manifest_T st_manifest;                /**< Manifest details */
      FOTAData_T st_FOTAData;                /**< FOTA Data details */
      FOTAAbort_T st_FOTAAbort;              /**< FOTA Abort details */
      // More FOTA Event details can be added here as needed
   } u_FOTAEvents;
} FOTAEvent_T;

/******************************************************************************/
/*                                                                            */
/*                                   UNIONS                                   */
/*                                                                            */
/******************************************************************************/
/**
 * @union         <Union name>
 * @brief         <Union details>.
 */

/******************************************************************************/
/*                                                                            */
/*                              EXTERN VARIABLES                              */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*                                                                            */
/*                              EXTERN FUNCTIONS                              */
/*                                                                            */
/******************************************************************************/

#endif //!_FOTA_EVENT_TYPES_H

/**
 * Copyright(c) Bajaj Auto Technology Limited (BATL) as an unpublished work.
 * THIS SOFTWARE AND/OR MATERIAL IS THE PROPERTY OF BATL.
 * ALL USE, DISCLOSURE, AND/OR REPRODUCTION NOT SPECIFICALLY AUTHORIZED BY
 * BATL IS PROHIBITED.
 *
 * @author:Shivam Chudasama [SC]
 */
