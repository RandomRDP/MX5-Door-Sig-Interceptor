# MX5-Door-Sig-Interceptor

A small project to read signals from central looking unit to trigger lighting presets in [WLED](https://github.com/Aircoookie/WLED)

Configue input pin on ESP32 as a normal pushbutton with 3 presets

| Action | State | 
|----:|-----------|
| short press | Unlocked Engine off |
| long press | Locked Engine off
| double press | Engine on |

| Pin | Description |
|----:|-----------|
| PA0 | DS34C87N Enable Pin|
| PA3 | ESP32 Enable Pin    |
| PA4 | ESP32 Trigger Pin |
| PA5 | Ignition Sense       |
| PA6 | Door Lock Sense |
| PA7 | Door Unlock Sense |

