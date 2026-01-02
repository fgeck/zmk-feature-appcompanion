if(CONFIG_ZMK_LAYER_STATUS_HID)
    target_sources_ifdef(CONFIG_ZMK_SPLIT_ROLE_CENTRAL app PRIVATE src/layer_status_hid.c)
endif()
