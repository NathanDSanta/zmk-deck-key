if(CONFIG_RAW_HID)
  target_sources(app PRIVATE
    src/raw_hid.c
  )

  target_include_directories(app PRIVATE
    include
  )
endif()