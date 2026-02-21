#include "render_gxepd2.h"
#include <SPI.h>

RendererGXEPD2::RendererGXEPD2()
    : m_display(GxEPD2_420_GDEY042T81(PIN_EPD_CS, PIN_EPD_DC, PIN_EPD_RST, PIN_EPD_BUSY)),
      m_inited(false), m_partialCount(0), m_cmdCount(0)
{
    ensureInit();
    m_width = m_display.width();
    m_height = m_display.height();
}

void RendererGXEPD2::ensureInit() {
    if (m_inited) return;
    m_inited = true;

    SPI.begin(PIN_EPD_SCK, PIN_EPD_MISO, PIN_EPD_MOSI, PIN_EPD_CS);
    m_display.init(115200, true, 2, false);
    m_display.setRotation(2);

    // Clear display at startup
    m_display.setFullWindow();
    m_display.firstPage();
    do { m_display.fillScreen(GxEPD_WHITE); } while (m_display.nextPage());
}

void RendererGXEPD2::clear() {
    m_cmdCount = 0;
}

void RendererGXEPD2::drawLine(int x1, int y1, int x2, int y2) {
    if (m_cmdCount >= MAX_CMDS) return;
    m_cmds[m_cmdCount++] = { CMD_LINE, (int16_t)x1, (int16_t)y1, (int16_t)x2, (int16_t)y2, 0, 0 };
}

void RendererGXEPD2::drawFilledTriangle(int x0, int y0, int x1, int y1, int x2, int y2) {
    if (m_cmdCount >= MAX_CMDS) return;
    m_cmds[m_cmdCount++] = { CMD_TRI, (int16_t)x0, (int16_t)y0, (int16_t)x1, (int16_t)y1, (int16_t)x2, (int16_t)y2 };
}

void RendererGXEPD2::replayCommands() {
    m_display.fillScreen(GxEPD_WHITE);
    for (int i = 0; i < m_cmdCount; ++i) {
        const DrawCmd &c = m_cmds[i];
        switch (c.type) {
            case CMD_LINE:
                m_display.drawLine(c.x0, c.y0, c.x1, c.y1, GxEPD_BLACK);
                break;
            case CMD_TRI:
                m_display.fillTriangle(c.x0, c.y0, c.x1, c.y1, c.x2, c.y2, GxEPD_BLACK);
                break;
        }
    }
}

void RendererGXEPD2::flush() {
    bool doFull = (m_partialCount >= FULL_REFRESH_INTERVAL);

    if (doFull) {
        m_display.setFullWindow();
        m_display.firstPage();
        do { replayCommands(); } while (m_display.nextPage());
        m_partialCount = 0;
    } else {
        m_display.setPartialWindow(0, 0, m_width, m_height);
        m_display.firstPage();
        do { replayCommands(); } while (m_display.nextPage());
        m_partialCount++;
    }
    m_cmdCount = 0; // reset commands after flush
}