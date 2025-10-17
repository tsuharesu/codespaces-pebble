# Settings (Clay configuration)

This app accepts configuration from Clay (the Pebble configuration page). The app listens for two Clay settings keys and persists them locally.

Message keys

- `ConfTemp` (MESSAGE_KEY_ConfTemp, id in build: 10003)
  - Purpose: choose temperature unit to display
  - Accepted types: string (`"C"` or `"F"`) or integer (`0` for C, `1` for F)
  - Default: `C`

- `ConfSymbol` (MESSAGE_KEY_ConfSymbol, id in build: 10002)
  - Purpose: single-character command symbol shown before command labels
  - Accepted types: string (first character is used)
  - Default: `$`

Runtime behavior

- The app stores settings in persistent storage and applies them immediately when received.
- Temperature data is still received from the phone as Kelvin (`MESSAGE_KEY_TEMPERATURE`) and converted locally. The displayed temperature will follow the selected unit.

Files

- `src/c/settings.h` and `src/c/settings.c` — definition and persistence for `AppSettings`.
- `src/c/main.c` — reads Clay keys (`MESSAGE_KEY_ConfTemp`, `MESSAGE_KEY_ConfSymbol`) and updates `app_settings`.

Notes

- The app will accept both simple int or string values for `ConfTemp` to be robust to different Clay component configurations.
- `ConfSymbol` uses the first character of the string sent by Clay; if an empty string is sent, the default `$` remains.
