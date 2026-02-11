/**
 * @file          FOTA_GATT.h
 * @brief         Header file containing GATT database for FOTA service.
 * @date          10/02/2026
 * @author        Shivam Chudasama [SC]
 * @copyright     Bajaj Auto Technology Limited (BATL)
 */

#ifndef _FOTA_GATT_H
#define _FOTA_GATT_H

/******************************************************************************/
/*                                                                            */
/*                                  INCLUDES                                  */
/*                                                                            */
/******************************************************************************/
#include <zephyr/bluetooth/gatt.h>

/******************************************************************************/
/*                                                                            */
/*                                  DEFINES                                   */
/*                                                                            */
/******************************************************************************/
/* ================= FOTA SERVICE UUID ================= */
/**
 * @def           BT_UUID_FOTA_SERVICE
 * @brief         Creating UUID structure pointer (const struct bt_uuid *) for
 * 					FOTA service.
 */
#define BT_UUID_FOTA_SERVICE                 BT_UUID_DECLARE_128( \
		                                          0xFB, 0x34, 0x9B, 0x5F, 0x80, 0x00, 0x00, 0x80, \
		                                          0x00, 0x10, 0x00, 0x00, 0x78, 0x56, 0x34, 0x12)

/* ================= CHARACTERISTIC UUIDs ============== */
/**
 * @def           BT_UUID_FOTA_CONTROL
 * @brief         Creating UUID structure pointer (const struct bt_uuid *) for
 * 					FOTA control characteristic.
 */
#define BT_UUID_FOTA_CONTROL                 BT_UUID_DECLARE_128( \
                                                0xFB, 0x34, 0x9B, 0x5F, 0x80, 0x00, 0x00, 0x80, \
	                                             0x00, 0x10, 0x00, 0x01, 0x78, 0x56, 0x34, 0x12)

/**
 * @def           BT_UUID_FOTA_ECU_ID
 * @brief         Creating UUID structure pointer (const struct bt_uuid *) for
 * 					ECU ID characteristic.
 */
#define BT_UUID_ECU_ID                       BT_UUID_DECLARE_128( \
                                                0xFB, 0x34, 0x9B, 0x5F, 0x80, 0x00, 0x00, 0x80, \
	                                             0x00, 0x10, 0x00, 0x02, 0x78, 0x56, 0x34, 0x12)

/**
 * @def           BT_UUID_FOTA_IMAGE_METADATA
 * @brief         Creating UUID structure pointer (const struct bt_uuid *) for
 * 					FOTA image metadata characteristic.
 */
#define BT_UUID_IMAGE_METADATA               BT_UUID_DECLARE_128( \
                                                0xFB, 0x34, 0x9B, 0x5F, 0x80, 0x00, 0x00, 0x80, \
	                                             0x00, 0x10, 0x00, 0x03, 0x78, 0x56, 0x34, 0x12)

/**
 * @def           BT_UUID_FIRMWARE_DATA
 * @brief         Creating UUID structure pointer (const struct bt_uuid *) for
 * 					FOTA firmware data characteristic.
 */
#define BT_UUID_FIRMWARE_DATA                BT_UUID_DECLARE_128( \
		                                          0xFB, 0x34, 0x9B, 0x5F, 0x80, 0x00, 0x00, 0x80, \
		                                          0x00, 0x10, 0x00, 0x04, 0x78, 0x56, 0x34, 0x12)

/**
 * @def           BT_UUID_FOTA_STATUS
 * @brief         Creating UUID structure pointer (const struct bt_uuid *) for
 * 					FOTA status characteristic.
 */
#define BT_UUID_FOTA_STATUS                  BT_UUID_DECLARE_128( \
                                                0xFB, 0x34, 0x9B, 0x5F, 0x80, 0x00, 0x00, 0x80, \
	                                             0x00, 0x10, 0x00, 0x05, 0x78, 0x56, 0x34, 0x12)

/**
 * @def           BT_UUID_FOTA_PROGRESS
 * @brief         Creating UUID structure pointer (const struct bt_uuid *) for
 * 					FOTA progress characteristic.
 */
#define BT_UUID_FOTA_PROGRESS                BT_UUID_DECLARE_128( \
                                                0xFB, 0x34, 0x9B, 0x5F, 0x80, 0x00, 0x00, 0x80, \
	                                             0x00, 0x10, 0x00, 0x06, 0x78, 0x56, 0x34, 0x12)

/**
 * @def           BT_UUID_FOTA_RESUME_OFFSET
 * @brief         Creating UUID structure pointer (const struct bt_uuid *) for
 * 					FOTA resume offset characteristic.
 */
#define BT_UUID_RESUME_OFFSET                BT_UUID_DECLARE_128( \
                                                0xFB, 0x34, 0x9B, 0x5F, 0x80, 0x00, 0x00, 0x80, \
	                                             0x00, 0x10, 0x00, 0x07, 0x78, 0x56, 0x34, 0x12)

/**
 * @def           BT_UUID_FOTA_ACTIVE_BLOCK_ADDR
 * @brief         Creating UUID structure pointer (const struct bt_uuid *) for
 * 					FOTA active block address characteristic.
 */
#define BT_UUID_ACTIVE_BLOCK_ADDR            BT_UUID_DECLARE_128( \
                                                0xFB, 0x34, 0x9B, 0x5F, 0x80, 0x00, 0x00, 0x80, \
	                                             0x00, 0x10, 0x00, 0x08, 0x78, 0x56, 0x34, 0x12)

/******************************************************************************/
/*                                                                            */
/*                                   ENUMS                                    */
/*                                                                            */
/******************************************************************************/
/**
 * @enum          <Enum name>
 * @brief         <Enum details>.
 */

/******************************************************************************/
/*                                                                            */
/*                                 STRUCTURES                                 */
/*                                                                            */
/******************************************************************************/
/**
 * @struct        <Structure name>
 * @brief         <Structure details>.
 */

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

#endif //!_FOTA_GATT_H

/**
 * Copyright(c) Bajaj Auto Technology Limited (BATL) as an unpublished work.
 * THIS SOFTWARE AND/OR MATERIAL IS THE PROPERTY OF BATL.
 * ALL USE, DISCLOSURE, AND/OR REPRODUCTION NOT SPECIFICALLY AUTHORIZED BY
 * BATL IS PROHIBITED.
 *
 * @author:Shivam Chudasama [SC]
 */
