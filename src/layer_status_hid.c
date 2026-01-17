#include <string.h>
#include <zephyr/kernel.h>
#include <zephyr/usb/class/usb_hid.h>
#include <zephyr/usb/usb_device.h>
#include <zmk/event_manager.h>
#include <zmk/events/layer_state_changed.h>
#include <zmk/keymap.h>
#include <zmk/usb.h>
#include "layer_status_hid.h"

#define RAW_EPSIZE 32
#define PAYLOAD_MARK 0x90
#define PAYLOAD_BEGIN 24

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

static const struct device *hid_dev;
static bool usb_ready = false;

static K_SEM_DEFINE(hid_sem, 1, 1);

static void in_ready_cb(const struct device *dev) {
    k_sem_give(&hid_sem);
}

static const struct hid_ops ops = {
    .int_in_ready = in_ready_cb,
};

static void send_layer_report(uint8_t layer) {
  if (hid_dev == NULL || !usb_ready) {
    return;
  }

  k_sem_take(&hid_sem, K_MSEC(30));

  uint8_t report[RAW_EPSIZE];
  memset(report, 0x00, RAW_EPSIZE);
  report[PAYLOAD_BEGIN] = PAYLOAD_MARK;
  report[PAYLOAD_BEGIN + 1] = layer;

  int ret = hid_int_ep_write(hid_dev, report, RAW_EPSIZE, NULL);
  if (ret < 0 && ret != -EAGAIN) {
    LOG_ERR("Failed to send layer report: %d", ret);
    k_sem_give(&hid_sem);
  }
}

static int layer_status_hid_init(const struct device *dev) {
  hid_dev = device_get_binding("HID_1");
  if (hid_dev == NULL) {
    LOG_WRN("HID_1 device not available");
    return 0;
  }

  usb_hid_register_device(hid_dev, layer_status_hid_report_desc,
                          sizeof(layer_status_hid_report_desc), &ops);

  usb_hid_init(hid_dev);

  k_sleep(K_MSEC(500));
  usb_ready = true;

  return 0;
}

static int layer_status_hid_event_listener(const zmk_event_t *eh) {
  const struct zmk_layer_state_changed *ev = as_zmk_layer_state_changed(eh);
  if (ev == NULL) {
    return -ENOTSUP;
  }

  uint8_t highest_layer = zmk_keymap_highest_layer_active();
  send_layer_report(highest_layer);

  return 0;
}

ZMK_LISTENER(layer_status_hid, layer_status_hid_event_listener);
ZMK_SUBSCRIPTION(layer_status_hid, zmk_layer_state_changed);

SYS_INIT(layer_status_hid_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
