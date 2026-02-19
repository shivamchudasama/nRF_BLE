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
 * @def           FOTAEventChannelSub
 * @brief         Subscribber for FOTAEventChannel with queue size of 4.
 */
ZBUS_SUBSCRIBER_DEFINE(FOTAEventChannelSub, 4);

/**
 * @def           FOTA_STATE_MACHINE_THREAD_STACK_SIZE
 * @brief         Stack size of the thread running the FOTA state machine.
 */
#define FOTA_STATE_MACHINE_THREAD_STACK_SIZE (1024)

/**
 * @def           FOTA_STATE_MACHINE_THREAD_PRIO
 * @brief         Priority of the thread running the FOTA state machine.
 */
#define FOTA_STATE_MACHINE_THREAD_PRIO       (5)

/******************************************************************************/
/*                                                                            */
/*                                   ENUMS                                    */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*                                                                            */
/*                                 STRUCTURES                                 */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*                                                                            */
/*                                   UNIONS                                   */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*                                                                            */
/*                       PRIVATE FUNCTION DECLARATIONS                        */
/*                                                                            */
/******************************************************************************/
static void sv_eFS_IDLE_Entry(void *vpt_obj);
static enum smf_state_result se_eFS_IDLE_Run(void *vpt_obj);
static void sv_eFS_IDLE_Exit(void *vpt_obj);

static void sv_eFS_RECEIVING_METADATA_Entry(void *vpt_obj);
static enum smf_state_result se_eFS_RECEIVING_METADATA_Run(void *vpt_obj);
static void sv_eFS_RECEIVING_METADATA_Exit(void *vpt_obj);

static void sv_eFS_RECEIVING_MANIFEST_Entry(void *vpt_obj);
static enum smf_state_result se_eFS_RECEIVING_MANIFEST_Run(void *vpt_obj);
static void sv_eFS_RECEIVING_MANIFEST_Exit(void *vpt_obj);

static void sv_eFS_RECEIVING_DATA_Entry(void *vpt_obj);
static enum smf_state_result se_eFS_RECEIVING_DATA_Run(void *vpt_obj);
static void sv_eFS_RECEIVING_DATA_Exit(void *vpt_obj);

static void sv_eFS_VALIDATE_IMAGE_Entry(void *vpt_obj);
static enum smf_state_result se_eFS_VALIDATE_IMAGE_Run(void *vpt_obj);
static void sv_eFS_VALIDATE_IMAGE_Exit(void *vpt_obj);

static void sv_eFS_STAGE_IMAGE_Entry(void *vpt_obj);
static enum smf_state_result se_eFS_STAGE_IMAGE_Run(void *vpt_obj);
static void sv_eFS_STAGE_IMAGE_Exit(void *vpt_obj);

static void sv_eFS_COMPLETED_Entry(void *vpt_obj);
static enum smf_state_result se_eFS_COMPLETED_Run(void *vpt_obj);
static void sv_eFS_COMPLETED_Exit(void *vpt_obj);

static void sv_eFS_ABORT_Entry(void *vpt_obj);
static enum smf_state_result se_eFS_ABORT_Run(void *vpt_obj);
static void sv_eFS_ABORT_Exit(void *vpt_obj);

static void sv_FOTAStateMachineThread(void *vpt_entryParam1, void *vpt_entryParam2,
   void *vpt_entryParam3);

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
   [eFS_RECEIVING_MANIFEST] = SMF_CREATE_STATE(sv_eFS_RECEIVING_MANIFEST_Entry, se_eFS_RECEIVING_MANIFEST_Run, sv_eFS_RECEIVING_MANIFEST_Exit, NULL, NULL),
   [eFS_RECEIVING_METADATA] = SMF_CREATE_STATE(sv_eFS_RECEIVING_METADATA_Entry, se_eFS_RECEIVING_METADATA_Run, sv_eFS_RECEIVING_METADATA_Exit, NULL, NULL),
   [eFS_RECEIVING_DATA] = SMF_CREATE_STATE(sv_eFS_RECEIVING_DATA_Entry, se_eFS_RECEIVING_DATA_Run, sv_eFS_RECEIVING_DATA_Exit, NULL, NULL),
   [eFS_VALIDATE_IMAGE] = SMF_CREATE_STATE(sv_eFS_VALIDATE_IMAGE_Entry, se_eFS_VALIDATE_IMAGE_Run, sv_eFS_VALIDATE_IMAGE_Exit, NULL, NULL),
   [eFS_STAGE_IMAGE] = SMF_CREATE_STATE(sv_eFS_STAGE_IMAGE_Entry, se_eFS_STAGE_IMAGE_Run, sv_eFS_STAGE_IMAGE_Exit, NULL, NULL),
   [eFS_COMPLETED] = SMF_CREATE_STATE(sv_eFS_COMPLETED_Entry, se_eFS_COMPLETED_Run, sv_eFS_COMPLETED_Exit, NULL, NULL),
   [eFS_ABORT] = SMF_CREATE_STATE(sv_eFS_ABORT_Entry, se_eFS_ABORT_Run, sv_eFS_ABORT_Exit, NULL, NULL),
};

/**
 * @var           sst_FOTAStateMachineCtx
 * @brief         FOTA SMF context structure.
 */
static FOTAStateMachineCtx_T sst_FOTAStateMachineCtx = { 0 };

/**
 * @var           sst_FOTAStateMachineThread
 * @brief         FOTA state machine thread structure.
 */
static struct k_thread sst_FOTAStateMachineThread;

/**
 * @var           sst_FOTAStateMachineThreadStack
 * @brief         FOTA state machine thread stack.
 */
K_THREAD_STACK_DEFINE(sst_FOTAStateMachineThreadStack, FOTA_STATE_MACHINE_THREAD_STACK_SIZE);

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
 * @param[in]     vpt_obj Pointer to user object declared for FOTA state machine.
 * @param[out]    <Output parameter details>.
 * @param[inout]  <Input-Output parameter details>.
 * @return        <Return details>.
 */
static void sv_eFS_IDLE_Entry(void *vpt_obj)
{
   LOG_INF("sv_eFS_IDLE entry");
}

/**
 * @private       sv_eFS_IDLE_Run
 * @brief         <Function details>.
 * @param[in]     vpt_obj Pointer to user object declared for FOTA state machine.
 * @return        <Return details>.
 */
static enum smf_state_result se_eFS_IDLE_Run(void *vpt_obj)
{
   FOTAStateMachineCtx_T *stpt_FOTAStateMachineCtx = (FOTAStateMachineCtx_T *)vpt_obj;
   enum smf_state_result e_retVal = SMF_EVENT_PROPAGATE;

   LOG_INF("sv_eFS_IDLE running");

   // Check if any event is pending to handle and that is FOTA START command
   if ((stpt_FOTAStateMachineCtx->b_isEventPending) &&
      (eFE_FOTA_START == stpt_FOTAStateMachineCtx->st_FOTAEvent.e_evt))
   {
      // Check if FOTA start signature has been received
      if (FOTA_START_SIGNATURE ==
         stpt_FOTAStateMachineCtx->st_FOTAEvent.u_FOTAEventsPayload.st_FOTAStart.u32_FOTAStartSignal)
      {
         // Clear the event pending flag
         stpt_FOTAStateMachineCtx->b_isEventPending = false;
         LOG_INF("Start request received, transitioning to eFS_RECEIVING_MANIFEST state");
         smf_set_state(SMF_CTX(stpt_FOTAStateMachineCtx), &gst_FOTAStates[eFS_RECEIVING_MANIFEST]);
      }
      else
      {
         LOG_INF("Invalid start signature.");
      }

      e_retVal = SMF_EVENT_HANDLED;
   }
   else
   {
      LOG_INF("Invalid activity");
   }

   return e_retVal;
}

/**
 * @private       sv_eFS_IDLE_Exit
 * @brief         <Function details>.
 * @param[in]     vpt_obj Pointer to user object declared for FOTA state machine.
 * @return        <Return details>.
 */
static void sv_eFS_IDLE_Exit(void *vpt_obj)
{
   LOG_INF("sv_eFS_IDLE exit");
}

// eFS_RECEIVING_MANIFEST state handlers
/**
 * @private       sv_eFS_RECEIVING_MANIFEST_Entry
 * @brief         <Function details>.
 * @param[in]     vpt_obj Pointer to user object declared for FOTA state machine.
 * @return        <Return details>.
 */
static void sv_eFS_RECEIVING_MANIFEST_Entry(void *vpt_obj)
{
   LOG_INF("sv_eFS_RECEIVING_MANIFEST entry");
}

/**
 * @private       sv_eFS_RECEIVING_MANIFEST_Run
 * @brief         <Function details>.
 * @param[in]     vpt_obj Pointer to user object declared for FOTA state machine.
 * @return        <Return details>.
 */
static enum smf_state_result se_eFS_RECEIVING_MANIFEST_Run(void *vpt_obj)
{
   FOTAStateMachineCtx_T *stpt_FOTAStateMachineCtx = (FOTAStateMachineCtx_T *)vpt_obj;
   enum smf_state_result e_retVal = SMF_EVENT_PROPAGATE;
   CPList_T st_CPList = { 0 };

   LOG_INF("sv_eFS_RECEIVING_MANIFEST running");

   // Check if any event is pending to handle and that is MANIFEST command
   if ((stpt_FOTAStateMachineCtx->b_isEventPending) &&
      (eFE_METADATA == stpt_FOTAStateMachineCtx->st_FOTAEvent.e_evt))
   {
      // Clear the event pending flag
      stpt_FOTAStateMachineCtx->b_isEventPending = false;
      // As there might be more than one manifest packets received. We need to parst it.
      ge_TP_ParseCPList(&stpt_FOTAStateMachineCtx->st_FOTAEvent.u_FOTAEventsPayload.star_metadata[0],
         stpt_FOTAStateMachineCtx->st_FOTAEvent.u16_payloadLength, &st_CPList);

      // As this is a Metadata, there are multiple CP blocks in it. After parsing
      // them, we need to send them to the FS FSM.

      LOG_INF("Metadata received, transitioning to eFS_RECEIVING_METADATA state");
      smf_set_state(SMF_CTX(stpt_FOTAStateMachineCtx), &gst_FOTAStates[eFS_RECEIVING_METADATA]);

      e_retVal = SMF_EVENT_HANDLED;
   }

   return e_retVal;
}

/**
 * @private       sv_eFS_RECEIVING_MANIFEST_Exit
 * @brief         <Function details>.
 * @param[in]     vpt_obj Pointer to user object declared for FOTA state machine.
 * @return        <Return details>.
 */
static void sv_eFS_RECEIVING_MANIFEST_Exit(void *vpt_obj)
{
   LOG_INF("sv_eFS_RECEIVING_MANIFEST exit");
}

// eFS_RECEIVING_METADATA state handlers
/**
 * @private       sv_eFS_RECEIVING_METADATA_Entry
 * @brief         <Function details>.
 * @param[in]     vpt_obj Pointer to user object declared for FOTA state machine.
 * @return        <Return details>.
 */
static void sv_eFS_RECEIVING_METADATA_Entry(void *vpt_obj)
{
   LOG_INF("sv_eFS_RECEIVING_METADATA entry");
}

/**
 * @private       sv_eFS_RECEIVING_METADATA_Run
 * @brief         <Function details>.
 * @param[in]     vpt_obj Pointer to user object declared for FOTA state machine.
 * @return        <Return details>.
 */
static enum smf_state_result se_eFS_RECEIVING_METADATA_Run(void *vpt_obj)
{
   FOTAStateMachineCtx_T *stpt_FOTAStateMachineCtx = (FOTAStateMachineCtx_T *)vpt_obj;
   enum smf_state_result e_retVal = SMF_EVENT_PROPAGATE;
   CPList_T st_CPList = { 0 };

   LOG_INF("sv_eFS_RECEIVING_METADATA running");

   // Check if any event is pending to handle and that is METADATA command
   if ((stpt_FOTAStateMachineCtx->b_isEventPending) &&
      (eFE_MANIFEST == stpt_FOTAStateMachineCtx->st_FOTAEvent.e_evt))
   {
      // Clear the event pending flag
      stpt_FOTAStateMachineCtx->b_isEventPending = false;
      // As there might be more than one metadata packets received. We need to parst it.
      ge_TP_ParseCPList(&stpt_FOTAStateMachineCtx->st_FOTAEvent.u_FOTAEventsPayload.st_manifest,
         stpt_FOTAStateMachineCtx->st_FOTAEvent.u16_payloadLength, &st_CPList);

      // As this is a Manifest, there are multiple CP blocks in it. After parsing
      // them, we need to send them to the FS FSM.

      LOG_INF("Manifest received, transitioning to eFS_RECEIVING_DATA state");
      smf_set_state(SMF_CTX(stpt_FOTAStateMachineCtx), &gst_FOTAStates[eFS_RECEIVING_DATA]);

      e_retVal = SMF_EVENT_HANDLED;
   }
   else
   {
      LOG_INF("Invalid activity");
   }

   return e_retVal;
}

/**
 * @private       sv_eFS_RECEIVING_METADATA_Exit
 * @brief         <Function details>.
 * @param[in]     vpt_obj Pointer to user object declared for FOTA state machine.
 * @return        <Return details>.
 */
static void sv_eFS_RECEIVING_METADATA_Exit(void *vpt_obj)
{
   LOG_INF("sv_eFS_RECEIVING_METADATA exit");
}

// eFS_RECEIVING_DATA state handlers
/**
 * @private       sv_eFS_RECEIVING_DATA_Entry
 * @brief         <Function details>.
 * @param[in]     vpt_obj Pointer to user object declared for FOTA state machine.
 * @return        <Return details>.
 */
static void sv_eFS_RECEIVING_DATA_Entry(void *vpt_obj)
{
   LOG_INF("sv_eFS_RECEIVING_DATA entry");
}

/**
 * @private       sv_eFS_RECEIVING_DATA_Run
 * @brief         <Function details>.
 * @param[in]     vpt_obj Pointer to user object declared for FOTA state machine.
 * @return        <Return details>.
 */
static enum smf_state_result se_eFS_RECEIVING_DATA_Run(void *vpt_obj)
{
   FOTAStateMachineCtx_T *stpt_FOTAStateMachineCtx = (FOTAStateMachineCtx_T *)vpt_obj;
   enum smf_state_result e_retVal = SMF_EVENT_PROPAGATE;

   LOG_INF("sv_eFS_RECEIVING_DATA running");

   // Check if any event is pending to handle and that is FOTA DATA command
   if ((stpt_FOTAStateMachineCtx->b_isEventPending) &&
      (eFE_FOTA_DATA == stpt_FOTAStateMachineCtx->st_FOTAEvent.e_evt))
   {
      // As this is a Data,  send them to the FS FSM.

      LOG_INF("Data received, transitioning to eFS_VALIDATE_IMAGE state");
      smf_set_state(SMF_CTX(stpt_FOTAStateMachineCtx), &gst_FOTAStates[eFS_VALIDATE_IMAGE]);

      e_retVal = SMF_EVENT_HANDLED;
   }

   return e_retVal;
}

/**
 * @private       sv_eFS_RECEIVING_DATA_Exit
 * @brief         <Function details>.
 * @param[in]     vpt_obj Pointer to user object declared for FOTA state machine.
 * @return        <Return details>.
 */
static void sv_eFS_RECEIVING_DATA_Exit(void *vpt_obj)
{
   LOG_INF("sv_eFS_RECEIVING_DATA exit");
}

// eFS_VALIDATE_IMAGE state handlers
/**
 * @private       sv_eFS_VALIDATE_IMAGE_Entry
 * @brief         <Function details>.
 * @param[in]     vpt_obj Pointer to user object declared for FOTA state machine.
 * @return        <Return details>.
 */
static void sv_eFS_VALIDATE_IMAGE_Entry(void *vpt_obj)
{
   LOG_INF("sv_eFS_VALIDATE_IMAGE entry");
}

/**
 * @private       sv_eFS_VALIDATE_IMAGE_Run
 * @brief         <Function details>.
 * @param[in]     vpt_obj Pointer to user object declared for FOTA state machine.
 * @return        <Return details>.
 */
static enum smf_state_result se_eFS_VALIDATE_IMAGE_Run(void *vpt_obj)
{
   FOTAStateMachineCtx_T *stpt_FOTAStateMachineCtx = (FOTAStateMachineCtx_T *)vpt_obj;
   enum smf_state_result e_retVal = SMF_EVENT_PROPAGATE;

   LOG_INF("sv_eFS_VALIDATE_IMAGE running");

   // if (stpt_FOTAStateMachineCtx->b_verifyOk)
   // {
   //    LOG_INF("Verify OK received, transitioning to eFS_STAGE_IMAGE state");
   //    smf_set_state(SMF_CTX(stpt_FOTAStateMachineCtx), &gst_FOTAStates[eFS_STAGE_IMAGE]);
   //    e_retVal = SMF_EVENT_HANDLED;
   // }

   return e_retVal;
}

/**
 * @private       sv_eFS_VALIDATE_IMAGE_Exit
 * @brief         <Function details>.
 * @param[in]     vpt_obj Pointer to user object declared for FOTA state machine.
 * @return        <Return details>.
 */
static void sv_eFS_VALIDATE_IMAGE_Exit(void *vpt_obj)
{
   LOG_INF("sv_eFS_VALIDATE_IMAGE exit");
}

// eFS_STAGE_IMAGE state handlers
/**
 * @private       sv_eFS_STAGE_IMAGE_Entry
 * @brief         <Function details>.
 * @param[in]     vpt_obj Pointer to user object declared for FOTA state machine.
 * @return        <Return details>.
 */
static void sv_eFS_STAGE_IMAGE_Entry(void *vpt_obj)
{
   LOG_INF("sv_eFS_STAGE_IMAGE entry");
}

/**
 * @private       sv_eFS_STAGE_IMAGE_Run
 * @brief         <Function details>.
 * @param[in]     vpt_obj Pointer to user object declared for FOTA state machine.
 * @return        <Return details>.
 */
static enum smf_state_result se_eFS_STAGE_IMAGE_Run(void *vpt_obj)
{
   FOTAStateMachineCtx_T *stpt_FOTAStateMachineCtx = (FOTAStateMachineCtx_T *)vpt_obj;
   enum smf_state_result e_retVal = SMF_EVENT_PROPAGATE;

   LOG_INF("sv_eFS_STAGE_IMAGE running");

   // if (stpt_FOTAStateMachineCtx->b_verifyOk)
   // {
   //    LOG_INF("Verify OK received, transitioning to eFS_COMPLETED state");
   //    smf_set_state(SMF_CTX(stpt_FOTAStateMachineCtx), &gst_FOTAStates[eFS_COMPLETED]);
   //    e_retVal = SMF_EVENT_HANDLED;
   // }

   return e_retVal;
}

/**
 * @private       sv_eFS_STAGE_IMAGE_Exit
 * @brief         <Function details>.
 * @param[in]     vpt_obj Pointer to user object declared for FOTA state machine.
 * @return        <Return details>.
 */
static void sv_eFS_STAGE_IMAGE_Exit(void *vpt_obj)
{
   LOG_INF("sv_eFS_STAGE_IMAGE exit");
}

// eFS_COMPLETED state handlers
/**
 * @private       sv_eFS_COMPLETED_Entry
 * @brief         <Function details>.
 * @param[in]     vpt_obj Pointer to user object declared for FOTA state machine.
 * @return        <Return details>.
 */
static void sv_eFS_COMPLETED_Entry(void *vpt_obj)
{
   LOG_INF("sv_eFS_COMPLETED entry");
}

/**
 * @private       sv_eFS_COMPLETED_Run
 * @brief         <Function details>.
 * @param[in]     vpt_obj Pointer to user object declared for FOTA state machine.
 * @return        <Return details>.
 */
static enum smf_state_result se_eFS_COMPLETED_Run(void *vpt_obj)
{
   FOTAStateMachineCtx_T *stpt_FOTAStateMachineCtx = (FOTAStateMachineCtx_T *)vpt_obj;
   enum smf_state_result e_retVal = SMF_EVENT_PROPAGATE;

   LOG_INF("sv_eFS_COMPLETED running");

   // if (stpt_FOTAStateMachineCtx->b_verifyOk)
   // {
   //    LOG_INF("Verify OK received, transitioning to eFS_IDLE state");
   //    smf_set_state(SMF_CTX(stpt_FOTAStateMachineCtx), &gst_FOTAStates[eFS_IDLE]);
   //    e_retVal = SMF_EVENT_HANDLED;
   // }

   return e_retVal;
}

/**
 * @private       sv_eFS_COMPLETED_Exit
 * @brief         <Function details>.
 * @param[in]     vpt_obj Pointer to user object declared for FOTA state machine.
 * @return        <Return details>.
 */
static void sv_eFS_COMPLETED_Exit(void *vpt_obj)
{
   LOG_INF("sv_eFS_COMPLETED exit");
}

// eFS_ABORT state handlers
/**
 * @private       sv_eFS_ABORT_Entry
 * @brief         <Function details>.
 * @param[in]     vpt_obj Pointer to user object declared for FOTA state machine.
 * @return        <Return details>.
 */
static void sv_eFS_ABORT_Entry(void *vpt_obj)
{
   LOG_INF("sv_eFS_ABORT entry");
}

/**
 * @private       sv_eFS_ABORT_Run
 * @brief         <Function details>.
 * @param[in]     vpt_obj Pointer to user object declared for FOTA state machine.
 * @return        <Return details>.
 */
static enum smf_state_result se_eFS_ABORT_Run(void *vpt_obj)
{
   FOTAStateMachineCtx_T *stpt_FOTAStateMachineCtx = (FOTAStateMachineCtx_T *)vpt_obj;
   enum smf_state_result e_retVal = SMF_EVENT_PROPAGATE;

   LOG_INF("sv_eFS_ABORT running");

   // if (stpt_FOTAStateMachineCtx->b_startReq)
   // {
   //    LOG_INF("Start request received, transitioning to eFS_COMPLETED state");
   //    smf_set_state(SMF_CTX(stpt_FOTAStateMachineCtx), &gst_FOTAStates[eFS_COMPLETED]);
   //    e_retVal = SMF_EVENT_HANDLED;
   // }

   return e_retVal;
}

/**
 * @private       sv_eFS_ABORT_Exit
 * @brief         <Function details>.
 * @param[in]     vpt_obj Pointer to user object declared for FOTA state machine.
 * @return        <Return details>.
 */
static void sv_eFS_ABORT_Exit(void *vpt_obj)
{
   LOG_INF("sv_eFS_ABORT exit");
}

/**
 * @private       sv_FOTAStateMachineThread
 * @brief         FOTA thread function. This function is called when the FOTA
 *                thread is running. It waits for publisher to publish events on
 *                FOTAEventChannelSub and reads it if published. It also runs
 *                FOTA state machine.
 * @return        Number of bytes written.
 */
static void sv_FOTAStateMachineThread(void *vpt_entryParam1, void *vpt_entryParam2,
   void *vpt_entryParam3)
{
   const struct zbus_channel *stpt_channel;
   FOTAEvent_T st_FOTAEvent;

   ARG_UNUSED(vpt_entryParam1);
   ARG_UNUSED(vpt_entryParam2);
   ARG_UNUSED(vpt_entryParam3);

   while (1)
   {
      LOG_INF("FOTA Thread started");

      // Wait for FOTA events from the FOTAEventChannel and process them
      if (0 == zbus_sub_wait(&FOTAEventChannelSub, &stpt_channel, K_FOREVER))
      {
         // Check if the event is from FOTAEventChannel
         if (&FOTAEventChannel == stpt_channel)
         {
            // Read the latest message from the channel (populate the FOTA state
            // machine with the latest event).
            zbus_chan_read(&FOTAEventChannel, &sst_FOTAStateMachineCtx.st_FOTAEvent, K_FOREVER);

            // Mark the pending event flag
            sst_FOTAStateMachineCtx.b_isEventPending = true;

            LOG_INF("Received FOTA Event from ZBUS channel: Event Type: %d", sst_FOTAStateMachineCtx.st_FOTAEvent.e_evt);
            LOG_INF("Received FOTA Event payload: 0x%08x", sst_FOTAStateMachineCtx.st_FOTAEvent.u_FOTAEventsPayload.st_FOTAStart.u32_FOTAStartSignal);

            LOG_INF("Running FOTA state machine");

            // Execute the FOTA state machine
            smf_run_state(SMF_CTX(&sst_FOTAStateMachineCtx));
         }
      }
      else
      {
         LOG_INF("No FOTA Event received from ZBUS channel");
      }
   }
}

/******************************************************************************/
/*                                                                            */
/*                        PUBLIC FUNCTION DEFINITIONS                         */
/*                                                                            */
/******************************************************************************/
/**
 * @public        gv_CreateFOTAStateMachineThread
 * @brief         Initialise SMF for FOTA and creates FOTA thread.
 * @return        THread ID.
 */
void gv_CreateFOTAStateMachineThread()
{
   LOG_INF("Initializing FOTA state machine");
   smf_set_initial(SMF_CTX(&sst_FOTAStateMachineCtx), &gst_FOTAStates[eFS_IDLE]);

   // Create a thread to run the FOTA state machine
   k_thread_create(&sst_FOTAStateMachineThread, sst_FOTAStateMachineThreadStack,
      K_THREAD_STACK_SIZEOF(sst_FOTAStateMachineThreadStack), sv_FOTAStateMachineThread, NULL, NULL, NULL,
      FOTA_STATE_MACHINE_THREAD_PRIO, 0, K_NO_WAIT);

   // Set the name of the FOTA thread for debugging purposes
   k_thread_name_set(&sst_FOTAStateMachineThread, "FOTAThread");
}

/**
 * Copyright(c) Bajaj Auto Technology Limited (BATL) as an unpublished work.
 * THIS SOFTWARE AND/OR MATERIAL IS THE PROPERTY OF BATL.
 * ALL USE, DISCLOSURE, AND/OR REPRODUCTION NOT SPECIFICALLY AUTHORIZED BY
 * BATL IS PROHIBITED.
 *
 * @author:Shivam Chudasama [SC]
 */
