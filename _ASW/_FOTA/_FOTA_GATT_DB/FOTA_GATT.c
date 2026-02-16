/**
 * @file          FOTA_GATT.c
 * @brief         Source file containing GATT database for FOTA service
 * @date          10/02/2026
 * @author        Shivam Chudasama [SC]
 * @copyright     Bajaj Auto Technology Limited (BATL)
 */

/******************************************************************************/
/*                                                                            */
/*                                  INCLUDES                                  */
/*                                                                            */
/******************************************************************************/
#include "FOTA_GATT.h"

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
/*                       PRIVATE FUNCTION DECLARATIONS                        */
/*                                                                            */
/******************************************************************************/
static ssize_t st_FOTACtrlWrite(struct bt_conn *stpt_connHandle,
	const struct bt_gatt_attr *stpt_attr, const void *vpt_buf, uint16_t u16_length,
	uint16_t u16_offset, uint8_t u8_flags);
static ssize_t st_FOTADataTransferWrite(struct bt_conn *stpt_connHandle,
	const struct bt_gatt_attr *stpt_attr, 	const void *vpt_buf, uint16_t u16_length,
	uint16_t u16_offset, uint8_t u8_flags);
static ssize_t st_FOTAStsRead(struct bt_conn *stpt_connHandle,
	const struct bt_gatt_attr *stpt_attr, void *vpt_buf, uint16_t u16_length,
	uint16_t u16_offset);
static ssize_t st_FOTAProgressRead(struct bt_conn *stpt_connHandle,
	const struct bt_gatt_attr *stpt_attr, void *vpt_buf, uint16_t u16_length,
	uint16_t u16_offset);

/******************************************************************************/
/*                                                                            */
/*                             PRIVATE VARIABLES                              */
/*                                                                            */
/******************************************************************************/
// /**
//  * @var           su8ar_FOTACtrl
//  * @brief         An array mapped with FOTA Control characteristic. All the
//  * 					Control Packets received from the peer device will be stored in
//  * 					this buffer by the write callback of FOTA Control characteristic.
//  */
// static uint8_t su8ar_FOTACtrl[MAX_MTU_SIZE_WITH_DLE];

/**
 * @var           sst_FOTACtrlCharCPList
 * @brief         An array mapped with FOTA Control characteristic. All the
 * 					Control Packets received from the peer device will be stored in
 * 					this structure after parsing it in the write callback of FOTA
 *                Control characteristic.
 */
static CPList_T sst_FOTACtrlCharCPList = { 0 };

/**
 * @var           sst_FOTACtrlCharCtx
 * @brief         A structure mapped with FOTA Control characteristic. It contains
 * 					pointer to the data buffer (su8ar_FOTACtrl) and its maximum and
 * 					current length.
 */
static CharCtx_T sst_FOTACtrlCharCtx = {
	.stpt_data = &sst_FOTACtrlCharCPList,
	.u16_maxLen = MAX_MTU_SIZE_WITH_DLE,
	.u16_curLen = 0U
};

// /**
//  * @var           sst_FOTACtrlCharCtx
//  * @brief         A structure mapped with FOTA Control characteristic. It contains
//  * 					pointer to the data buffer (su8ar_FOTACtrl) and its maximum and
//  * 					current length.
//  */
// static CharCtx_T sst_FOTACtrlCharCtx = {
// 	.u8pt_data = su8ar_FOTACtrl,
// 	.u16_maxLen = sizeof(su8ar_FOTACtrl),
// 	.u16_curLen = 0U
// };

/**
 * @var           su8ar_FOTADataXfer
 * @brief         An array mapped with FOTA Data Transfer characteristic. All the
 * 					Data Packets received from the peer device will be stored in
 * 					this buffer by the write callback of FOTA Data Transfer characteristic.
 */
static uint8_t su8ar_FOTADataXfer[MAX_MTU_SIZE_WITH_DLE];

/**
 * @var           sst_FOTADataXferCharCtx
 * @brief         A structure mapped with FOTA Data Transfer characteristic. It contains
 * 					pointer to the data buffer (su8ar_FOTADataXfer) and its maximum and
 * 					current length.
 */
static CharCtx_T sst_FOTADataXferCharCtx = {
	.stpt_data = su8ar_FOTADataXfer,
	.u16_maxLen = sizeof(su8ar_FOTADataXfer),
	.u16_curLen = 0U
};

/**
 * @var           su8ar_FOTAStatus
 * @brief         An array mapped with FOTA Status characteristic. All the status
 * 					inquired by FOTA Status characteristic will be stored in this buffer.
 */
static uint8_t su8ar_FOTAStatus[4];

/**
 * @var           sst_FOTAStatusCharCtx
 * @brief         A structure mapped with FOTA Status characteristic. It contains
 * 					pointer to the data buffer (su8ar_FOTAStatus) and its maximum and
 * 					current length.
 */
static CharCtx_T sst_FOTAStatusCharCtx = {
	.stpt_data = su8ar_FOTAStatus,
	.u16_maxLen = sizeof(su8ar_FOTAStatus),
	.u16_curLen = 0U
};

/**
 * @var           su8ar_FOTAProgress
 * @brief         An array mapped with FOTA Progress characteristic. All the progress
 * 					inquired by FOTA Progress characteristic will be stored in this buffer.
 */
static uint8_t su8ar_FOTAProgress[4];

/**
 * @var           sst_FOTAProgressCharCtx
 * @brief         A structure mapped with FOTA Progress characteristic. It contains
 * 					pointer to the data buffer (su8ar_FOTAProgress) and its maximum and
 * 					current length.
 */
static CharCtx_T sst_FOTAProgressCharCtx = {
	.stpt_data = su8ar_FOTAProgress,
	.u16_maxLen = sizeof(su8ar_FOTAProgress),
	.u16_curLen = 0U
};

/******************************************************************************/
/*                                                                            */
/*                                 STRUCTURES                                 */
/*                                                                            */
/******************************************************************************/
/**
 * @var           gstar_FOTASvc
 * @brief         FOTA service instance. Creates a structure of bt_gatt_attr type.
 *                It statically define and register this GATT service.
 */
BT_GATT_SERVICE_DEFINE(gstar_FOTASvc,
	// Primary service declaration with FOTA service UUID
	BT_GATT_PRIMARY_SERVICE(
		// UUID
		BT_UUID_FOTA_SERVICE
	),
	// Characteristic declaration for FOTA control
	BT_GATT_CHARACTERISTIC(
		// UUID
		BT_UUID_FOTA_CONTROL_CHAR,
		// Properties - Write
		BT_GATT_CHRC_WRITE,
		// Permissions - Write
		BT_GATT_PERM_WRITE,
		// Read callback - NULL
		NULL,
		// Write callback - st_FOTACtrlWrite
		st_FOTACtrlWrite,
		// User data - sst_FOTACtrlCharCtx
		&sst_FOTACtrlCharCtx
	),
	BT_GATT_CUD(
		"FOTA Control",
		BT_GATT_PERM_READ
	),
	// Characteristic declaration for FOTA data transfer
	BT_GATT_CHARACTERISTIC(
		// UUID
		BT_UUID_FOTA_DATA_TRANSFER_CHAR,
		// Properties - Write
		BT_GATT_CHRC_WRITE,
		// Permissions - Write
		BT_GATT_PERM_WRITE,
		// Read callback - NULL
		NULL,
		// Write callback - st_FOTADataTransferWrite
		st_FOTADataTransferWrite,
		// User data - sst_FOTADataXferCharCtx
		&sst_FOTADataXferCharCtx
	),
	BT_GATT_CUD(
		"FOTA Data Transfer",
		BT_GATT_PERM_READ
	),
	// Characteristic declaration for FOTA Status
	BT_GATT_CHARACTERISTIC(
		// UUID
		BT_UUID_FOTA_STATUS_CHAR,
		// Properties - Read
		BT_GATT_CHRC_READ,
		// Permissions - Read
		BT_GATT_PERM_READ,
		// Read callback - st_FOTAStsRead
		st_FOTAStsRead,
		// Write callback - NULL
		NULL,
		// User data - sst_FOTAStatusCharCtx
		&sst_FOTAStatusCharCtx
	),
	BT_GATT_CUD(
		"FOTA Status",
		BT_GATT_PERM_READ
	),
	// Client Characteristic Configuration Descriptor for FOTA Status characteristic
	BT_GATT_CCC(
		// Configuration changed callback - NULL
		NULL,
		// Permissions - Read and Write
		BT_GATT_PERM_READ | BT_GATT_PERM_WRITE
	),
	// Characteristic declaration for FOTA Progress
	BT_GATT_CHARACTERISTIC(
		// UUID
		BT_UUID_FOTA_PROGRESS_CHAR,
		// Properties - Read
		BT_GATT_CHRC_READ,
		// Permissions - Read
		BT_GATT_PERM_READ,
		// Read callback - st_FOTAProgressRead
		st_FOTAProgressRead,
		// Write callback - NULL
		NULL,
		// User data - sst_FOTAProgressCharCtx
		&sst_FOTAProgressCharCtx
	),
	BT_GATT_CUD(
		"FOTA Progress",
		BT_GATT_PERM_READ
	),
	BT_GATT_CCC(
		// Configuration changed callback - NULL
		NULL,
		// Permissions - Read and Write
		BT_GATT_PERM_READ | BT_GATT_PERM_WRITE
	),
);

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
/*                              EXTERN FUNCTIONS                              */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*                                                                            */
/*                        PRIVATE FUNCTION DEFINITIONS                        */
/*                                                                            */
/******************************************************************************/
/**
 * @private       st_FOTACtrlWrite
 * @brief         <Function details>.
 * @param[inout]  stpt_connHandle Connection handle.
 * @param[in]    	stpt_attr The attribute being written to.
 * @param[in]    	vpt_buf The buffer containing the data being written.
 * @param[in]    	u16_length The length of the data being written.
 * @param[in]    	u16_offset The offset at which the data is being written.
 * @param[in]    	u8_flags Write flags, see @ref bt_gatt_attr_write_flag.
 * @return        Either BT_GATT_ERR() or the number of bytes written.
 */
static ssize_t st_FOTACtrlWrite(struct bt_conn *stpt_connHandle,
	const struct bt_gatt_attr *stpt_attr, const void *vpt_buf, uint16_t u16_length,
	uint16_t u16_offset, uint8_t u8_flags)
{
	ssize_t t_retVal = 0;
	// Take a pointer to the user data of this attribute and consider it as a byte array,
	//	as we are expecting byte array input for this characteristic.
	CharCtx_T *stpt_charCtx = stpt_attr->user_data;
	// Create a temporary CP list structure to hold the parsed Control Packets from the incoming data.
	CPList_T st_CPList;
	FOTAEvent_T st_FOTAEvent = { 0 };

	// Perform all the basic checks for the incoming write request parameters before processing the data.
	// Check if the connection handle, attribute pointer, input buffer pointer or
	// characteristic context pointer is NULL.
	if (!stpt_connHandle || !stpt_attr || !vpt_buf || !stpt_charCtx)
	{
		t_retVal = BT_GATT_ERR(BT_ATT_ERR_UNLIKELY);
		LOG_INF("Invalid parameters in FOTA Control characteristic write. "
			"Connection Handle: %p, Attribute: %p, Buffer: %p", stpt_connHandle, stpt_attr, vpt_buf);
	}
	// Check if the write operation is not starting at offset 0, as we do not support
	//	offset writes for this characteristic.
	else if (u16_offset != 0U)
	{
		t_retVal = BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
		LOG_INF("Offset write not supported for FOTA Control characteristic. "
			"Received u16_offset: %u", u16_offset);
	}
	// We want to keep writing to this characteristic with variable length data,
	// so we will only check if the length of incoming data is more than the buffer size.
	else if (u16_length > stpt_charCtx->u16_maxLen)
	{
		t_retVal = BT_GATT_ERR(BT_ATT_ERR_INVALID_ATTRIBUTE_LEN);
		LOG_INF("FOTA Control characteristic write length exceeds buffer size. "
			"Received length: %u", u16_length);
	}

	// Check if there is not any error in parameters
	if (0 == t_retVal)
	{
		// Actual code
		// Writing to this characteristic will considered as an input to FOTA state machine.

		LOG_INF("Received data from FOTA Control characteristic.");

		// // Copy the incoming data to the mapped data buffer of this characteristic.
		// // This will be used as input to FOTA FSM.
		// memcpy(stpt_charCtx->u8pt_data, vpt_buf, u16_length);

		// Update the current length of the data in the buffer.
		stpt_charCtx->u16_curLen = u16_length;

		// // SDC:
		// // Added just for testing purposes - to simulate the presence of Control Packets
      // stpt_charCtx->stpt_data.star_CPBlocks[0].u8_CPBlockLength = 5;
      // stpt_charCtx->stpt_data.star_CPBlocks[0].e_CPType = eCPT_FOTA_START;
      // stpt_charCtx->stpt_data.star_CPBlocks[0].u8ar_CPData[0] = 0xAA;
      // stpt_charCtx->stpt_data.star_CPBlocks[0].u8ar_CPData[1] = 0xBB;
      // stpt_charCtx->stpt_data.star_CPBlocks[0].u8ar_CPData[2] = 0xCC;
      // stpt_charCtx->stpt_data.star_CPBlocks[0].u8ar_CPData[3] = 0xDD;
		// stpt_charCtx->u8pt_data[0] = 5;
		// stpt_charCtx->u8pt_data[1] = 1;
		// stpt_charCtx->u8pt_data[2] = 0xAA;
		// stpt_charCtx->u8pt_data[3] = 0xBB;
		// stpt_charCtx->u8pt_data[4] = 0xCC;
		// stpt_charCtx->u8pt_data[5] = 0xDD;

		// Check if parsing the Control Packets from the incoming data and
		// populating the temporary CP list structure completed successfully.
		// if (eTP_OK == ge_TP_ParseCPList(stpt_charCtx->u8pt_data, stpt_charCtx->u16_curLen, &st_CPList))
      if (eTP_OK == ge_TP_ParseCPList((uint8_t *)vpt_buf, stpt_charCtx->u16_curLen, stpt_charCtx->stpt_data))
		{
			// Check if there is only 1 CP block in the parsed CP list
			// if (1 == st_CPList.u8_count)
         if (1 == stpt_charCtx->stpt_data->u8_count)
			{
				// Populate the event
				// // Check if the CP type of the single CP block in the parsed CP list is FOTA Start
				// if (st_CPList.star_CPBlocks[0].e_CPType == eCPT_FOTA_START)
				// {
				// 	st_FOTAEvent.e_evt = eFE_FOTA_START;
				// 	memcpy(&st_FOTAEvent.u_FOTAEvents.st_FOTAStart, \
				// 		st_CPList.star_CPBlocks[0].u8ar_CPData, \
				// 		st_CPList.star_CPBlocks[0].u8_CPBlockLength);
				// }
				// Check if the CP type of the single CP block in the parsed CP list is FOTA Start
				if (stpt_charCtx->stpt_data->star_CPBlocks[0].e_CPType == eCPT_FOTA_START)
				{
					st_FOTAEvent.e_evt = eFE_FOTA_START;
					memcpy(&st_FOTAEvent.u_FOTAEvents.st_FOTAStart, \
						stpt_charCtx->stpt_data->star_CPBlocks[0].u8ar_CPData, \
						stpt_charCtx->stpt_data->star_CPBlocks[0].u8_CPBlockLength);
				}
				// // Check if the CP type of the single CP block in the parsed CP list is Metadata
				// else if (st_CPList.star_CPBlocks[0].e_CPType == eCPT_METADATA)
				// {
				// 	st_FOTAEvent.e_evt = eFE_METADATA;
				// 	memcpy(&st_FOTAEvent.u_FOTAEvents.st_metadata, \
				// 		st_CPList.star_CPBlocks[0].u8ar_CPData, \
				// 		st_CPList.star_CPBlocks[0].u8_CPBlockLength);
				// }
				// Check if the CP type of the single CP block in the parsed CP list is Metadata
				else if (stpt_charCtx->stpt_data->star_CPBlocks[0].e_CPType == eCPT_METADATA)
				{
					st_FOTAEvent.e_evt = eFE_METADATA;
					memcpy(&st_FOTAEvent.u_FOTAEvents.st_metadata, \
						stpt_charCtx->stpt_data->star_CPBlocks[0].u8ar_CPData, \
						stpt_charCtx->stpt_data->star_CPBlocks[0].u8_CPBlockLength);
				}
				// // Check if the CP type of the single CP block in the parsed CP list is Manifest
				// else if (st_CPList.star_CPBlocks[0].e_CPType == eCPT_MANIFEST)
				// {
				// 	st_FOTAEvent.e_evt = eFE_MANIFEST;
				// 	memcpy(&st_FOTAEvent.u_FOTAEvents.st_manifest, \
				// 		st_CPList.star_CPBlocks[0].u8ar_CPData, \
				// 		st_CPList.star_CPBlocks[0].u8_CPBlockLength);
				// }
				// Check if the CP type of the single CP block in the parsed CP list is Manifest
				else if (stpt_charCtx->stpt_data->star_CPBlocks[0].e_CPType == eCPT_MANIFEST)
				{
					st_FOTAEvent.e_evt = eFE_MANIFEST;
					memcpy(&st_FOTAEvent.u_FOTAEvents.st_manifest, \
						stpt_charCtx->stpt_data->star_CPBlocks[0].u8ar_CPData, \
						stpt_charCtx->stpt_data->star_CPBlocks[0].u8_CPBlockLength);
				}
				// // Check if the CP type of the single CP block in the parsed CP list is FOTA Data
				// else if (st_CPList.star_CPBlocks[0].e_CPType == eCPT_FOTA_DATA)
				// {
				// 	st_FOTAEvent.e_evt = eFE_FOTA_DATA;
				// 	memcpy(&st_FOTAEvent.u_FOTAEvents.st_FOTAData, \
				// 		st_CPList.star_CPBlocks[0].u8ar_CPData, \
				// 		st_CPList.star_CPBlocks[0].u8_CPBlockLength);
				// }
				// Check if the CP type of the single CP block in the parsed CP list is FOTA Data
				else if (stpt_charCtx->stpt_data->star_CPBlocks[0].e_CPType == eCPT_FOTA_DATA)
				{
					st_FOTAEvent.e_evt = eFE_FOTA_DATA;
					memcpy(&st_FOTAEvent.u_FOTAEvents.st_FOTAData, \
						stpt_charCtx->stpt_data->star_CPBlocks[0].u8ar_CPData, \
						stpt_charCtx->stpt_data->star_CPBlocks[0].u8_CPBlockLength);
				}
				else
				{
					LOG_INF("Unsupported Control Packet type received in FOTA Control characteristic data. "
						"Received CP type: %u", st_CPList.star_CPBlocks[0].e_CPType);
				}

				// Publish this event
				if (0 == zbus_chan_pub(&FOTAEventChannel, &st_FOTAEvent, K_NO_WAIT))
				{
					LOG_INF("Published FOTA Event to ZBUS channel successfully.");
				}
				else
				{
					LOG_INF("Failed to publish FOTA Event to ZBUS channel.");
				}
			}
			else
			{
				LOG_INF("Invalid number of CP blocks in FOTA Control characteristic data. "
					"Parsed CP block count: %u", st_CPList.u8_count);
			}
		}
		else
		{
			LOG_INF("Failed to parse Control Packets from FOTA Control characteristic data.");
		}

		// Finally, after successful completion,  update the buffer with incoming data
		// and return the length of data written.
		t_retVal = u16_length;
	}

	return t_retVal;
}

/**
 * @private       st_FOTADataTransferWrite
 * @brief         <Function details>.
 * @param[inout]  stpt_connHandle Connection handle.
 * @param[in]    	stpt_attr The attribute being written to.
 * @param[in]    	vpt_buf The buffer containing the data being written.
 * @param[in]    	u16_length The length of the data being written.
 * @param[in]    	u16_offset The offset at which the data is being written.
 * @param[in]    	u8_flags Write flags, see @ref bt_gatt_attr_write_flag.
 * @return        Either BT_GATT_ERR() or the number of bytes written.
 */
static ssize_t st_FOTADataTransferWrite(struct bt_conn *stpt_connHandle,
	const struct bt_gatt_attr *stpt_attr, const void *vpt_buf, uint16_t u16_length,
	uint16_t u16_offset, uint8_t u8_flags)
{
#if 0
	ssize_t t_retVal = 0;
	// Take a pointer to the user data of this attribute and consider it as a byte array,
	//	as we are expecting byte array input for this characteristic.
	CharCtx_T *stpt_charCtx = stpt_attr->user_data;

	// Perform all the basic checks for the incoming write request parameters before processing the data.
	// Check if the connection handle, attribute pointer, input buffer pointer or
	// characteristic context pointer is NULL.
	if (!stpt_connHandle || !stpt_attr || !vpt_buf || !stpt_charCtx)
	{
		t_retVal = BT_GATT_ERR(BT_ATT_ERR_UNLIKELY);
		LOG_INF("Invalid parameters in FOTA Data Transfer characteristic write. "
			"Connection Handle: %p, Attribute: %p, Buffer: %p", stpt_connHandle, stpt_attr, vpt_buf);
	}
	// Check if the write operation is not starting at offset 0, as we do not support
	//	offset writes for this characteristic.
	else if (u16_offset != 0U)
	{
		t_retVal = BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
		LOG_INF("Offset write not supported for FOTA Data Transfer characteristic. "
			"Received u16_offset: %u", u16_offset);
	}
	// We want to keep writing to this characteristic with variable length data,
	// so we will only check if the length of incoming data is more than the buffer size.
	else if (u16_length > stpt_charCtx->u16_maxLen)
	{
		t_retVal = BT_GATT_ERR(BT_ATT_ERR_INVALID_ATTRIBUTE_LEN);
		LOG_INF("FOTA Data Transfer characteristic write length exceeds buffer size. "
			"Received length: %u", u16_length);
	}

	// Check if there is not any error in parameters
	if (0 == t_retVal)
	{
		// Actual code
		// Writing to this characteristic will considered as an input to FOTA state machine.

		LOG_INF("Received data from FOTA Data Transfer characteristic.");

		// Copy the incoming data to the mapped data buffer of this characteristic.
		// This will be used as input to FOTA FSM.
		memcpy(stpt_charCtx->u8pt_data, vpt_buf, u16_length);

		// Update the current length of the data in the buffer.
		stpt_charCtx->u16_curLen = u16_length;

		// Push this to FSM input queue for processing. This is just a placeholder,
		// actual implementation may vary.

		// Finally, after successful completion,  update the buffer with incoming data
		// and return the length of data written.
		t_retVal = u16_length;
	}

	return t_retVal;
#endif // 0
}

/* ================= Read Handlers ================= */
/**
 * @private       st_FOTAStsRead
 * @brief         <Function details>.
 * @param[inout]  stpt_connHandle Connection handle.
 * @param[in]    	stpt_attr The attribute being read from.
 * @param[in]    	vpt_buf The buffer to be filled with the data being read.
 * @param[in]    	u16_length The length of the buffer.
 * @param[in]    	u16_offset The offset at which the data is being read.
 * @return        Either BT_GATT_ERR() or the number of bytes written.
 */
static ssize_t st_FOTAStsRead(struct bt_conn *stpt_connHandle,
	const struct bt_gatt_attr *stpt_attr, void *vpt_buf, uint16_t u16_length,
	uint16_t u16_offset)
{
#if 0
	ssize_t t_retVal = 0;
	// Take a pointer to the user data of this attribute and consider it as a byte array,
	//	as we are expecting byte array input for this characteristic.
	CharCtx_T *stpt_charCtx = stpt_attr->user_data;

	// Perform all the basic checks for the incoming read request parameters before processing the data.
	// Check if the connection handle, attribute pointer, input buffer pointer or
	// characteristic context pointer is NULL.
	if (!stpt_connHandle || !stpt_attr || !vpt_buf || !stpt_charCtx)
	{
		t_retVal = BT_GATT_ERR(BT_ATT_ERR_UNLIKELY);
		LOG_INF("Invalid parameters in FOTA Status characteristic read. "
			"Connection Handle: %p, Attribute: %p, Buffer: %p", stpt_connHandle, stpt_attr, vpt_buf);
	}
	// Check if the read operation is not starting at offset 0, as we do not support
	//	offset reads for this characteristic.
	else if (u16_offset != 0U)
	{
		t_retVal = BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
		LOG_INF("Offset read not supported for FOTA Status characteristic. "
			"Received u16_offset: %u", u16_offset);
	}

	// Check if there is not any error in parameters
	if (0 == t_retVal)
	{
		// Read the local attribute value from the local database.
		t_retVal = bt_gatt_attr_read(stpt_connHandle, stpt_attr, vpt_buf, u16_length,
			u16_offset, stpt_charCtx->u8pt_data, stpt_charCtx->u16_maxLen);
	}

	return t_retVal;
#endif // 0
}

/**
 * @private       st_FOTAProgressRead
 * @brief         <Function details>.
 * @param[inout]  stpt_connHandle Connection handle.
 * @param[in]    	stpt_attr The attribute being read from.
 * @param[in]    	vpt_buf The buffer to be filled with the data being read.
 * @param[in]    	u16_length The length of the buffer.
 * @param[in]    	u16_offset The offset at which the data is being read.
 * @return        Either BT_GATT_ERR() or the number of bytes written.
 */
static ssize_t st_FOTAProgressRead(struct bt_conn *stpt_connHandle,
	const struct bt_gatt_attr *stpt_attr, void *vpt_buf, uint16_t u16_length,
	uint16_t u16_offset)
{
#if 0
	ssize_t t_retVal = 0;
	// Take a pointer to the user data of this attribute and consider it as a byte array,
	//	as we are expecting byte array input for this characteristic.
	CharCtx_T *stpt_charCtx = stpt_attr->user_data;

	// Perform all the basic checks for the incoming read request parameters before processing the data.
	// Check if the connection handle, attribute pointer, input buffer pointer or
	// characteristic context pointer is NULL.
	if (!stpt_connHandle || !stpt_attr || !vpt_buf || !stpt_charCtx)
	{
		t_retVal = BT_GATT_ERR(BT_ATT_ERR_UNLIKELY);
		LOG_INF("Invalid parameters in FOTA Progress characteristic read. "
			"Connection Handle: %p, Attribute: %p, Buffer: %p", stpt_connHandle, stpt_attr, vpt_buf);
	}
	// Check if the read operation is not starting at offset 0, as we do not support
	//	offset reads for this characteristic.
	else if (u16_offset != 0U)
	{
		t_retVal = BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
		LOG_INF("Offset read not supported for FOTA Progress characteristic. "
			"Received u16_offset: %u", u16_offset);
	}

	// Check if there is not any error in parameters
	if (0 == t_retVal)
	{
		// Read the local attribute value from the local database.
		t_retVal = bt_gatt_attr_read(stpt_connHandle, stpt_attr, vpt_buf, u16_length,
			u16_offset, stpt_charCtx->u8pt_data, stpt_charCtx->u16_maxLen);
	}

	return t_retVal;
#endif // 0
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
