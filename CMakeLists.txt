zephyr_library()

if((NOT CONFIG_ZMK_SPLIT) OR CONFIG_ZMK_SPLIT_ROLE_CENTRAL)

    target_sources_ifdef(CONFIG_ZMK_DECK_KEY app PRIVATE src/behaviors/behavior-deck-key.c src/hid/deck_key_hid.c)

    
    zephyr_library_include_directories(include)

endif()