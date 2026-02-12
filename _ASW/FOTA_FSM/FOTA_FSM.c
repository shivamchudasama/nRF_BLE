/**
 * @file          FOTA_FSM.c
 * @brief         Source file containing SMF implementation for FOTA service.
 * @date          13/02/2026
 * @author        Shivam Chudasama [SC]
 * @copyright     Bajaj Auto Technology Limited (BATL)
 */

/******************************************************************************/
/*                                                                            */
/*                                  INCLUDES                                  */
/*                                                                            */
/******************************************************************************/
#include "FOTA_FSM.h"

/******************************************************************************/
/*                                                                            */
/*                                  DEFINES                                   */
/*                                                                            */
/******************************************************************************/
LOG_MODULE_DECLARE(APP_LOG);

/******************************************************************************/
/*                                                                            */
/*                                   ENUMS                                    */
/*                                                                            */
/******************************************************************************/
// Definition of all the enums
/**
 * @enum          <Enum name>
 * @brief         <Enum details>.
 */

// Declarations of all the enum variables
/**
 * @var           <Variable name>
 * @brief         <Variable details>.
 */

/******************************************************************************/
/*                                                                            */
/*                                 STRUCTURES                                 */
/*                                                                            */
/******************************************************************************/
// Definition of all the structures
/**
 * @struct        <Structure name>
 * @brief         <Structure details>.
 */

// Declarations of all the structure variables
/**
 * @var           <Variable name>
 * @brief         <Variable details>.
 */

/******************************************************************************/
/*                                                                            */
/*                                   UNIONS                                   */
/*                                                                            */
/******************************************************************************/
// Definition of all the unions
/**
 * @union         <Union name>
 * @brief         <Union details>.
 */

// Declarations of all the union variables
/**
 * @var           <Variable name>
 * @brief         <Variable details>.
 */

/******************************************************************************/
/*                                                                            */
/*                       PRIVATE FUNCTION DECLARATIONS                        */
/*                                                                            */
/******************************************************************************/
static void sv_eFS_STATE_IDLE_Entry(void *obj);
static enum smf_state_result se_eFS_STATE_IDLE_Run(void *obj);
static void sv_eFS_STATE_IDLE_Exit(void *obj);

static void sv_eFS_STATE_RECEIVING_Entry(void *obj);
static enum smf_state_result se_eFS_STATE_RECEIVING_Run(void *obj);
static void sv_eFS_STATE_RECEIVING_Exit(void *obj);

static void sv_eFS_STATE_VERIFY_Entry(void *obj);
static enum smf_state_result se_eFS_STATE_VERIFY_Run(void *obj);
static void sv_eFS_STATE_VERIFY_Exit(void *obj);

static void sv_eFS_STATE_DONE_Entry(void *obj);
static enum smf_state_result se_eFS_STATE_DONE_Run(void *obj);
static void sv_eFS_STATE_DONE_Exit(void *obj);

/******************************************************************************/
/*                                                                            */
/*                              EXTERN VARIABLES                              */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*                                                                            */
/*                              PUBLIC VARIABLES                              */
/*                                                                            */
/******************************************************************************/
/**
 * @var           <Variable name>
 * @brief         <Variable details>.
 */

/******************************************************************************/
/*                                                                            */
/*                             PRIVATE VARIABLES                              */
/*                                                                            */
/******************************************************************************/
/**
 * @var           gst_FOTAStates
 * @brief         FOTA state machine states array.
 */
const struct smf_state gst_FOTAStates[eFS_STATE_MAX] =
{
   [eFS_STATE_IDLE] = SMF_CREATE_STATE(sv_eFS_STATE_IDLE_Entry, se_eFS_STATE_IDLE_Run, sv_eFS_STATE_IDLE_Exit, NULL, NULL),
   [eFS_STATE_RECEIVING] = SMF_CREATE_STATE(sv_eFS_STATE_RECEIVING_Entry, se_eFS_STATE_RECEIVING_Run, sv_eFS_STATE_RECEIVING_Exit, NULL, NULL),
   [eFS_STATE_VERIFY] = SMF_CREATE_STATE(sv_eFS_STATE_VERIFY_Entry, se_eFS_STATE_VERIFY_Run, sv_eFS_STATE_VERIFY_Exit, NULL, NULL),
   [eFS_STATE_DONE] = SMF_CREATE_STATE(sv_eFS_STATE_DONE_Entry, se_eFS_STATE_DONE_Run, sv_eFS_STATE_DONE_Exit, NULL, NULL)
};

/******************************************************************************/
/*                                                                            */
/*                              EXTERN FUNCTIONS                              */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*                                                                            */
/*                        PRIVATE FUNCTION DEFINITIONS                        */
/*                                                                            */
/******************************************************************************/
// eFS_STATE_IDLE state handlers
/**
 * @private       sv_eFS_STATE_IDLE_Entry
 * @brief         <Function details>.
 * @param[in]     <Input parameter details>.
 * @param[out]    <Output parameter details>.
 * @param[inout]  <Input-Output parameter details>.
 * @return        <Return details>.
 */
static void sv_eFS_STATE_IDLE_Entry(void *obj)
{
   LOG_INF("sv_eFS_STATE_IDLE entry");
}

/**
 * @private       sv_eFS_STATE_IDLE_Run
 * @brief         <Function details>.
 * @param[in]     <Input parameter details>.
 * @param[out]    <Output parameter details>.
 * @param[inout]  <Input-Output parameter details>.
 * @return        <Return details>.
 */
static enum smf_state_result se_eFS_STATE_IDLE_Run(void *obj)
{
   FOTACtx_T *stpt_FOTACtx = (FOTACtx_T *)obj;
   enum smf_state_result e_retVal = SMF_EVENT_PROPAGATE;

   LOG_INF("sv_eFS_STATE_IDLE running");

   if (stpt_FOTACtx->b_startReq)
   {
      LOG_INF("Start request received, transitioning to eFS_STATE_RECEIVING state");
      smf_set_state(SMF_CTX(stpt_FOTACtx), &gst_FOTAStates[eFS_STATE_RECEIVING]);
      e_retVal = SMF_EVENT_HANDLED;
   }

   return e_retVal;
}

/**
 * @private       sv_eFS_STATE_IDLE_Exit
 * @brief         <Function details>.
 * @param[in]     <Input parameter details>.
 * @param[out]    <Output parameter details>.
 * @param[inout]  <Input-Output parameter details>.
 * @return        <Return details>.
 */
static void sv_eFS_STATE_IDLE_Exit(void *obj)
{
   LOG_INF("sv_eFS_STATE_IDLE exit");
}

// eFS_STATE_RECEIVING state handlers
/**
 * @private       sv_eFS_STATE_RECEIVING_Entry
 * @brief         <Function details>.
 * @param[in]     <Input parameter details>.
 * @param[out]    <Output parameter details>.
 * @param[inout]  <Input-Output parameter details>.
 * @return        <Return details>.
 */
static void sv_eFS_STATE_RECEIVING_Entry(void *obj)
{
   LOG_INF("sv_eFS_STATE_RECEIVING entry");
}

/**
 * @private       sv_eFS_STATE_RECEIVING_Run
 * @brief         <Function details>.
 * @param[in]     <Input parameter details>.
 * @param[out]    <Output parameter details>.
 * @param[inout]  <Input-Output parameter details>.
 * @return        <Return details>.
 */
static enum smf_state_result se_eFS_STATE_RECEIVING_Run(void *obj)
{
   FOTACtx_T *stpt_FOTACtx = (FOTACtx_T *)obj;
   enum smf_state_result e_retVal = SMF_EVENT_PROPAGATE;

   LOG_INF("sv_eFS_STATE_RECEIVING running");

   if (stpt_FOTACtx->b_dataComplete)
   {
      LOG_INF("Data complete received, transitioning to eFS_STATE_VERIFY state");
      smf_set_state(SMF_CTX(stpt_FOTACtx), &gst_FOTAStates[eFS_STATE_VERIFY]);
      e_retVal = SMF_EVENT_HANDLED;
   }

   return e_retVal;
}

/**
 * @private       sv_eFS_STATE_RECEIVING_Exit
 * @brief         <Function details>.
 * @param[in]     <Input parameter details>.
 * @param[out]    <Output parameter details>.
 * @param[inout]  <Input-Output parameter details>.
 * @return        <Return details>.
 */
static void sv_eFS_STATE_RECEIVING_Exit(void *obj)
{
   LOG_INF("sv_eFS_STATE_RECEIVING exit");
}

// eFS_STATE_VERIFY state handlers
/**
 * @private       sv_eFS_STATE_VERIFY_Entry
 * @brief         <Function details>.
 * @param[in]     <Input parameter details>.
 * @param[out]    <Output parameter details>.
 * @param[inout]  <Input-Output parameter details>.
 * @return        <Return details>.
 */
static void sv_eFS_STATE_VERIFY_Entry(void *obj)
{
   LOG_INF("sv_eFS_STATE_VERIFY entry");
}

/**
 * @private       sv_eFS_STATE_VERIFY_Run
 * @brief         <Function details>.
 * @param[in]     <Input parameter details>.
 * @param[out]    <Output parameter details>.
 * @param[inout]  <Input-Output parameter details>.
 * @return        <Return details>.
 */
static enum smf_state_result se_eFS_STATE_VERIFY_Run(void *obj)
{
   FOTACtx_T *stpt_FOTACtx = (FOTACtx_T *)obj;
   enum smf_state_result e_retVal = SMF_EVENT_PROPAGATE;

   LOG_INF("sv_eFS_STATE_VERIFY running");

   if (stpt_FOTACtx->b_verifyOk)
   {
      LOG_INF("Verify OK received, transitioning to eFS_STATE_DONE state");
      smf_set_state(SMF_CTX(stpt_FOTACtx), &gst_FOTAStates[eFS_STATE_DONE]);
      e_retVal = SMF_EVENT_HANDLED;
   }

   return e_retVal;
}

/**
 * @private       sv_eFS_STATE_VERIFY_Exit
 * @brief         <Function details>.
 * @param[in]     <Input parameter details>.
 * @param[out]    <Output parameter details>.
 * @param[inout]  <Input-Output parameter details>.
 * @return        <Return details>.
 */
static void sv_eFS_STATE_VERIFY_Exit(void *obj)
{
   LOG_INF("sv_eFS_STATE_VERIFY exit");
}

// eFS_STATE_DONE state handlers
/**
 * @private       sv_eFS_STATE_DONE_Entry
 * @brief         <Function details>.
 * @param[in]     <Input parameter details>.
 * @param[out]    <Output parameter details>.
 * @param[inout]  <Input-Output parameter details>.
 * @return        <Return details>.
 */
static void sv_eFS_STATE_DONE_Entry(void *obj)
{
   LOG_INF("sv_eFS_STATE_DONE entry");
}

/**
 * @private       sv_eFS_STATE_DONE_Run
 * @brief         <Function details>.
 * @param[in]     <Input parameter details>.
 * @param[out]    <Output parameter details>.
 * @param[inout]  <Input-Output parameter details>.
 * @return        <Return details>.
 */
static enum smf_state_result se_eFS_STATE_DONE_Run(void *obj)
{
   FOTACtx_T *stpt_FOTACtx = (FOTACtx_T *)obj;
   enum smf_state_result e_retVal = SMF_EVENT_PROPAGATE;

   LOG_INF("sv_eFS_STATE_DONE running");

   if (stpt_FOTACtx->b_startReq)
   {
      LOG_INF("Start request received, transitioning to eFS_STATE_IDLE state");
      smf_set_state(SMF_CTX(stpt_FOTACtx), &gst_FOTAStates[eFS_STATE_IDLE]);
      e_retVal = SMF_EVENT_HANDLED;
   }

   return e_retVal;
}

/**
 * @private       sv_eFS_STATE_DONE_Exit
 * @brief         <Function details>.
 * @param[in]     <Input parameter details>.
 * @param[out]    <Output parameter details>.
 * @param[inout]  <Input-Output parameter details>.
 * @return        <Return details>.
 */
static void sv_eFS_STATE_DONE_Exit(void *obj)
{
   LOG_INF("sv_eFS_STATE_DONE exit");
}

/******************************************************************************/
/*                                                                            */
/*                        PUBLIC FUNCTION DEFINITIONS                         */
/*                                                                            */
/******************************************************************************/
/**
 * @public        <Function name>
 * @brief         <Function details>.
 * @param[in]     <Input parameter details>.
 * @param[out]    <Output parameter details>.
 * @param[inout]  <Input-Output parameter details>.
 * @return        <Return details>.
 */

/**
 * Copyright(c) Bajaj Auto Technology Limited (BATL) as an unpublished work.
 * THIS SOFTWARE AND/OR MATERIAL IS THE PROPERTY OF BATL.
 * ALL USE, DISCLOSURE, AND/OR REPRODUCTION NOT SPECIFICALLY AUTHORIZED BY
 * BATL IS PROHIBITED.
 *
 * @author:Shivam Chudasama [SC]
 */
