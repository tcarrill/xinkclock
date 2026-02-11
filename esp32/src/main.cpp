#include <Arduino.h>
#include <WiFi.h>
#include <time.h>
#include "esp_sntp.h"

#include "secrets.h"
#include "epd_pins.h"

#include "render/render.h"
#include "core/clock_time.h"
#include "core/xclock.h"

Render gxepd2_renderer();

static void wifi_connect()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.printf("WiFi connecting to '%s' ...\n", WIFI_SSID);

    uint32_t start = millis();
    while (WiFi.status() != WL_CONNECTED) {
        delay(250);
        Serial.print(".");
        if (millis() - start > 20000) {
            Serial.println("\nWiFi timeout.");
            return;
        }
    }
    Serial.printf("\nWiFi connected: %s\n", WiFi.localIP().toString().c_str());
}

static void apply_tz()
{
    setenv("TZ", TZ_STRING, 1);
    tzset();
}

static bool ntp_sync()
{
    apply_tz();
    sntp_set_sync_mode(SNTP_SYNC_MODE_IMMED);
    configTime(0, 0, "time.google.com", "pool.ntp.org", "time.nist.gov");

    Serial.print("Waiting for NTP sync");
    for (int i = 0; i < 60; i++) {
        if (time(nullptr) > 1700000000) {
            Serial.println(" OK");
            return true;
        }
        delay(250);
        Serial.print(".");
    }
    Serial.println(" FAILED");
    return false;
}

static int last_minute = -1;

static ClockTime now_local()
{
    apply_tz();
    time_t now = time(nullptr);
    tm t{};
    localtime_r(&now, &t);
    return ClockTime{ t.tm_hour, t.tm_min };
}

void setup()
{
    Serial.begin(115200);
    delay(1500);
    Serial.println("\nBOOT xinkclock");

    Render r = gxepd2_renderer();

    wifi_connect();
    if (!ntp_sync()) {
        Serial.println("No time; skipping initial draw.");
        return;
    }

    ClockTime ct = now_local();
    Serial.printf("Local time: %02d:%02d\n", ct.hour, ct.minute);

    last_minute = ct.minute;
    xclock_draw(&r, ct);
}

void loop()
{
    if (time(nullptr) < 1700000000) {
        delay(500);
        return;
    }

    ClockTime ct = now_local();

    if (ct.minute != last_minute) {
        last_minute = ct.minute;
        Serial.printf("Refresh %02d:%02d\n", ct.hour, ct.minute);

        Render r = gxepd2_renderer();
        xclock_draw(&r, ct);
    }

    delay(250);
}
