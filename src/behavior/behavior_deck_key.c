#define DT_DRV_COMPAT zmk_behavior_deck_key

#include <stdint.h>

#include <raw_hid/events.h>

#include <zephyr/device.h>
#include <zephyr/logging/log.h>

#include <drivers/behavior.h>
#include <zmk/behavior.h>

LOG_MODULE_REGISTER(behavior_deck_key, CONFIG_ZMK_LOG_LEVEL);

/*
 * The Raw HID event carries a pointer to the payload. The transport consumes
 * this synchronously from the event listener, so keep the payload in static
 * storage rather than on the stack.
 *
 * This behavior sends one byte:
 *
 *   report[0] = key index
 */
static uint8_t deck_key_report[1];

static int deck_key_pressed(struct zmk_behavior_binding *binding,
                            struct zmk_behavior_binding_event event) {
    ARG_UNUSED(event);

    uint8_t key_index = (uint8_t)(binding->param1 & 0xff);

    deck_key_report[0] = key_index;

    LOG_INF("Sending deck key index: %u", key_index);

    raise_raw_hid_sent_event((struct raw_hid_sent_event){
        .data = deck_key_report,
        .length = sizeof(deck_key_report),
    });

    return ZMK_BEHAVIOR_OPAQUE;
}

static int deck_key_released(struct zmk_behavior_binding *binding,
                             struct zmk_behavior_binding_event event) {
    ARG_UNUSED(binding);
    ARG_UNUSED(event);

    /*
     * Nothing is sent on release. Change this if you also want a release
     * report.
     */
    return ZMK_BEHAVIOR_OPAQUE;
}

static const struct zmk_behavior_driver_api deck_key_api = {
    .binding_pressed = deck_key_pressed,
    .binding_released = deck_key_released,
};

#define DECK_KEY_INIT(n)                                                     \
    BEHAVIOR_DT_INST_DEFINE(                                                 \
        n,                                                                    \
        NULL,                                                                 \
        NULL,                                                                 \
        NULL,                                                                 \
        NULL,                                                                 \
        POST_KERNEL,                                                          \
        CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,                                 \
        &deck_key_api                                                         \
    );

DT_INST_FOREACH_STATUS_OKAY(DECK_KEY_INIT)