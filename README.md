# ZMK Layer Status

A ZMK module that reports layer status via standard HID reports(Usb/Bluetooth) and USB HID raw, enabling the [
Keyboard Layers App Companion](https://github.com/maatthc/keyboard_layers_app_companion) to display the current active layer.

In the case of split keyboards, this should be defined only in the side defined as `Role Central`

Both can be enabled simultaneously if desired.

This [repo](https://github.com/maatthc/zmk-crosses) includes all the required changes for both approaches and can be used as a reference.

## Installation

Add to your `config/west.yml`:

```yaml
manifest:
  remotes:
    - name: maatthc
      url-base: https://github.com/maatthc
  projects:
    - name: zmk-feature-appcompanion
      remote: maatthc
      revision: main
```

then run `west update`.

## Configuration Options

Add the desired parameters to your shield's .conf file (e.g., crosses_right.conf).

### Standard HID: Embedded in existing Keyboard Report

```
CONFIG_ZMK_LAYER_STATUS_BLE_HID=y
```

Embeds the current layer number in the keyboard HID report's reserved byte. This works over both USB and Bluetooth HID without additional interfaces. The companion app reads standard keyboard reports to extract the layer from byte 1.

**Advantages:**
- Works over USB and BLE HID
- No extra HID interfaces needed
- Compatible with paired BLE keyboards

**Disadvantages:**
- Might require `root` access on some platforms to read raw keyboard reports
- Somewhat non-standard use of reserved byte in keyboard report

### USB HID Raw Reports

```
CONFIG_USB_HID_DEVICE_COUNT=2
CONFIG_ZMK_LAYER_STATUS_USB_HID=y
```

Sends layer status via a separate USB HID raw report interface.

The module uses HID_1 since ZMK core uses HID_0 for keyboard/mouse reports.

**Advantages:**
- Standardized way to send custom data over USB HID
- Easier to access on some platforms without `root` access

**Disadvantages:**
- Only works over USB
- Requires an additional HID interface, which may not be supported by all operating systems

Optionally, custom usage page/usage can be set for the USB HID raw report interface:
```
CONFIG_ZMK_LAYER_STATUS_USB_HID_USAGE_PAGE=0xFF60
CONFIG_ZMK_LAYER_STATUS_USB_HID_USAGE=0x61
```


## Protocol

### BLE HID Embedded (keyboard report)
- Byte 0: Modifiers
- **Byte 1: Layer number** (normally reserved/unused)
- Bytes 2+: Key codes

### USB HID Raw Report (32 bytes)
- Bytes 0-23: Reserved (`0x00`)
- Byte 24: Marker (`0x90`)
- Byte 25: Layer number
- Bytes 26-31: Reserved (`0x00`)

## License

MIT
