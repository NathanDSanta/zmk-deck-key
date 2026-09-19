#pragma once

#include <stdint.h>

int deck_key_hid_init(void);

int deck_key_hid_send(uint8_t event_id);