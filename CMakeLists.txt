if(CONFIG_RAW_HID)
  target_sources(app PRIVATE
    src/hid/raw_hid.c
    src/behavior/behavior_deck_key.c
  )

  target_include_directories(app PRIVATE
    include
  )
endif()