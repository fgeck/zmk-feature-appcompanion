if(CONFIG_ZMK_LAYER_STATUS_USB_HID OR CONFIG_ZMK_LAYER_STATUS_BLE_HID)
    zephyr_library()
    zephyr_include_directories(include)
endif()

if(CONFIG_ZMK_LAYER_STATUS_USB_HID)
    zephyr_library_sources(src/layer_status_usb_hid.c)
endif()

if(CONFIG_ZMK_LAYER_STATUS_BLE_HID)
    zephyr_library_sources(src/layer_status_ble_hid.c)
endif()
