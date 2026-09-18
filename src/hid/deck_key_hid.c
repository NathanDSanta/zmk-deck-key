#include "deck_key_hid.h"

#include <errno.h>
#include <stdint.h>

#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <zephyr/usb/class/usbd_hid.h>

LOG_MODULE_REGISTER(deck_key_hid, CONFIG_ZMK_LOG_LEVEL);

#define DECK_KEY_PROTOCOL_VERSION 0x01

/*
 * Vendor-defined HID usage page.
 *
 * 0xFF00-0xFFFF are vendor-defined HID usage pages.
 */
#define DECK_KEY_USAGE_PAGE 0xFF00

/*
 * We use a single input report:
 *
 *   byte 0 = protocol version
 *   byte 1 = event ID
 */
#define DECK_KEY_REPORT_SIZE 2

/*
 * HID report descriptor.
 */
static const uint8_t deck_key_report_desc[] = {
    /* Usage Page (Vendor Defined 0xFF00) */
    0x06, 0x00, 0xFF,

    /* Usage (0x01) */
    0x09, 0x01,

    /* Collection (Application) */
    0xA1, 0x01,

    /*
     * Logical Minimum = 0
     */
    0x15, 0x00,

    /*
     * Logical Maximum = 255
     */
    0x26, 0xFF, 0x00,

    /*
     * Report Size = 8 bits
     */
    0x75, 0x08,

    /*
     * Report Count = 2
     */
    0x95, 0x02,

    /*
     * Input (Data, Variable, Absolute)
     */
    0x81, 0x02,

    /* End Collection */
    0xC0,
};

static bool deck_key_ready;

static void deck_key_iface_ready(
    const struct device *dev,
    bool ready
)
{
    ARG_UNUSED(dev);

    deck_key_ready = ready;

    LOG_INF(
        "Deck Key HID interface %s",
        ready ? "ready" : "not ready"
    );
}

static const struct hid_device_ops deck_key_hid_ops = {
    .iface_ready = deck_key_iface_ready,
};

int deck_key_hid_init(void)
{
    /*
     * The actual HID device instance is supplied by the
     * Zephyr USB device stack.
     *
     * We obtain it through the devicetree HID node below.
     */

    const struct device *dev =
        DEVICE_DT_GET(DT_NODELABEL(deck_key_hid));

    if (!device_is_ready(dev)) {
        LOG_ERR("Deck Key HID device is not ready");
        return -ENODEV;
    }

    int ret = hid_device_register(
        dev,
        deck_key_report_desc,
        sizeof(deck_key_report_desc),
        &deck_key_hid_ops
    );

    if (ret < 0) {
        LOG_ERR(
            "Failed to register Deck Key HID: %d",
            ret
        );

        return ret;
    }

    LOG_INF("Deck Key HID registered");

    return 0;
}

int deck_key_hid_send(uint8_t event_id)
{
    const struct device *dev =
        DEVICE_DT_GET(DT_NODELABEL(deck_key_hid));

    if (!device_is_ready(dev)) {
        return -ENODEV;
    }

    if (!deck_key_ready) {
        return -EAGAIN;
    }

    /*
     * Raw HID report:
     *
     *   01 = protocol version
     *   XX = event ID
     */
    uint8_t report[DECK_KEY_REPORT_SIZE] = {
        DECK_KEY_PROTOCOL_VERSION,
        event_id,
    };

    LOG_HEXDUMP_DBG(
        report,
        sizeof(report),
        "Deck Key report"
    );

    return hid_device_submit_report(
        dev,
        sizeof(report),
        report
    );
}