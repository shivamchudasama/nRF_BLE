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
/**
 * @def           APP_LOG
 * @brief         Declare - already registered - the application log module.
 *                Module name is APP_LOG. Log level is set to INFO.
 */
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
static void sv_eFS_IDLE_Entry(void *obj);
static enum smf_state_result se_eFS_IDLE_Run(void *obj);
static void sv_eFS_IDLE_Exit(void *obj);

static void sv_eFS_RECEIVING_METADATA_Entry(void *obj);
static enum smf_state_result se_eFS_RECEIVING_METADATA_Run(void *obj);
static void sv_eFS_RECEIVING_METADATA_Exit(void *obj);

static void sv_eFS_RECEIVING_MANIFEST_Entry(void *obj);
static enum smf_state_result se_eFS_RECEIVING_MANIFEST_Run(void *obj);
static void sv_eFS_RECEIVING_MANIFEST_Exit(void *obj);

static void sv_eFS_RECEIVING_DATA_Entry(void *obj);
static enum smf_state_result se_eFS_RECEIVING_DATA_Run(void *obj);
static void sv_eFS_RECEIVING_DATA_Exit(void *obj);

static void sv_eFS_VALIDATE_IMAGE_Entry(void *obj);
static enum smf_state_result se_eFS_VALIDATE_IMAGE_Run(void *obj);
static void sv_eFS_VALIDATE_IMAGE_Exit(void *obj);

static void sv_eFS_STAGE_IMAGE_Entry(void *obj);
static enum smf_state_result se_eFS_STAGE_IMAGE_Run(void *obj);
static void sv_eFS_STAGE_IMAGE_Exit(void *obj);

static void sv_eFS_COMPLETED_Entry(void *obj);
static enum smf_state_result se_eFS_COMPLETED_Run(void *obj);
static void sv_eFS_COMPLETED_Exit(void *obj);

static void sv_eFS_ABORT_Entry(void *obj);
static enum smf_state_result se_eFS_ABORT_Run(void *obj);
static void sv_eFS_ABORT_Exit(void *obj);

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
   [eFS_IDLE] = SMF_CREATE_STATE(sv_eFS_IDLE_Entry, se_eFS_IDLE_Run, sv_eFS_IDLE_Exit, NULL, NULL),
   [eFS_RECEIVING_METADATA] = SMF_CREATE_STATE(sv_eFS_RECEIVING_METADATA_Entry, se_eFS_RECEIVING_METADATA_Run, sv_eFS_RECEIVING_METADATA_Exit, NULL, NULL),
   [eFS_RECEIVING_MANIFEST] = SMF_CREATE_STATE(sv_eFS_RECEIVING_MANIFEST_Entry, se_eFS_RECEIVING_MANIFEST_Run, sv_eFS_RECEIVING_MANIFEST_Exit, NULL, NULL),
   [eFS_RECEIVING_DATA] = SMF_CREATE_STATE(sv_eFS_RECEIVING_DATA_Entry, se_eFS_RECEIVING_DATA_Run, sv_eFS_RECEIVING_DATA_Exit, NULL, NULL),
   [eFS_VALIDATE_IMAGE] = SMF_CREATE_STATE(sv_eFS_VALIDATE_IMAGE_Entry, se_eFS_VALIDATE_IMAGE_Run, sv_eFS_VALIDATE_IMAGE_Exit, NULL, NULL),
   [eFS_STAGE_IMAGE] = SMF_CREATE_STATE(sv_eFS_STAGE_IMAGE_Entry, se_eFS_STAGE_IMAGE_Run, sv_eFS_STAGE_IMAGE_Exit, NULL, NULL),
   [eFS_COMPLETED] = SMF_CREATE_STATE(sv_eFS_COMPLETED_Entry, se_eFS_COMPLETED_Run, sv_eFS_COMPLETED_Exit, NULL, NULL),
   [eFS_ABORT] = SMF_CREATE_STATE(sv_eFS_ABORT_Entry, se_eFS_ABORT_Run, sv_eFS_ABORT_Exit, NULL, NULL),
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
// eFS_IDLE state handlers
/**
 * @private       sv_eFS_IDLE_Entry
 * @brief         <Function details>.
 * @param[in]     <Input parameter details>.
 * @param[out]    <Output parameter details>.
 * @param[inout]  <Input-Output parameter details>.
 * @return        <Return details>.
 */
static void sv_eFS_IDLE_Entry(void *obj)
{
   LOG_INF("sv_eFS_IDLE entry");
}

/**
 * @private       sv_eFS_IDLE_Run
 * @brief         <Function details>.
 * @param[in]     <Input parameter details>.
 * @param[out]    <Output parameter details>.
 * @param[inout]  <Input-Output parameter details>.
 * @return        <Return details>.
 */
static enum smf_state_result se_eFS_IDLE_Run(void *obj)
{
   FOTACtx_T *stpt_FOTACtx = (FOTACtx_T *)obj;
   enum smf_state_result e_retVal = SMF_EVENT_PROPAGATE;

   LOG_INF("sv_eFS_IDLE running");

   if (stpt_FOTACtx->b_startReq)
   {
      LOG_INF("Start request received, transitioning to eFS_RECEIVING_METADATA state");
      smf_set_state(SMF_CTX(stpt_FOTACtx), &gst_FOTAStates[eFS_RECEIVING_METADATA]);
      e_retVal = SMF_EVENT_HANDLED;
   }

   return e_retVal;
}

/**
 * @private       sv_eFS_IDLE_Exit
 * @brief         <Function details>.
 * @param[in]     <Input parameter details>.
 * @param[out]    <Output parameter details>.
 * @param[inout]  <Input-Output parameter details>.
 * @return        <Return details>.
 */
static void sv_eFS_IDLE_Exit(void *obj)
{
   LOG_INF("sv_eFS_IDLE exit");
}

// eFS_RECEIVING_METADATA state handlers
/**
 * @private       sv_eFS_RECEIVING_METADATA_Entry
 * @brief         <Function details>.
 * @param[in]     <Input parameter details>.
 * @param[out]    <Output parameter details>.
 * @param[inout]  <Input-Output parameter details>.
 * @return        <Return details>.
 */
static void sv_eFS_RECEIVING_METADATA_Entry(void *obj)
{
   LOG_INF("sv_eFS_RECEIVING_METADATA entry");
}

/**
 * @private       sv_eFS_RECEIVING_METADATA_Run
 * @brief         <Function details>.
 * @param[in]     <Input parameter details>.
 * @param[out]    <Output parameter details>.
 * @param[inout]  <Input-Output parameter details>.
 * @return        <Return details>.
 */
static enum smf_state_result se_eFS_RECEIVING_METADATA_Run(void *obj)
{
   FOTACtx_T *stpt_FOTACtx = (FOTACtx_T *)obj;
   enum smf_state_result e_retVal = SMF_EVENT_PROPAGATE;

   LOG_INF("sv_eFS_RECEIVING_METADATA running");

   if (stpt_FOTACtx->b_dataComplete)
   {
      LOG_INF("Data complete received, transitioning to eFS_RECEIVING_MANIFEST state");
      smf_set_state(SMF_CTX(stpt_FOTACtx), &gst_FOTAStates[eFS_RECEIVING_MANIFEST]);
      e_retVal = SMF_EVENT_HANDLED;
   }

   return e_retVal;
}

/**
 * @private       sv_eFS_RECEIVING_METADATA_Exit
 * @brief         <Function details>.
 * @param[in]     <Input parameter details>.
 * @param[out]    <Output parameter details>.
 * @param[inout]  <Input-Output parameter details>.
 * @return        <Return details>.
 */
static void sv_eFS_RECEIVING_METADATA_Exit(void *obj)
{
   LOG_INF("sv_eFS_RECEIVING_METADATA exit");
}

// eFS_RECEIVING_MANIFEST state handlers
/**
 * @private       sv_eFS_RECEIVING_MANIFEST_Entry
 * @brief         <Function details>.
 * @param[in]     <Input parameter details>.
 * @param[out]    <Output parameter details>.
 * @param[inout]  <Input-Output parameter details>.
 * @return        <Return details>.
 */
static void sv_eFS_RECEIVING_MANIFEST_Entry(void *obj)
{
   LOG_INF("sv_eFS_RECEIVING_MANIFEST entry");
}

/**
 * @private       sv_eFS_RECEIVING_MANIFEST_Run
 * @brief         <Function details>.
 * @param[in]     <Input parameter details>.
 * @param[out]    <Output parameter details>.
 * @param[inout]  <Input-Output parameter details>.
 * @return        <Return details>.
 */
static enum smf_state_result se_eFS_RECEIVING_MANIFEST_Run(void *obj)
{
   FOTACtx_T *stpt_FOTACtx = (FOTACtx_T *)obj;
   enum smf_state_result e_retVal = SMF_EVENT_PROPAGATE;

   LOG_INF("sv_eFS_RECEIVING_MANIFEST running");

   if (stpt_FOTACtx->b_verifyOk)
   {
      LOG_INF("Verify OK received, transitioning to eFS_RECEIVING_DATA state");
      smf_set_state(SMF_CTX(stpt_FOTACtx), &gst_FOTAStates[eFS_RECEIVING_DATA]);
      e_retVal = SMF_EVENT_HANDLED;
   }

   return e_retVal;
}

/**
 * @private       sv_eFS_RECEIVING_MANIFEST_Exit
 * @brief         <Function details>.
 * @param[in]     <Input parameter details>.
 * @param[out]    <Output parameter details>.
 * @param[inout]  <Input-Output parameter details>.
 * @return        <Return details>.
 */
static void sv_eFS_RECEIVING_MANIFEST_Exit(void *obj)
{
   LOG_INF("sv_eFS_RECEIVING_MANIFEST exit");
}

// eFS_RECEIVING_DATA state handlers
/**
 * @private       sv_eFS_RECEIVING_DATA_Entry
 * @brief         <Function details>.
 * @param[in]     <Input parameter details>.
 * @param[out]    <Output parameter details>.
 * @param[inout]  <Input-Output parameter details>.
 * @return        <Return details>.
 */
static void sv_eFS_RECEIVING_DATA_Entry(void *obj)
{
   LOG_INF("sv_eFS_RECEIVING_DATA entry");
}

/**
 * @private       sv_eFS_RECEIVING_DATA_Run
 * @brief         <Function details>.
 * @param[in]     <Input parameter details>.
 * @param[out]    <Output parameter details>.
 * @param[inout]  <Input-Output parameter details>.
 * @return        <Return details>.
 */
static enum smf_state_result se_eFS_RECEIVING_DATA_Run(void *obj)
{
   FOTACtx_T *stpt_FOTACtx = (FOTACtx_T *)obj;
   enum smf_state_result e_retVal = SMF_EVENT_PROPAGATE;

   LOG_INF("sv_eFS_RECEIVING_DATA running");

   if (stpt_FOTACtx->b_verifyOk)
   {
      LOG_INF("Verify OK received, transitioning to eFS_VALIDATE_IMAGE state");
      smf_set_state(SMF_CTX(stpt_FOTACtx), &gst_FOTAStates[eFS_VALIDATE_IMAGE]);
      e_retVal = SMF_EVENT_HANDLED;
   }

   return e_retVal;
}

/**
 * @private       sv_eFS_RECEIVING_DATA_Exit
 * @brief         <Function details>.
 * @param[in]     <Input parameter details>.
 * @param[out]    <Output parameter details>.
 * @param[inout]  <Input-Output parameter details>.
 * @return        <Return details>.
 */
static void sv_eFS_RECEIVING_DATA_Exit(void *obj)
{
   LOG_INF("sv_eFS_RECEIVING_DATA exit");
}

// eFS_VALIDATE_IMAGE state handlers
/**
 * @private       sv_eFS_VALIDATE_IMAGE_Entry
 * @brief         <Function details>.
 * @param[in]     <Input parameter details>.
 * @param[out]    <Output parameter details>.
 * @param[inout]  <Input-Output parameter details>.
 * @return        <Return details>.
 */
static void sv_eFS_VALIDATE_IMAGE_Entry(void *obj)
{
   LOG_INF("sv_eFS_VALIDATE_IMAGE entry");
}

/**
 * @private       sv_eFS_VALIDATE_IMAGE_Run
 * @brief         <Function details>.
 * @param[in]     <Input parameter details>.
 * @param[out]    <Output parameter details>.
 * @param[inout]  <Input-Output parameter details>.
 * @return        <Return details>.
 */
static enum smf_state_result se_eFS_VALIDATE_IMAGE_Run(void *obj)
{
   FOTACtx_T *stpt_FOTACtx = (FOTACtx_T *)obj;
   enum smf_state_result e_retVal = SMF_EVENT_PROPAGATE;

   LOG_INF("sv_eFS_VALIDATE_IMAGE running");

   if (stpt_FOTACtx->b_verifyOk)
   {
      LOG_INF("Verify OK received, transitioning to eFS_STAGE_IMAGE state");
      smf_set_state(SMF_CTX(stpt_FOTACtx), &gst_FOTAStates[eFS_STAGE_IMAGE]);
      e_retVal = SMF_EVENT_HANDLED;
   }

   return e_retVal;
}

/**
 * @private       sv_eFS_VALIDATE_IMAGE_Exit
 * @brief         <Function details>.
 * @param[in]     <Input parameter details>.
 * @param[out]    <Output parameter details>.
 * @param[inout]  <Input-Output parameter details>.
 * @return        <Return details>.
 */
static void sv_eFS_VALIDATE_IMAGE_Exit(void *obj)
{
   LOG_INF("sv_eFS_VALIDATE_IMAGE exit");
}

// eFS_STAGE_IMAGE state handlers
/**
 * @private       sv_eFS_STAGE_IMAGE_Entry
 * @brief         <Function details>.
 * @param[in]     <Input parameter details>.
 * @param[out]    <Output parameter details>.
 * @param[inout]  <Input-Output parameter details>.
 * @return        <Return details>.
 */
static void sv_eFS_STAGE_IMAGE_Entry(void *obj)
{
   LOG_INF("sv_eFS_STAGE_IMAGE entry");
}

/**
 * @private       sv_eFS_STAGE_IMAGE_Run
 * @brief         <Function details>.
 * @param[in]     <Input parameter details>.
 * @param[out]    <Output parameter details>.
 * @param[inout]  <Input-Output parameter details>.
 * @return        <Return details>.
 */
static enum smf_state_result se_eFS_STAGE_IMAGE_Run(void *obj)
{
   FOTACtx_T *stpt_FOTACtx = (FOTACtx_T *)obj;
   enum smf_state_result e_retVal = SMF_EVENT_PROPAGATE;

   LOG_INF("sv_eFS_STAGE_IMAGE running");

   if (stpt_FOTACtx->b_verifyOk)
   {
      LOG_INF("Verify OK received, transitioning to eFS_COMPLETED state");
      smf_set_state(SMF_CTX(stpt_FOTACtx), &gst_FOTAStates[eFS_COMPLETED]);
      e_retVal = SMF_EVENT_HANDLED;
   }

   return e_retVal;
}

/**
 * @private       sv_eFS_STAGE_IMAGE_Exit
 * @brief         <Function details>.
 * @param[in]     <Input parameter details>.
 * @param[out]    <Output parameter details>.
 * @param[inout]  <Input-Output parameter details>.
 * @return        <Return details>.
 */
static void sv_eFS_STAGE_IMAGE_Exit(void *obj)
{
   LOG_INF("sv_eFS_STAGE_IMAGE exit");
}

// eFS_COMPLETED state handlers
/**
 * @private       sv_eFS_COMPLETED_Entry
 * @brief         <Function details>.
 * @param[in]     <Input parameter details>.
 * @param[out]    <Output parameter details>.
 * @param[inout]  <Input-Output parameter details>.
 * @return        <Return details>.
 */
static void sv_eFS_COMPLETED_Entry(void *obj)
{
   LOG_INF("sv_eFS_COMPLETED entry");
}

/**
 * @private       sv_eFS_COMPLETED_Run
 * @brief         <Function details>.
 * @param[in]     <Input parameter details>.
 * @param[out]    <Output parameter details>.
 * @param[inout]  <Input-Output parameter details>.
 * @return        <Return details>.
 */
static enum smf_state_result se_eFS_COMPLETED_Run(void *obj)
{
   FOTACtx_T *stpt_FOTACtx = (FOTACtx_T *)obj;
   enum smf_state_result e_retVal = SMF_EVENT_PROPAGATE;

   LOG_INF("sv_eFS_COMPLETED running");

   if (stpt_FOTACtx->b_verifyOk)
   {
      LOG_INF("Verify OK received, transitioning to eFS_IDLE state");  
      smf_set_state(SMF_CTX(stpt_FOTACtx), &gst_FOTAStates[eFS_IDLE]);
      e_retVal = SMF_EVENT_HANDLED;
   }

   return e_retVal;
}

/**
 * @private       sv_eFS_COMPLETED_Exit
 * @brief         <Function details>.
 * @param[in]     <Input parameter details>.
 * @param[out]    <Output parameter details>.
 * @param[inout]  <Input-Output parameter details>.
 * @return        <Return details>.
 */
static void sv_eFS_COMPLETED_Exit(void *obj)
{
   LOG_INF("sv_eFS_COMPLETED exit");
}

// eFS_ABORT state handlers
/**
 * @private       sv_eFS_ABORT_Entry
 * @brief         <Function details>.
 * @param[in]     <Input parameter details>.
 * @param[out]    <Output parameter details>.
 * @param[inout]  <Input-Output parameter details>.
 * @return        <Return details>.
 */
static void sv_eFS_ABORT_Entry(void *obj)
{
   LOG_INF("sv_eFS_ABORT entry");
}

/**
 * @private       sv_eFS_ABORT_Run
 * @brief         <Function details>.
 * @param[in]     <Input parameter details>.
 * @param[out]    <Output parameter details>.
 * @param[inout]  <Input-Output parameter details>.
 * @return        <Return details>.
 */
static enum smf_state_result se_eFS_ABORT_Run(void *obj)
{
   FOTACtx_T *stpt_FOTACtx = (FOTACtx_T *)obj;
   enum smf_state_result e_retVal = SMF_EVENT_PROPAGATE;

   LOG_INF("sv_eFS_ABORT running");

   if (stpt_FOTACtx->b_startReq)
   {
      LOG_INF("Start request received, transitioning to eFS_COMPLETED state");
      smf_set_state(SMF_CTX(stpt_FOTACtx), &gst_FOTAStates[eFS_COMPLETED]);
      e_retVal = SMF_EVENT_HANDLED;
   }

   return e_retVal;
}

/**
 * @private       sv_eFS_ABORT_Exit
 * @brief         <Function details>.
 * @param[in]     <Input parameter details>.
 * @param[out]    <Output parameter details>.
 * @param[inout]  <Input-Output parameter details>.
 * @return        <Return details>.
 */
static void sv_eFS_ABORT_Exit(void *obj)
{
   LOG_INF("sv_eFS_ABORT exit");
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
