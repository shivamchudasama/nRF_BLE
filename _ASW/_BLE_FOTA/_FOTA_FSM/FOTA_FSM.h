/**
 * @file          FOTA_FSM.h
 * @brief         Header file containing FOTA FSM related definitions and declarations
 * @date          13/02/2026
 * @author        Shivam Chudasama [SC]
 * @copyright     Bajaj Auto Technology Limited (BATL)
 */

#ifndef _FOTA_FSM_H
#define _FOTA_FSM_H

/******************************************************************************/
/*                                                                            */
/*                                  INCLUDES                                  */
/*                                                                            */
/******************************************************************************/
#include <zephyr/smf.h>
#include <zephyr/logging/log.h>

/******************************************************************************/
/*                                                                            */
/*                                  DEFINES                                   */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*                                                                            */
/*                                   ENUMS                                    */
/*                                                                            */
/******************************************************************************/
/**
 * @enum          FOTAState_E
 * @brief         FOTA state machine states.
 */
typedef enum
{
   eFS_STATE_IDLE,                           /**< Idle state */
   eFS_STATE_RECEIVING,                      /**< Receiving firmware data */
   eFS_STATE_VERIFY,                         /**< Verifying received firmware */
   eFS_STATE_DONE,                           /**< Update process completed */
   eFS_STATE_MAX                             /**< Maximum state value */
} FOTAState_E;

/******************************************************************************/
/*                                                                            */
/*                                 STRUCTURES                                 */
/*                                                                            */
/******************************************************************************/
/**
 * @struct        FOTACtx_T
 * @brief         FOTA context structure.
 */
typedef struct
{
   struct smf_ctx st_SMFCtx;                 /**< State machine context */
   bool b_startReq;                          /**< Flag to indicate if start request is received */
   bool b_dataComplete;                      /**< Flag to indicate if all firmware data is received */
   bool b_verifyOk;                          /**< Flag to indicate if firmware verification is successful */
} FOTACtx_T;

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
extern const struct smf_state gst_FOTAStates[eFS_STATE_MAX];

/******************************************************************************/
/*                                                                            */
/*                              EXTERN FUNCTIONS                              */
/*                                                                            */
/******************************************************************************/

#endif //!_FOTA_FSM_H

/**
 * Copyright(c) Bajaj Auto Technology Limited (BATL) as an unpublished work.
 * THIS SOFTWARE AND/OR MATERIAL IS THE PROPERTY OF BATL.
 * ALL USE, DISCLOSURE, AND/OR REPRODUCTION NOT SPECIFICALLY AUTHORIZED BY
 * BATL IS PROHIBITED.
 *
 * @author:Shivam Chudasama [SC]
 */
