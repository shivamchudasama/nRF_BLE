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
 * @def           <Define name>
 * @brief         <Define details>.
 */

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
static ssize_t st_FOTACtrlWrite(struct bt_conn *conn,
	const struct bt_gatt_attr *attr,
	const void *buf, uint16_t len,
	uint16_t offset, uint8_t flags);
static ssize_t st_ECUIdWrite(struct bt_conn *conn,
	const struct bt_gatt_attr *attr,
	const void *buf, uint16_t len,
	uint16_t offset, uint8_t flags);
static ssize_t st_ImageMetadataWrite(struct bt_conn *conn,
	const struct bt_gatt_attr *attr,
	const void *buf, uint16_t len,
	uint16_t offset, uint8_t flags);
static ssize_t st_ActiveBlockAddrWrite(struct bt_conn *conn,
	const struct bt_gatt_attr *attr,
	const void *buf, uint16_t len,
	uint16_t offset, uint8_t flags);
static ssize_t st_FWDataWrite(struct bt_conn *conn,
	const struct bt_gatt_attr *attr,
	const void *buf, uint16_t len,
	uint16_t offset, uint8_t flags);
static ssize_t st_FOTAStsRead(struct bt_conn *conn,
	const struct bt_gatt_attr *attr,
	void *buf, uint16_t len,
	uint16_t offset);
static ssize_t st_ResumeOffsetRead(struct bt_conn *conn,
	const struct bt_gatt_attr *attr,
	void *buf, uint16_t len,
	uint16_t offset);

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
		BT_UUID_FOTA_CONTROL,
		// Properties - Write
		BT_GATT_CHRC_WRITE,
		// Permissions - Write
		BT_GATT_PERM_WRITE,
		// Read callback - NULL
		NULL, 
		// Write callback - st_FOTACtrlWrite
		st_FOTACtrlWrite, 
		// User data - NULL
		NULL
	),
	// Characteristic declaration for ECU ID
	BT_GATT_CHARACTERISTIC(
		// UUID
		BT_UUID_ECU_ID,
		// Properties - Write
		BT_GATT_CHRC_WRITE,
		// Permissions - Write
		BT_GATT_PERM_WRITE,
		// Read callback - NULL
		NULL,
		// Write callback - st_ECUIdWrite
		st_ECUIdWrite, 
		// User data - NULL
		NULL
	),
	// Characteristic declaration for Image Metadata
	BT_GATT_CHARACTERISTIC(
		// UUID
		BT_UUID_IMAGE_METADATA,
		// Properties - Write
		BT_GATT_CHRC_WRITE,
		// Permissions - Write
		BT_GATT_PERM_WRITE,
		// Read callback - NULL
		NULL,
		// Write callback - st_ImageMetadataWrite
		st_ImageMetadataWrite, 
		// User data - NULL
		NULL
	),
	// Characteristic declaration for Active Block Address
	BT_GATT_CHARACTERISTIC(
		// UUID
		BT_UUID_ACTIVE_BLOCK_ADDR,
		// Properties - Write
		BT_GATT_CHRC_WRITE,
		// Permissions - Write
		BT_GATT_PERM_WRITE,
		// Read callback - NULL
		NULL,
		// Write callback - st_ActiveBlockAddrWrite
		st_ActiveBlockAddrWrite,
		// User data - NULL
		NULL
	),
	// Characteristic declaration for Firmware Data
	BT_GATT_CHARACTERISTIC(
		// UUID
		BT_UUID_FIRMWARE_DATA,
		// Properties - Write without response
		BT_GATT_CHRC_WRITE_WITHOUT_RESP,
		// Permissions - Write
		BT_GATT_PERM_WRITE,
		// Read callback - NULL
		NULL, 
		// Write callback - st_FWDataWrite
		st_FWDataWrite,
		// User data - NULL
		NULL
	),
	// Characteristic declaration for FOTA Status
	BT_GATT_CHARACTERISTIC(
		// UUID
		BT_UUID_FOTA_STATUS,
		// Properties - Read and Notify
		BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
		// Permissions - Read
		BT_GATT_PERM_READ,
		// Read callback - st_FOTAStsRead
		st_FOTAStsRead,
		// Write callback - NULL
		NULL,
		// User data - NULL
		NULL
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
		BT_UUID_FOTA_PROGRESS,
		// Properties - Notify
		BT_GATT_CHRC_NOTIFY,
		// Permissions - None
		BT_GATT_PERM_NONE,
		// Read callback - NULL
		NULL,
		// Write callback - NULL
		NULL, 
		// User data - NULL
		NULL
	),
	BT_GATT_CCC(
		// Configuration changed callback - NULL
		NULL, 
		// Permissions - Read and Write
		BT_GATT_PERM_READ | BT_GATT_PERM_WRITE
	),
	// Characteristic declaration for Resume Offset
	BT_GATT_CHARACTERISTIC(
		// UUID
		BT_UUID_RESUME_OFFSET,
		// Properties - Read
		BT_GATT_CHRC_READ,
		// Permissions - Read
		BT_GATT_PERM_READ,
		// Read callback - st_ResumeOffsetRead
		st_ResumeOffsetRead, 
		// Write callback - NULL
		NULL, 
		// User data - NULL
		NULL
	)
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
/*                             PRIVATE VARIABLES                              */
/*                                                                            */
/******************************************************************************/
/**
 * @var           <Variable name>
 * @brief         <Variable details>.
 */
/**
 * @var           su8ar_FOTACtrl
 * @brief         <Variable details>.
 */
static uint8_t su8ar_FOTACtrl[4];

/**
 * @var           su8ar_ECUId
 * @brief         <Variable details>.
 */
static uint8_t su8ar_ECUId[64];

/**
 * @var           su8ar_imageMetadata
 * @brief         <Variable details>.
 */
static uint8_t su8ar_imageMetadata[12];

/**
 * @var           su32_activeBlockAddr
 * @brief         <Variable details>.
 */
static uint32_t su32_activeBlockAddr;

/**
 * @var           su8ar_firmwareData
 * @brief         <Variable details>.
 */
static uint8_t su8ar_firmwareData[244];

/**
 * @var           su8ar_FOTAStatus
 * @brief         <Variable details>.
 */
static uint8_t su8ar_FOTAStatus[4];

/**
 * @var           su8ar_FOTAProgress
 * @brief         <Variable details>.
 */
static uint8_t su8ar_FOTAProgress[12];

/**
 * @var           su8ar_resumeOffset
 * @brief         <Variable details>.
 */
static uint8_t su8ar_resumeOffset[8];

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
 * @param[in]     <Input parameter details>.
 * @param[out]    <Output parameter details>.
 * @param[inout]  <Input-Output parameter details>.
 * @return        <Return details>.
 */
static ssize_t st_FOTACtrlWrite(struct bt_conn *conn,
	const struct bt_gatt_attr *attr,
	const void *buf, uint16_t len,
	uint16_t offset, uint8_t flags)
{
	if (len != sizeof(su8ar_FOTACtrl)) {
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_ATTRIBUTE_LEN);
	}
	memcpy(su8ar_FOTACtrl, buf, len);
	return len;
}

/**
 * @private       st_ECUIdWrite
 * @brief         <Function details>.
 * @param[in]     <Input parameter details>.
 * @param[out]    <Output parameter details>.
 * @param[inout]  <Input-Output parameter details>.
 * @return        <Return details>.
 */
static ssize_t st_ECUIdWrite(struct bt_conn *conn,
	const struct bt_gatt_attr *attr,
	const void *buf, uint16_t len,
	uint16_t offset, uint8_t flags)
{
	if (len > sizeof(su8ar_ECUId)) {
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_ATTRIBUTE_LEN);
	}
	memcpy(su8ar_ECUId, buf, len);
	return len;
}

/**
 * @private       st_ImageMetadataWrite
 * @brief         <Function details>.
 * @param[in]     <Input parameter details>.
 * @param[out]    <Output parameter details>.
 * @param[inout]  <Input-Output parameter details>.
 * @return        <Return details>.
 */
static ssize_t st_ImageMetadataWrite(struct bt_conn *conn,
	const struct bt_gatt_attr *attr,
	const void *buf, uint16_t len,
	uint16_t offset, uint8_t flags)
{
	if (len != sizeof(su8ar_imageMetadata)) {
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_ATTRIBUTE_LEN);
	}
	memcpy(su8ar_imageMetadata, buf, len);
	return len;
}

/**
 * @private       st_ActiveBlockAddrWrite
 * @brief         <Function details>.
 * @param[in]     <Input parameter details>.
 * @param[out]    <Output parameter details>.
 * @param[inout]  <Input-Output parameter details>.
 * @return        <Return details>.
 */
static ssize_t st_ActiveBlockAddrWrite(struct bt_conn *conn,
	const struct bt_gatt_attr *attr,
	const void *buf, uint16_t len,
	uint16_t offset, uint8_t flags)
{
	if (len != sizeof(uint32_t)) {
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_ATTRIBUTE_LEN);
	}
	memcpy(&su32_activeBlockAddr, buf, len);
	return len;
}

/**
 * @private       st_FWDataWrite
 * @brief         <Function details>.
 * @param[in]     <Input parameter details>.
 * @param[out]    <Output parameter details>.
 * @param[inout]  <Input-Output parameter details>.
 * @return        <Return details>.
 */
static ssize_t st_FWDataWrite(struct bt_conn *conn,
	const struct bt_gatt_attr *attr,
	const void *buf, uint16_t len,
	uint16_t offset, uint8_t flags)
{
	if (len > sizeof(su8ar_firmwareData)) {
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_ATTRIBUTE_LEN);
	}
	memcpy(su8ar_firmwareData, buf, len);
	/* Data handling / flash write goes here */
	return len;
}

/* ================= Read Handlers ================= */
/**
 * @private       st_FOTAStsRead
 * @brief         <Function details>.
 * @param[in]     <Input parameter details>.
 * @param[out]    <Output parameter details>.
 * @param[inout]  <Input-Output parameter details>.
 * @return        <Return details>.
 */
static ssize_t st_FOTAStsRead(struct bt_conn *conn,
	const struct bt_gatt_attr *attr,
	void *buf, uint16_t len,
	uint16_t offset)
{
	return bt_gatt_attr_read(conn, attr, buf, len, offset,
	                         su8ar_FOTAStatus,
	                         sizeof(su8ar_FOTAStatus));
}

/**
 * @private       st_ResumeOffsetRead
 * @brief         <Function details>.
 * @param[in]     <Input parameter details>.
 * @param[out]    <Output parameter details>.
 * @param[inout]  <Input-Output parameter details>.
 * @return        <Return details>.
 */
static ssize_t st_ResumeOffsetRead(struct bt_conn *conn,
	const struct bt_gatt_attr *attr,
	void *buf, uint16_t len,
	uint16_t offset)
{
	return bt_gatt_attr_read(conn, attr, buf, len, offset,
	                         su8ar_resumeOffset,
	                         sizeof(su8ar_resumeOffset));
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
