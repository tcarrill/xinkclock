# xinkclock - ESP32 Hardware Bringup

ESP32-C3 target for `xinkclock` with Waveshare Pico-ePaper-4.2" V2 (400x300 B/W)

## What works

- Integer-only clock rendering (Q1.15 trig tables/XIAO-ESP32-C3 lacks FPU)
- NTP time sync over WiFi with POSIX timezone support
- GxEPD2 e-paper driver with command-buffer replay for paged rendering
- Partial refresh every min, full refresh every 30 mins
- Adjustable center bias for physical panel alignment

## Wiring

> Display 0-ohm jumper moved to 3.3VDC position; panel shipped expecting 5VDC

> The ESP32-C3 has no hardware FPU, so all trig uses precomputed integer lookup tables (Q1.15 fixed-point, ~0.003% accuracy).

| Waveshare Pin | XIAO ESP32-C3 Pin |
|----|----|
| BUSY | D0 |
| RST | D1 |
| DC | D2 | 
| CS | D3 | 
| CLK (SCK) | D8                |
| DIN (MOSI)    | D10               |
| GND           | GND               |
| VCC           | 3V3               |

MISO (D9) is declared for the SPI driver but left physically unconnected.

## Setup

### 1. Install PlatformIO

```bash
pip install platformio
# or: brew install platformio
```

### 2. Create secrets.h

```bash
cd esp32
cp include/secrets.example.h include/secrets.h
```

Edit `include/secrets.h` with your WiFi credentials and timezone:

```c
#define WIFI_SSID     "your-ssid"
#define WIFI_PASSWORD "your-password"
#define TZ_STRING     "EST5EDT,M3.2.0/2,M11.1.0/2"
```

Find your POSIX TZ string: https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv

### 3. Build and flash

```bash
cd esp32
pio run -t upload
```

### 4. Monitor serial output

```bash
pio run -t monitor
```

You should see WiFi connection, NTP sync, and the local time printed.

## Tuning

Build flags in `platformio.ini`:

| Flag                      | Default | Description                                    |
|---------------------------|---------|------------------------------------------------|
| `XCLOCK_CX_BIAS`         | 6       | Horizontal center offset (pixels, +right)      |
| `XCLOCK_CY_BIAS`         | 10      | Vertical center offset (pixels, +down)         |
| `FULL_REFRESH_INTERVAL`  | 30      | Minutes between full refreshes (ghosting clear) |

If the clock face is off-center on your panel, adjust the bias values.
If the display rotation is wrong (12 o'clock in wrong position), change
`display.setRotation(N)` in `src/render/render_gxepd2.cpp` — try 0, 1, 2, or 3.
