/**
 * @file          Sample_Format.c
 * @brief         Source file containing <Details>
 * @date          <Date of generating C file - DD/MM/YY>
 * @author        <Author of C file - Name [Initials]>
 * @copyright     Bajaj Auto Technology Limited (BATL)
 */

/******************************************************************************/
/*                                                                            */
/*                                  INCLUDES                                  */
/*                                                                            */
/******************************************************************************/
#include <zephyr/kernel.h>
#include <zephyr/types.h>
#include <zephyr/sys/printk.h>
#include <zephyr/logging/log.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/gatt.h>

/******************************************************************************/
/*                                                                            */
/*                                  DEFINES                                   */
/*                                                                            */
/******************************************************************************/
/**
 * @def           APP_LOG
 * @brief         Register the application log module. Module name is APP_LOG.
 *                Log level is set to INFO.
 */
LOG_MODULE_REGISTER(APP_LOG, LOG_LEVEL_INF);

/* Custom 128-bit UUIDs */
/**
 * @def           BT_UUID_CUSTOM_SERVICE_VAL
 * @brief         Register the application log module. Module name is APP_LOG.
 *                Log level is set to INFO.
 */
#define BT_UUID_CUSTOM_SERVICE_VAL           BT_UUID_128_ENCODE( \
                                                0x1d2a0001, \
                                                0x8d24, \
                                                0x4b4f, \
                                                0x9e36, \
                                                0x6b1c6a5b9f01)

/**
 * @def           BT_UUID_CUSTOM_CHAR_VAL
 * @brief         Register the application log module. Module name is APP_LOG.
 *                Log level is set to INFO.
 */
#define BT_UUID_CUSTOM_CHAR_VAL              BT_UUID_128_ENCODE( \
                                                0x1d2a0002, \
                                                0x8d24, \
                                                0x4b4f, \
                                                0x9e36, \
                                                0x6b1c6a5b9f01)

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
static ssize_t st_ReadChar(struct bt_conn *stpt_conn, const struct bt_gatt_attr *stpt_attr,
   void *vpt_buf, uint16_t u16_len, uint16_t u16_offset);
static ssize_t st_WriteChar(struct bt_conn *stpt_conn, const struct bt_gatt_attr *stpt_attr,
    const void *vpt_buf, uint16_t u16_len, uint16_t u16_offset);
static void sv_Connected(struct bt_conn *conn, uint8_t err);
static void sv_Disconnected(struct bt_conn *conn, uint8_t reason);

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
 * @var           sst_customSvcUUID
 * @brief         Custom service UUID. Initialize the variable with 128-bit UUID
 *                for custom service.
 */
static struct bt_uuid_128 sst_customSvcUUID = BT_UUID_INIT_128(BT_UUID_CUSTOM_SERVICE_VAL);

/**
 * @var           sst_customCharUUID
 * @brief         Custom characteristic UUID. Initialize the variable with 128-bit UUID
 *                for custom characteristic.
 */
static struct bt_uuid_128 sst_customCharUUID = BT_UUID_INIT_128(BT_UUID_CUSTOM_CHAR_VAL);

/**
 * @var           su8_charVal
 * @brief         Characteristic value.
 */
static uint8_t su8_charVal = 0x00;

/**
 * @var           gstar_customSvc
 * @brief         Custom service instance. It statically define and register a GATT service.
 */
BT_GATT_SERVICE_DEFINE(gstar_customSvc,
   // Primary service declaration with custom service UUID
   BT_GATT_PRIMARY_SERVICE(&sst_customSvcUUID),
   // Characteristic declaration with custom characteristic UUID, read/write/notify properties and permissions
   BT_GATT_CHARACTERISTIC(&sst_customCharUUID.uuid,
                  BT_GATT_CHRC_READ | BT_GATT_CHRC_WRITE | BT_GATT_CHRC_NOTIFY,
                  BT_GATT_PERM_READ | BT_GATT_PERM_WRITE,
                  st_ReadChar, st_WriteChar, &su8_charVal),
   // Client Characteristic Configuration Descriptor for enabling notifications
   BT_GATT_CCC(NULL, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE)
);

/******************************************************************************/
/*                                                                            */
/*                             PRIVATE VARIABLES                              */
/*                                                                            */
/******************************************************************************/
/**
 * @var           sst_connCallbacks
 * @brief         Connection callbacks for the custom service.
 */
BT_CONN_CB_DEFINE(sst_connCallbacks) = {
	.connected = sv_Connected,
	.disconnected = sv_Disconnected,
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
/**
 * @private       st_ReadChar
 * @brief         Read the value of the custom characteristic. This function is
 *                called when a read request is made for the custom characteristic.
 * @param[in]     stpt_conn Connection object for the current read operation.
 * @param[in]     stpt_attr Attribute being read.
 * @param[out]    vpt_buf Buffer to store the read value.
 * @param[in]     u16_len Length of the buffer.
 * @param[in]     u16_offset Offset for partial reads.
 * @return        Number of bytes read.
 */
static ssize_t st_ReadChar(struct bt_conn *stpt_conn, const struct bt_gatt_attr *stpt_attr,
   void *vpt_buf, uint16_t u16_len, uint16_t u16_offset)
{
   // Read the attribute value from local database
   return bt_gatt_attr_read(
      stpt_conn,                             // Connection object for the current read operation
      stpt_attr,                             // Attribute being read
      vpt_buf,                               // Buffer to store the read value
      u16_len,                               // Length of the buffer
      u16_offset,                            // Offset for partial reads
      &su8_charVal,                          // Pointer to the characteristic value to be read
      sizeof(su8_charVal)                    // Size of the characteristic value
   );
}

/**
 * @private       st_WriteChar
 * @brief         Write the value of the custom characteristic. This function is
 *                called when a write request is made for the custom characteristic.
 * @param[in]     stpt_conn Connection object for the current write operation.
 * @param[in]     stpt_attr Attribute being written.
 * @param[out]    vpt_buf Buffer to store the written value.
 * @param[in]     u16_len Length of the buffer.
 * @param[in]     u16_offset Offset for partial writes.
 * @return        Number of bytes written.
 */
static ssize_t st_WriteChar(struct bt_conn *stpt_conn, const struct bt_gatt_attr *stpt_attr,
    const void *vpt_buf, uint16_t u16_len, uint16_t u16_offset)
{
   // Check if the offset and the length of the data being written are not valid
   if (u16_offset != 0 || u16_len != sizeof(su8_charVal))
   {
      return BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
   }

   memcpy(&su8_charVal, vpt_buf, sizeof(su8_charVal));
   LOG_INF("Write: 0x%02x", su8_charVal);
   return u16_len;
}

/**
 * @private       sv_Connected
 * @brief         Callback for handling new connections. This function is called when a
 *                new connection is established.
 * @return        Number of bytes written.
 */
static void sv_Connected(struct bt_conn *conn, uint8_t err)
{
	if (err)
   {
		LOG_ERR("Connection failed (err %u)", err);
		return;
	}
	LOG_INF("Connected");
}

/**
 * @private       sv_Disconnected
 * @brief         Callback for handling disconnections. This function is called when a
 *                connection is disconnected.
 * @return        Number of bytes written.
 */
static void sv_Disconnected(struct bt_conn *conn, uint8_t reason)
{
	LOG_INF("Disconnected (reason %u)", reason);
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
int main(void)
{
   // Enable Bluetooth
   int i_err = bt_enable(NULL);
	if (i_err)
   {
		LOG_ERR("Bluetooth init failed (err %d)", i_err);
		return 0;
	}

   LOG_INF("Bluetooth initialized");

	/* Advertise with name and custom service UUID */

   // Create advertising data structure array
	const struct bt_data star_advData[] =
   {
      // AD Type: Flags - general discoverable and no BR/EDR support
		BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
      // AD Type: 128-bit UUID, all listed UUIDs available
		BT_DATA_BYTES(BT_DATA_UUID128_ALL, BT_UUID_CUSTOM_SERVICE_VAL),
	};

   // Create scan response data structure array
	const struct bt_data star_scanRespData[] =
   {
      // AD Type: Complete local name
		BT_DATA(BT_DATA_NAME_COMPLETE, CONFIG_BT_DEVICE_NAME,
			sizeof(CONFIG_BT_DEVICE_NAME) - 1),
	};

   // Start advertising by setting advertisement data, scan response data,
   // and advertisement parameters.
   i_err = bt_le_adv_start(
      BT_LE_ADV_CONN_FAST_1,                 // Advertising parameters: Connectable undirected advertising
      star_advData,                          // Advertising data
      ARRAY_SIZE(star_advData),              // Length of advertising data
      star_scanRespData,                     // Scan response data
      ARRAY_SIZE(star_scanRespData)          // Length of scan response data
   );
	if (i_err)
   {
		LOG_ERR("Advertising failed to start (err %d)", i_err);
		return 0;
	}

   LOG_INF("Advertising started");

	while (1)
   {
      // Put the current thread to sleep for 1 second
		k_sleep(K_SECONDS(1));
	}

   return 0;
}

/**
 * Copyright(c) Bajaj Auto Technology Limited (BATL) as an unpublished work.
 * THIS SOFTWARE AND/OR MATERIAL IS THE PROPERTY OF BATL.
 * ALL USE, DISCLOSURE, AND/OR REPRODUCTION NOT SPECIFICALLY AUTHORIZED BY
 * BATL IS PROHIBITED.
 *
 * @author:Shivam Chudasama [SC]
 */

