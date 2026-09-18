#define DT_DRV_COMPAT zmk_behavior_deck_key

#include <stdint.h>

#include <zephyr/device.h>
#include <zephyr/logging/log.h>

#include <drivers/behavior.h>

#include <zmk/behavior.h>

#include "deck_key_hid.h"

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

static int deck_key_pressed(
    struct zmk_behavior_binding *binding,
    struct zmk_behavior_binding_event event
)
{
    ARG_UNUSED(event);

    uint8_t event_id =
        (uint8_t)binding->param1;

    LOG_INF(
        "Deck Key: %u",
        event_id
    );

    return deck_key_hid_send(event_id);
}

static int deck_key_released(
    struct zmk_behavior_binding *binding,
    struct zmk_behavior_binding_event event
)
{
    ARG_UNUSED(binding);
    ARG_UNUSED(event);

    /*
     * Deck keys are one-shot events.
     *
     * Nothing is sent when the physical key
     * is released.
     */

    return 0;
}

static const struct behavior_driver_api deck_key_driver_api = {
    .binding_pressed = deck_key_pressed,
    .binding_released = deck_key_released,
};

BEHAVIOR_DT_INST_DEFINE(
    0,
    NULL,
    NULL,
    NULL,
    NULL,
    POST_KERNEL,
    CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,
    &deck_key_driver_api
);