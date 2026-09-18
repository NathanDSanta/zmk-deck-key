zephyr_library()

if(CONFIG_ZMK_BEHAVIOR_DECK_KEY)

    zephyr_library_include_directories(include)

    zephyr_library_sources(
        src/behaviors/zmk-deck.c
        src/hid/deck_key_hid.c
    )
endif()