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
#include <zephyr/bluetooth/gatt.h>
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
 * @var           su8ar_FOTACtrl
 * @brief         <Variable details>.
 */
static UInt8_T su8ar_FOTACtrl[4];

/**
 * @var           su8ar_ECUId
 * @brief         <Variable details>.
 */
static UInt8_T su8ar_ECUId[64];

/**
 * @var           su8ar_imageMetadata
 * @brief         <Variable details>.
 */
static UInt8_T su8ar_imageMetadata[12];

/**
 * @var           su32_activeBlockAddr
 * @brief         <Variable details>.
 */
static UInt32_T su32_activeBlockAddr;

/**
 * @var           su8ar_firmwareData
 * @brief         <Variable details>.
 */
static UInt8_T su8ar_firmwareData[244];

/**
 * @var           su8ar_FOTAStatus
 * @brief         <Variable details>.
 */
static UInt8_T su8ar_FOTAStatus[4];

/**
 * @var           su8ar_FOTAProgress
 * @brief         <Variable details>.
 */
static UInt8_T su8ar_FOTAProgress[12];

/**
 * @var           su8ar_resumeOffset
 * @brief         <Variable details>.
 */
static UInt8_T su8ar_resumeOffset[8];

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
 * @private       <Function name>
 * @brief         <Function details>.
 * @param[in]     <Input parameter details>.
 * @param[out]    <Output parameter details>.
 * @param[inout]  <Input-Output parameter details>.
 * @return        <Return details>.
 */
static ssize_t fota_control_write(struct bt_conn *conn,
	const struct bt_gatt_attr *attr,
	const void *buf, uint16_t len,
	uint16_t offset, uint8_t flags)
{
	if (len != sizeof(fota_control_buf)) {
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_ATTRIBUTE_LEN);
	}
	memcpy(fota_control_buf, buf, len);
	return len;
}

static ssize_t ecu_id_write(struct bt_conn *conn,
	const struct bt_gatt_attr *attr,
	const void *buf, uint16_t len,
	uint16_t offset, uint8_t flags)
{
	if (len > sizeof(ecu_id_buf)) {
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_ATTRIBUTE_LEN);
	}
	memcpy(ecu_id_buf, buf, len);
	return len;
}

static ssize_t image_metadata_write(struct bt_conn *conn,
	const struct bt_gatt_attr *attr,
	const void *buf, uint16_t len,
	uint16_t offset, uint8_t flags)
{
	if (len != sizeof(image_metadata_buf)) {
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_ATTRIBUTE_LEN);
	}
	memcpy(image_metadata_buf, buf, len);
	return len;
}

static ssize_t active_block_addr_write(struct bt_conn *conn,
	const struct bt_gatt_attr *attr,
	const void *buf, uint16_t len,
	uint16_t offset, uint8_t flags)
{
	if (len != sizeof(uint32_t)) {
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_ATTRIBUTE_LEN);
	}
	memcpy(&active_block_address, buf, len);
	return len;
}

static ssize_t firmware_data_write(struct bt_conn *conn,
	const struct bt_gatt_attr *attr,
	const void *buf, uint16_t len,
	uint16_t offset, uint8_t flags)
{
	if (len > sizeof(firmware_data_buf)) {
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_ATTRIBUTE_LEN);
	}
	memcpy(firmware_data_buf, buf, len);
	/* Data handling / flash write goes here */
	return len;
}

/* ================= Read Handlers ================= */

static ssize_t fota_status_read(struct bt_conn *conn,
	const struct bt_gatt_attr *attr,
	void *buf, uint16_t len,
	uint16_t offset)
{
	return bt_gatt_attr_read(conn, attr, buf, len, offset,
	                         fota_status_buf,
	                         sizeof(fota_status_buf));
}

static ssize_t resume_offset_read(struct bt_conn *conn,
	const struct bt_gatt_attr *attr,
	void *buf, uint16_t len,
	uint16_t offset)
{
	return bt_gatt_attr_read(conn, attr, buf, len, offset,
	                         resume_offset_buf,
	                         sizeof(resume_offset_buf));
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
