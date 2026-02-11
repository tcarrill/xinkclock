#pragma once

// Copy this file to secrets.h and fill in your credentials.
// secrets.h is .gitignored and will not be committed.

#define WIFI_SSID     "your-ssid"
#define WIFI_PASSWORD "your-password"

// POSIX TZ string for your timezone.
// Examples:
//   US Eastern:  "EST5EDT,M3.2.0/2,M11.1.0/2"
//   US Central:  "CST6CDT,M3.2.0/2,M11.1.0/2"
//   US Pacific:  "PST8PDT,M3.2.0/2,M11.1.0/2"
//   UTC:         "UTC0"
// Full list: https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
#define TZ_STRING "EST5EDT,M3.2.0/2,M11.1.0/2"
