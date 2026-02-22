#include <Arduino.h>
#include <WiFi.h>
#include <time.h>
#include "esp_sntp.h"
#include "esp_sleep.h"


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
    Serial.printf("Connecting to %s ", WIFI_SSID);
    uint32_t start = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - start <= 20000) {
        Serial.print(".");
        delay(250);
    }

    Serial.println();

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("Connected!");
    } else {
        Serial.println("Connection timed out.");
    }
}

static void wifi_disconnect() {
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    Serial.printf("Disconnected from %s\n", WIFI_SSID);
}

static void apply_tz() {
    setenv("TZ", TZ_STRING, 1);
    tzset();
}

static bool ntp_sync() {
    sntp_set_sync_mode(SNTP_SYNC_MODE_IMMED);
    configTime(0, 0, "time.google.com", "pool.ntp.org", "time.nist.gov");

    Serial.println("Waiting for NTP sync...");

    for (int i = 0; i < 60; ++i) {
        time_t now = time(nullptr);

        if (now > 1700000000) {   // Valid epoch check
            last_ntp_sync = now;

            // UTC time
            tm utc{};
            gmtime_r(&now, &utc);

            // Local time
            tm local{};
            localtime_r(&now, &local);

            Serial.println("NTP synced successfully!");
            Serial.printf("UTC Time   : %04d-%02d-%02d %02d:%02d:%02d\n",
                          utc.tm_year + 1900,
                          utc.tm_mon + 1,
                          utc.tm_mday,
                          utc.tm_hour,
                          utc.tm_min,
                          utc.tm_sec);

            Serial.printf("Local Time : %04d-%02d-%02d %02d:%02d:%02d\n",
                          local.tm_year + 1900,
                          local.tm_mon + 1,
                          local.tm_mday,
                          local.tm_hour,
                          local.tm_min,
                          local.tm_sec);

            return true;
        }

        Serial.print(".");
        delay(250);
    }

    Serial.println("\nNTP sync failed.");
    return false;
}

static uint32_t seconds_until_next_minute() {
    time_t now = time(nullptr);
    tm t{};
    localtime_r(&now, &t);
    return 60 - t.tm_sec;
}

static ClockTime now_local() {
    apply_tz();
    time_t now = time(nullptr);
    tm t{};
    localtime_r(&now, &t);
    return ClockTime{ t.tm_hour, t.tm_min };
}

void setup() {
    Serial.begin(115200);
    delay(1500);
    Serial.println("Booting xinkclock...");
    apply_tz();
    wifi_connect();
    ntp_sync();
    wifi_disconnect();

    static RendererGXEPD2 renderer;
    ClockTime ct = now_local();

    // Draw initial full face
    xclock_draw_face(renderer);
    xclock_draw_hands(renderer, ct);
    renderer.flush();
}

void loop() {
    ClockTime ct = now_local();

    if (time(nullptr) - last_ntp_sync >= NTP_SYNC_INTERVAL) {
        Serial.println("Re-syncing NTP...");
        wifi_connect();
        ntp_sync();
        wifi_disconnect();
    }

    static RendererGXEPD2 renderer;
    xclock_draw_face(renderer);
    xclock_draw_hands(renderer, ct);
    renderer.flush();
     delay(seconds_until_next_minute() * 1000);
}