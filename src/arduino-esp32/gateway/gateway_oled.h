/**
   USE OF THIS SOFTWARE IS GOVERNED BY THE TERMS AND CONDITIONS
   OF THE LICENSE STATEMENT AND LIMITED WARRANTY FURNISHED WITH
   THE PRODUCT.
   <p/>
   IN PARTICULAR, YOU WILL INDEMNIFY AND HOLD B2N LTD., ITS
   RELATED COMPANIES AND ITS SUPPLIERS, HARMLESS FROM AND AGAINST ANY
   CLAIMS OR LIABILITIES ARISING OUT OF THE USE, REPRODUCTION, OR
   DISTRIBUTION OF YOUR PROGRAMS, INCLUDING ANY CLAIMS OR LIABILITIES
   ARISING OUT OF OR RESULTING FROM THE USE, MODIFICATION, OR
   DISTRIBUTION OF PROGRAMS OR FILES CREATED FROM, BASED ON, AND/OR
   DERIVED FROM THIS SOURCE CODE FILE.
*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// OLED DISPLAY

#if defined(OLED)

#include "SSD1306.h"

SSD1306 display(DISPLAY_ADDR, DISPLAY_SDA, DISPLAY_SCL, DISPLAY_GEOMETRY);

#define DISPLAY_TIMEOUT  10000
uint32_t DISPLAY_LAST_EXECUTE;

uint8_t DISPLAY_PROGRESS;

uint64_t DISPLAY_MSEC;

inline void DISPLAY_INIT()
{
  DISPLAY_LAST_EXECUTE = millis();
  DISPLAY_PROGRESS = 10;
  DISPLAY_MSEC = 0;

  pinMode(DISPLAY_RESET_PIN, OUTPUT);

  digitalWrite(DISPLAY_RESET_PIN, LOW); // set GPIO16 low to reset OLED
  delay(50);
  digitalWrite(DISPLAY_RESET_PIN, HIGH);

  // Initialising the UI will init the display too.
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 0, "Version:" + String(Version_Major_Minor_Revision));
  display.drawString(0, 10, "ID:" + String(GATEWAY_ID));
  String key = String();
  for (uint8_t i = 0; i < 16; i++)
  {
    String bx = String(GATEWAY_KEY[i], HEX);
    if (bx.length() < 2)
    {
      bx = "0" + bx;
    }
    key += bx;
  }
  key.toUpperCase();
  display.drawString(0, 20, "KEY: " + String(key));
  display.drawString(0, 30, "SSID: " + String(WIFI_SSID));
  display.drawString(0, 40, "PASS: " + String(WIFI_PASS));
  display.drawString(0, 50, "Freq: " + String(FREQ, 2));
  display.display();

  LOG64_SET(F("DISPLAY: INIT"));
  LOG64_NEW_LINE;
}

inline void DISPLAY_WORK()
{
  if (((uint32_t)(((uint32_t)millis()) - DISPLAY_LAST_EXECUTE)) >= DISPLAY_TIMEOUT)
  {
    DISPLAY_MSEC += (uint64_t)((uint32_t)(((uint32_t)millis()) - DISPLAY_LAST_EXECUTE));
    DISPLAY_LAST_EXECUTE = millis();

    display.clear();

    DISPLAY_PROGRESS += 10;
    if (DISPLAY_PROGRESS > 100)
    {
      DISPLAY_PROGRESS = 10;
      display.resetDisplay();
      display.flipScreenVertically();
      display.setFont(ArialMT_Plain_10);
      LOG64_SET(F("DISPLAY: INIT"));
      LOG64_NEW_LINE;
    }

    String iot_strength = " RF:" + String(gateway->signal_strength()) + "% WF:" +  String(WIFI_STRENGTH()) + "% " + String(FREQ, 1);
    display.drawString(0, 0, iot_strength.c_str());

    String iot_ack = " ACK SENT : " +  String(gateway->ack_messages());
    display.drawString(0, 10, iot_ack.c_str());

    String iot_recv = " RECV MY: " +  String(gateway->recv_messages());
    display.drawString(0, 20, iot_recv.c_str());

    String iot_recv_all = " RECV ALL: " +  String(gateway->all_messages());
    display.drawString(0, 30, iot_recv_all.c_str());

#define DAY_MILLIS 86400000LL
#define HOUR_MILLIS 3600000LL
#define MINUTE_MILLIS 60000LL
#define SECOND_MILLIS 1000LL

    uint32_t d = (uint32_t)(DISPLAY_MSEC / DAY_MILLIS);
    uint8_t h = (uint8_t)((DISPLAY_MSEC % DAY_MILLIS)  / HOUR_MILLIS);
    uint8_t m = (uint8_t)(((DISPLAY_MSEC % DAY_MILLIS) % HOUR_MILLIS) / MINUTE_MILLIS);
    uint8_t s = (uint8_t)((((DISPLAY_MSEC % DAY_MILLIS) % HOUR_MILLIS) % MINUTE_MILLIS) / SECOND_MILLIS);

    String iot_now = " TIME : " + String(d) + " " + ((h < 10) ? "0" + String(h) : String(h)) + ":"  + ((m < 10) ? "0" + String(m) : String(m)) + ":" + ((s < 10) ? "0" + String(s) : String(s));
    display.drawString(0, 40, iot_now.c_str());

    String temp = " T: " +  ((DHT_TEMP_GET() == -20.0f) ?  "---" : String(DHT_TEMP_GET(), 1)) + "C " + ((DHT_HUM_GET() == 0) ? "--" : (String(DHT_HUM_GET()))) + "%";
    display.drawString(0, 50, temp.c_str());

    display.drawProgressBar(75, 54, 52, 6, DISPLAY_PROGRESS);

    display.display();

  }
}

#endif






