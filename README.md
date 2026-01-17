# ZMK Layer Status HID

A ZMK module that reports layer status via USB HID raw reports, enabling companion apps like [Keyboard Layers](https://github.com/maatthc/keyboard_layers_app_companion) to display the current active layer on the computer.

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

## Configuration

Add to your shield's `.conf` file (e.g., `crosses_right.conf`):

```
CONFIG_USB_HID_DEVICE_COUNT=2
CONFIG_ZMK_LAYER_STATUS_HID=y
```

The module uses `HID_1` since ZMK core uses `HID_0` for keyboard/mouse reports.

### Optional: Custom HID Usage Page/Usage

```
CONFIG_ZMK_LAYER_STATUS_HID_USAGE_PAGE=0xFF60
CONFIG_ZMK_LAYER_STATUS_HID_USAGE=0x61
```

## HID Report Format

32-byte reports:
- Bytes 0-23: Reserved (`0x00`)
- Byte 24: Marker (`0x90`)
- Byte 25: Layer number
- Bytes 26-31: Reserved (`0x00`)

## Notes

- **USB only** - does not work over Bluetooth
- **Split keyboards** - enable on whichever side connects via USB
- Reports highest active layer when multiple layers are active

## License

MIT
