#include <zephyr/bluetooth/gatt.h>
#include "gatt_fota.h"

/* ================= Storage Buffers ================= */

static uint8_t fota_control_buf[4];
static uint8_t ecu_id_buf[64];
static uint8_t image_metadata_buf[12];
static uint32_t active_block_address;
static uint8_t firmware_data_buf[244];
static uint8_t fota_status_buf[4];
static uint8_t fota_progress_buf[12];
static uint8_t resume_offset_buf[8];

/* ================= Write Handlers ================= */

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
