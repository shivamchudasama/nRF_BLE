/**
 * @file          ConnectionHandling.c
 * @brief         Source file containing all the connection handling related functions.
 * @date          21/02/2026
 * @author        Shivam Chudasama [SC]
 * @copyright     Bajaj Auto Technology Limited (BATL)
 */

/******************************************************************************/
/*                                                                            */
/*                                  INCLUDES                                  */
/*                                                                            */
/******************************************************************************/
#include "ConnectionHandling.h"

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
static void sv_RequestDataLenUpdate(struct bt_conn *stpt_conn);
static void sv_MTUExchangeCallback(struct bt_conn *stpt_conn, uint8_t u8_err,
   struct bt_gatt_exchange_params *stpt_exchangeParams);
static void sv_ReqMTUUpdate(struct bt_conn *stpt_conn);
static void sv_Connected(struct bt_conn *stpt_conn, uint8_t err);
static void sv_Disconnected(struct bt_conn *stpt_conn, uint8_t reason);
static void sv_Recycled(void);
static void sv_DataLengthUpdated(struct bt_conn *stpt_conn,
   struct bt_conn_le_data_len_info *stpt_info);

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

/**
 * @var           sst_connCallbacks
 * @brief         Connection callbacks for the custom service.
 */
BT_CONN_CB_DEFINE(sst_connCallbacks) = {
	.connected = sv_Connected,
	.disconnected = sv_Disconnected,
   .recycled = sv_Recycled,
   .le_data_len_updated = sv_DataLengthUpdated,
};

/**
 * @var           sstpt_currentConn
 * @brief         Pointer to current connection handle.
 */
static struct bt_conn *sstpt_currentConn;

/**
 * @var           sst_exchangeParams
 * @brief         Structure for GATT exchange parameters.
 */
static struct bt_gatt_exchange_params sst_exchangeParams;

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
 * @private       sv_RequestDataLenUpdate
 * @brief         Function requests the connection for data length update.
 * @param[in]     stpt_conn Connection handle
 * @return        None.
 */
static void sv_RequestDataLenUpdate(struct bt_conn *stpt_conn)
{
   int i_err;

   // Update the maximum data length
   i_err = bt_conn_le_data_len_update(stpt_conn, BT_LE_DATA_LEN_PARAM_MAX);

   // Check if any error
   if (i_err)
   {
      LOG_ERR("LE data length update request failed: %d", i_err);
   }
   else
   {
      LOG_INF("LE data length update requested");
   }
}

/**
 * @private       sv_MTUExchangeCallback
 * @brief         Callback function to be called after exchanging MTU
 * @param[in]     stpt_conn Connection handle
 * @return        None.
 */
static void sv_MTUExchangeCallback(struct bt_conn *stpt_conn, uint8_t u8_err,
   struct bt_gatt_exchange_params *stpt_exchangeParams)
{
   LOG_INF("MTU exchange %s", (u8_err == 0 ? "successful" : "failed"));

   // Check if no error
   if (!u8_err)
   {
      // Get the current MTU size
      uint16_t u16_payloadMTU = bt_gatt_get_mtu(stpt_conn) - 3;  /* 3 bytes ATT header */
      LOG_INF("New MTU payload: %d bytes", u16_payloadMTU);
   }
}

/**
 * @private       sv_ReqMTUUpdate
 * @brief         Function requests the connection for data length update.
 * @param[in]     stpt_conn Connection handle
 * @return        None.
 */
static void sv_ReqMTUUpdate(struct bt_conn *stpt_conn)
{
   int i_err;

   // Set MTU update function
   sst_exchangeParams.func = sv_MTUExchangeCallback;

   // Exchange MTU parameters
   i_err = bt_gatt_exchange_mtu(stpt_conn, &sst_exchangeParams);

   // Check if any error occured
   if (i_err)
   {
      LOG_ERR("Failed to exchange MTU: Error code is (err %d)", i_err);
   }
   else
   {
      LOG_INF("MTU exchange requested");
   }
}

/**
 * @private       sv_Connected
 * @brief         Callback for handling new connections. This function is called when a
 *                new connection is established.
 * @return        Number of bytes written.
 */
static void sv_Connected(struct bt_conn *stpt_conn, uint8_t u8_err)
{
	if (u8_err)
   {
		LOG_ERR("Connection failed (err %u)", u8_err);
		return;
	}
	LOG_INF("Connected");
   sstpt_currentConn = bt_conn_ref(stpt_conn);

   // Request MTu update
   sv_ReqMTUUpdate(stpt_conn);
   // Initiate DLE and MTU exchange from the peripheral side
   sv_RequestDataLenUpdate(stpt_conn);
}

/**
 * @private       sv_Disconnected
 * @brief         Callback for handling disconnections. This function is called when a
 *                connection is disconnected.
 * @return        Number of bytes written.
 */
static void sv_Disconnected(struct bt_conn *stpt_conn, uint8_t reason)
{
	LOG_INF("Disconnected (reason 0x%x)", reason);

   // Check if the current connection is still not cleared
   if (sstpt_currentConn)
   {
      bt_conn_unref(sstpt_currentConn);
      sstpt_currentConn = NULL;
   }
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

/**
 * @private       sv_DataLengthUpdated
 * @brief         Callback tobe called upon data length updated
 * @return        None.
 */
static void sv_DataLengthUpdated(struct bt_conn *stpt_conn,
   struct bt_conn_le_data_len_info *stpt_info)
{
   uint16_t u16_txLen = stpt_info->tx_max_len;
   uint16_t u16_txTime = stpt_info->tx_max_time;
   uint16_t u16_rxLen = stpt_info->rx_max_len;
   uint16_t u16_rxTime = stpt_info->rx_max_time;

   LOG_INF("Data length updated. Len TX/ RX: %d/ %d bytes, time TX/ RX: %d/ %d us",
      u16_txLen, u16_rxLen, u16_txTime, u16_rxTime);
}

/******************************************************************************/
/*                                                                            */
/*                        PUBLIC FUNCTION DEFINITIONS                         */
/*                                                                            */
/******************************************************************************/
/**
 * @public        gv_BLEInitStartAdv
 * @brief         Initialize BLE module and start advertising.
 * @return        None.
 */
void gv_BLEInitStartAdv(void)
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
}

/**
 * Copyright(c) Bajaj Auto Technology Limited (BATL) as an unpublished work.
 * THIS SOFTWARE AND/OR MATERIAL IS THE PROPERTY OF BATL.
 * ALL USE, DISCLOSURE, AND/OR REPRODUCTION NOT SPECIFICALLY AUTHORIZED BY
 * BATL IS PROHIBITED.
 *
 * @author:Shviam Chudasama [SC]
 */
