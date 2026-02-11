#include <Arduino.h>
#include <SPI.h>

#include <GxEPD2_BW.h>
#include <gdey/GxEPD2_420_GDEY042T81.h>

#include "render.h"
#include "../../include/epd_pins.h"

using Panel = GxEPD2_420_GDEY042T81;
static GxEPD2_BW<Panel, Panel::HEIGHT> display(Panel(PIN_EPD_CS, PIN_EPD_DC, PIN_EPD_RST, PIN_EPD_BUSY));

static bool g_inited = false;
static int  g_partial_count = 0;

#ifndef FULL_REFRESH_INTERVAL
#define FULL_REFRESH_INTERVAL 30
#endif

enum CmdType : uint8_t { CMD_LINE, CMD_TRI };

struct DrawCmd {
    CmdType type;
    int16_t x0, y0, x1, y1, x2, y2;
};

static constexpr int MAX_CMDS = 128;
static DrawCmd g_cmds[MAX_CMDS];
static int g_cmd_count = 0;

static void ensure_init()
{
    if (g_inited) return;
    g_inited = true;

    SPI.begin(PIN_EPD_SCK, PIN_EPD_MISO, PIN_EPD_MOSI, PIN_EPD_CS);
    display.init(115200, true, 2, false);
    display.setRotation(2);

    display.setFullWindow();
    display.firstPage();
    do { display.fillScreen(GxEPD_WHITE); } while (display.nextPage());
}

static void gx_clear()
{
    ensure_init();
    g_cmd_count = 0;
}

static void gx_line(int x1, int y1, int x2, int y2)
{
    if (g_cmd_count >= MAX_CMDS) return;
    DrawCmd& c = g_cmds[g_cmd_count++];
    c.type = CMD_LINE;
    c.x0 = (int16_t)x1;  c.y0 = (int16_t)y1;
    c.x1 = (int16_t)x2;  c.y1 = (int16_t)y2;
}

static void gx_tri(int x0, int y0, int x1, int y1, int x2, int y2)
{
    if (g_cmd_count >= MAX_CMDS) return;
    DrawCmd& c = g_cmds[g_cmd_count++];
    c.type = CMD_TRI;
    c.x0 = (int16_t)x0;  c.y0 = (int16_t)y0;
    c.x1 = (int16_t)x1;  c.y1 = (int16_t)y1;
    c.x2 = (int16_t)x2;  c.y2 = (int16_t)y2;
}

static void replay_cmds()
{
    display.fillScreen(GxEPD_WHITE);
    for (int i = 0; i < g_cmd_count; i++) {
        const DrawCmd& c = g_cmds[i];
        switch (c.type) {
            case CMD_LINE:
                display.drawLine(c.x0, c.y0, c.x1, c.y1, GxEPD_BLACK);
                break;
            case CMD_TRI:
                display.fillTriangle(c.x0, c.y0, c.x1, c.y1, c.x2, c.y2, GxEPD_BLACK);
                break;
        }
    }
}

static void gx_flush()
{
    ensure_init();

    bool do_full = (g_partial_count >= FULL_REFRESH_INTERVAL);

    if (do_full) {
        Serial.println("[epd] full refresh");
        display.setFullWindow();
        display.firstPage();
        do { replay_cmds(); } while (display.nextPage());
        g_partial_count = 0;
    } else {
        display.setPartialWindow(0, 0, display.width(), display.height());
        display.firstPage();
        do { replay_cmds(); } while (display.nextPage());
        g_partial_count++;
    }
}

Render gxepd2_renderer()
{
    ensure_init();

    Render r;
    r.width  = display.width();
    r.height = display.height();

    r.clear = gx_clear;
    r.draw_filled_triangle = gx_tri;
    r.draw_line = gx_line;
    r.flush = gx_flush;

    r.userdata = nullptr;
    return r;
}
