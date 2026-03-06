#ifndef RENDERER_GXEPD2_H
#define RENDERER_GXEPD2_H

#include "render.h"
#include <GxEPD2_BW.h>

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

class RendererGXEPD2 : public Renderer {
public:
    RendererGXEPD2();
    virtual ~RendererGXEPD2() = default;

    // Dimensions
    int width() const override { return m_width; }
    int height() const override { return m_height; }

    // Draw primitives
    void clear() override;
    void drawLine(int x1, int y1, int x2, int y2) override;
    void drawFilledTriangle(int x0, int y0, int x1, int y1, int x2, int y2) override;

    // Flush to display: handles partial vs full refresh internally
    void flush() override;

private:
    void ensureInit();
    void replayCommands();
    
    GxEPD2_BW<GxEPD2_420_GDEY042T81, GxEPD2_420_GDEY042T81::HEIGHT> m_display;
    bool m_inited;
    int m_width;
    int m_height;
    int m_partialCount;

    enum CmdType : uint8_t { CMD_LINE, CMD_TRI };
    struct DrawCmd {
        CmdType type;
        int16_t x0, y0, x1, y1, x2, y2;
    };

    static constexpr int MAX_CMDS = 128;
    DrawCmd m_cmds[MAX_CMDS];
    int m_cmdCount;
};

#endif