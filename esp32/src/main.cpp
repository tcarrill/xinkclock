#include <Arduino.h>
#include <WiFi.h>
#include <time.h>
#include "esp_sntp.h"

#include "../include/secrets.h"

#include "render/render_gxepd2.h"
#include "core/xclock.h"
#include "core/clock_time.h"

#define NTP_SYNC_INTERVAL 86400L // 24h
#define TZ_STRING "EST5EDT,M3.2.0/2,M11.1.0/2"  // Example for US Eastern Time

static time_t last_ntp_sync = 0;

static void wifi_connect() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    uint32_t start = millis();
    while (WiFi.status() != WL_CONNECTED) {
        delay(250);
        if (millis() - start > 20000) break;
    }
}

static void apply_tz() {
    setenv("TZ", TZ_STRING, 1);
    tzset();
}

static bool ntp_sync() {
    apply_tz();
    sntp_set_sync_mode(SNTP_SYNC_MODE_IMMED);
    configTime(0, 0, "time.google.com", "pool.ntp.org", "time.nist.gov");

    for (int i = 0; i < 60; ++i) {
        if (time(nullptr) > 1700000000) {
            last_ntp_sync = time(nullptr);
            return true;
        }
        delay(250);
    }
    return false;
}

static ClockTime now_local() {
    apply_tz();
    time_t now = time(nullptr);
    tm t{};
    localtime_r(&now, &t);
    Serial.printf("[DEBUG] Localtime: %02d:%02d\n", t.tm_hour, t.tm_min);
    return ClockTime{ t.tm_hour, t.tm_min };
}

void setup() {
    Serial.begin(115200);
    delay(1500);
    Serial.println("Booting xinkclock...");

    wifi_connect();
    ntp_sync();

    static RendererGXEPD2 renderer;
    ClockTime ct = now_local();

    // Draw initial full face
    xclock_draw_face(renderer);
    xclock_draw_hands(renderer, ct);
    renderer.flush();
}

void loop() {
    static int last_minute = -1;

    ClockTime ct = now_local();

    // Re-sync NTP every 24h
    if (time(nullptr) - last_ntp_sync >= NTP_SYNC_INTERVAL) {
        Serial.println("Re-syncing NTP...");
        wifi_connect();
        ntp_sync();
    }

    // Only update hands if minute changed
    if (ct.minute != last_minute) {
        last_minute = ct.minute;

        static RendererGXEPD2 renderer;
        xclock_draw_face(renderer);
        xclock_draw_hands(renderer, ct);
        renderer.flush();
    }

    delay(500);
}