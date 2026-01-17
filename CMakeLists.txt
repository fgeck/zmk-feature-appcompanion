if(CONFIG_ZMK_LAYER_STATUS_HID)
    zephyr_library()
    
    zephyr_library_sources(src/layer_status_hid.c)
    zephyr_include_directories(include)
endif()
