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
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/gatt.h>
#include "FOTA_GATT.h"
#include "GAP_GATT.h"
#include "FOTA_FSM.h"
#include "AppLog.h"

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
static void sv_Connected(struct bt_conn *conn, uint8_t err);
static void sv_Disconnected(struct bt_conn *conn, uint8_t reason);
static void sv_Recycled(void);

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
 * @var           su8_charVal
 * @brief         Characteristic value.
 */
static uint8_t su8_charVal = 0x00;

/**
 * @var           sstar_advData
 * @brief         Create advertising data structure array.
 */
static struct bt_data sstar_advData[] =
{
   // AD Type: Flags - general discoverable and no BR/EDR support
   BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
   // AD Type: 128-bit UUID, all listed UUIDs available
   BT_DATA_BYTES(BT_DATA_UUID128_ALL, BT_UUID_FOTA_SERVICE_VAL),
};

/**
 * @var           sstar_scanRespData
 * @brief         Create scan response data structure array.
 */
static struct bt_data sstar_scanRespData[] =
{
   // AD Type: Complete local name
   BT_DATA(BT_DATA_NAME_COMPLETE, CONFIG_BT_DEVICE_NAME,
      sizeof(CONFIG_BT_DEVICE_NAME) - 1),
};

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
   .recycled = sv_Recycled,
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

/**
 * @private       sv_Recycled
 * @brief         Callback for handling recycled connections. This function is called when a
 *                connection is recycled.
 * @return        Number of bytes written.
 */
static void sv_Recycled(void)
{
   int i_err;

   LOG_INF("Let's restart advertising after connection recycle");

   // Start advertising by setting advertisement data, scan response data,
   // and advertisement parameters.
   i_err = bt_le_adv_start(
      BT_LE_ADV_CONN_FAST_1,                 // Advertising parameters: Connectable undirected advertising
      sstar_advData,                         // Advertising data
      ARRAY_SIZE(sstar_advData),             // Length of advertising data
      sstar_scanRespData,                    // Scan response data
      ARRAY_SIZE(sstar_scanRespData)         // Length of scan response data
   );
	if (i_err)
   {
		LOG_ERR("Advertising failed to start (err %d)", i_err);
		return;
	}

   LOG_INF("Advertising restarted");
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

	// Advertise with name and custom service UUID
   // Start advertising by setting advertisement data, scan response data,
   // and advertisement parameters.
   i_err = bt_le_adv_start(
      BT_LE_ADV_CONN_FAST_1,                 // Advertising parameters: Connectable undirected advertising
      sstar_advData,                         // Advertising data
      ARRAY_SIZE(sstar_advData),             // Length of advertising data
      sstar_scanRespData,                    // Scan response data
      ARRAY_SIZE(sstar_scanRespData)         // Length of scan response data
   );
	if (i_err)
   {
		LOG_ERR("Advertising failed to start (err %d)", i_err);
		return 0;
	}

   LOG_INF("Advertising started");

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

