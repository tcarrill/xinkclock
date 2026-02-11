#pragma once

// Waveshare Pico-ePaper-4.2" V2 -> Seeed XIAO ESP32-C3
// Verified by continuity + working panel bring-up.

static constexpr int PIN_EPD_BUSY   = D0;
static constexpr int PIN_EPD_RST    = D1;
static constexpr int PIN_EPD_DC     = D2;
static constexpr int PIN_EPD_CS     = D3;

static constexpr int PIN_EPD_SCK    = D8;
static constexpr int PIN_EPD_MOSI   = D10;

// ESP32-C3 Arduino SPI wants a valid MISO pin even if
// the panel doesn't use it.  Leave disconnected.
static constexpr int PIN_EPD_MISO   = D9;
