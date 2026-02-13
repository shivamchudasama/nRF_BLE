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
#include <zephyr/logging/log.h>

/******************************************************************************/
/*                                                                            */
/*                                  DEFINES                                   */
/*                                                                            */
/******************************************************************************/
/* ================= FOTA SERVICE UUID ================= */
/**
 * @def           BT_UUID_FOTA_SERVICE_VAL
 * @brief         128-bit value encoding for FOTA service UUID.
 */
#define BT_UUID_FOTA_SERVICE_VAL            	BT_UUID_128_ENCODE( \
                                                0x12345678, \
                                                0x0001, \
                                                0x8000, \
                                                0x0080, \
                                                0x5F9B34FB)

/**
 * @def           BT_UUID_FOTA_SERVICE
 * @brief         Creating UUID structure pointer (const struct bt_uuid *) for
 * 					FOTA service.
 */
#define BT_UUID_FOTA_SERVICE                	BT_UUID_DECLARE_128(BT_UUID_FOTA_SERVICE_VAL)

/* ================= CHARACTERISTIC UUIDs ============== */
/**
 * @def           BT_UUID_FOTA_CONTROL_CHAR_VAL
 * @brief         128-bit value encoding for FOTA control characteristic UUID.
 */
#define BT_UUID_FOTA_CONTROL_CHAR_VAL        BT_UUID_128_ENCODE( \
                                                0x12345678, \
                                                0x0002, \
                                                0x8000, \
                                                0x0080, \
                                                0x5F9B34FB)

/**
 * @def           BT_UUID_FOTA_CONTROL
 * @brief         Creating UUID structure pointer (const struct bt_uuid *) for
 * 					FOTA control characteristic.
 */
#define BT_UUID_FOTA_CONTROL_CHAR            BT_UUID_DECLARE_128(BT_UUID_FOTA_CONTROL_CHAR_VAL)

/**
 * @def           BT_UUID_FOTA_DATA_TRANSFER_CHAR_VAL
 * @brief         128-bit value encoding for FOTA data transfer characteristic UUID.
 */
#define BT_UUID_FOTA_DATA_TRANSFER_CHAR_VAL  BT_UUID_128_ENCODE( \
                                                0x12345678, \
                                                0x0003, \
                                                0x8000, \
                                                0x0080, \
                                                0x5F9B34FB)

/**
 * @def           BT_UUID_FOTA_DATA_TRANSFER_CHAR
 * @brief         Creating UUID structure pointer (const struct bt_uuid *) for
 * 					FOTA data transfer characteristic.
 */
#define BT_UUID_FOTA_DATA_TRANSFER_CHAR      BT_UUID_DECLARE_128(BT_UUID_FOTA_DATA_TRANSFER_CHAR_VAL)

/**
 * @def           BT_UUID_FOTA_STATUS_CHAR_VAL
 * @brief         128-bit value encoding for FOTA status characteristic UUID.
 */
#define BT_UUID_FOTA_STATUS_CHAR_VAL         BT_UUID_128_ENCODE( \
                                                0x12345678, \
                                                0x0004, \
                                                0x8000, \
                                                0x0080, \
                                                0x5F9B34FB)

/**
 * @def           BT_UUID_FOTA_STATUS_CHAR
 * @brief         Creating UUID structure pointer (const struct bt_uuid *) for
 * 					FOTA status characteristic.
 */
#define BT_UUID_FOTA_STATUS_CHAR             BT_UUID_DECLARE_128(BT_UUID_FOTA_STATUS_CHAR_VAL)

/**
 * @def           BT_UUID_FOTA_PROGRESS_CHAR_VAL
 * @brief         128-bit value encoding for FOTA progress characteristic UUID.
 */
#define BT_UUID_FOTA_PROGRESS_CHAR_VAL       BT_UUID_128_ENCODE( \
                                                0x12345678, \
                                                0x0005, \
                                                0x8000, \
                                                0x0080, \
                                                0x5F9B34FB)

/**
 * @def           BT_UUID_FOTA_PROGRESS_CHAR
 * @brief         Creating UUID structure pointer (const struct bt_uuid *) for
 * 					FOTA progress characteristic.
 */
#define BT_UUID_FOTA_PROGRESS_CHAR           BT_UUID_DECLARE_128(BT_UUID_FOTA_PROGRESS_CHAR_VAL)

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
