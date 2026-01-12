extern "C" {
  #include "../core/xclock.h"
  #include "../render/render.h"
}

// Temporary stubs until you add e-ink
static void clear() {}
static void circle(int,int,int) {}
static void line(int,int,int,int) {}
static void flush() {}

Render esp_renderer() {
  Render r = {
    .clear = clear,
    .draw_circle = circle,
    .draw_line = line,
    .flush = flush
  };
  return r;
}

void setup() {
  ClockTime t = { 10, 15, 0 };
  Render r = esp_renderer();
  xclock_draw(&r, t);
}

void loop() {}
