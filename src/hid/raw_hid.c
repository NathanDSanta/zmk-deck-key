#include "zmk-deck-key/raw_hid.h"

#include <raw_hid/events.h>

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(raw_hid_hi, CONFIG_ZMK_LOG_LEVEL);

/*
 * The Raw HID event contains a pointer to the data rather than copying the
 * payload. Therefore this buffer must remain valid after the event is raised.
 */
static uint8_t hi_message[] = {
    'H',
    'i',
};

void raw_hid_send_hi(void) {
    LOG_INF("Sending Raw HID message: Hi");

    raise_raw_hid_sent_event((struct raw_hid_sent_event){
        .data = hi_message,
        .length = sizeof(hi_message),
    });
}

static void send_hi_work_handler(struct k_work *work) {
    ARG_UNUSED(work);

    raw_hid_send_hi();
}

K_WORK_DELAYABLE_DEFINE(send_hi_work, send_hi_work_handler);

static int raw_hid_hi_init(void) {
    /*
     * Delay the transmission to give USB enumeration or Bluetooth
     * initialization time to complete.
     */
    k_work_schedule(&send_hi_work, K_SECONDS(2));

    return 0;
}

SYS_INIT(raw_hid_hi_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);