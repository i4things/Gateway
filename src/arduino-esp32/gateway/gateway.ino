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


#define OLED
#define HELTEC
//#define DHT_AVAILABLE
//#define TTGO1
//#define TTGO2
//#define TTGO21
/////////////////////////////////////////////////////////////////////////////////
// PLEASE SET VERSION and REVISION HERE

#define Version_Major_Minor_Revision "Ver.#2.82.763"


#define GATEWAY_NOT_AVAILABLE (-1000.0f)
///////////////////////////////////////////////////////////////////////////////////
// ID's
#define GATEWAY_INIT_GATEWAY_ID 10
uint8_t  GATEWAY_INIT_GATEWAY_KEY[16] = {0, 1, 2, 3, 4, 5, 6, 7 , 8, 9, 10, 11,  12,  13, 14, 15 };

///////////////////////////////////////////////////////////////////////////////////
// WIFI constants
#define SERVER_PORT 5409
#define SERVER_HOST "server.i4things.com"

#define GATEWAY_INIT_WIFI_SSID "N/A"
#define GATEWAY_INIT_WIFI_PASS "N/A"
#define GATEWAY_INIT_FREQ 868.1f

///////////////////////////////////////////////////////////////////////////////////
// DO EXECUTE test
// will return true only if the fuction is ready for execute

inline bool DO_EXECUTE(uint32_t loopCounter, uint32_t lastExecute, uint32_t timeout)
{
  return (((uint32_t)(loopCounter - lastExecute)) >= timeout);
}

///////////////////////////////////////////////////////////////////////////////////
// HEARTBEAT DATA

//600000
#define HEARTBEAT_TIMEOUT 60000
uint32_t HEARTBEAT_LAST;
#define HEARTBEAT_MAX_MESSAGE_LEN 16
#define OPER_HEARTBEAT_IOT 127
uint8_t OPER_MAGIC_COUNTER = 0;

///////////////////////////////////////////////////////////////////////////////////
// LOG constants
#define LOG64_ENABLED

#include <Log64.h>

#include "gateway_eprom.h"
#include "gateway_pins.h"
#include "gateway_id.h"
#include "IoTGateway.h"
#include "gateway_wifi.h"
#include "gateway_dht.h"
///////////////////////////////////////////////////////////////////////////////////
// LED FLASH

#define FLASH_INTERVAL  500
uint32_t FLASH_LAST;
bool FLASHING;



inline void flash()
{
  if (!FLASHING)
  {
    FLASHING = true;
    FLASH_LAST = millis();
    digitalWrite(LED_PIN, HIGH);
  }
}

IoTGateway *gateway;

// called whne wifi deserialzie a packet
void DISPATCH_RECEIVED(uint8_t buf_[], uint8_t start_index_, uint8_t size_)
{
  gateway->add_dispatch_buffer(buf_, start_index_, size_);
}

// called when packet received from node
void received(uint8_t buf[], uint8_t size, int16_t rssi)
{
  flash();

  uint8_t buf_wifi[128];
  uint8_t size_wifi;

  gateway->gen_internet_message(buf_wifi, size_wifi, buf, size, rssi);


  WIFI_SEND_DATA_ASYNC(buf_wifi, size_wifi);

};

void RF_INIT()
{
  gateway->init(FREQ, false);
};

#include "gateway_oled.h"
#include "gateway_config.h"


///////////////////////////////////////////////////////////////////////////////////
// HEARTBEAT REQUEST

void HEARTBEAT()
{
  gateway->clear_dispatch_buffer();

  uint8_t degree_celsius = (uint8_t)((((DHT_TEMP_GET() == GATEWAY_NOT_AVAILABLE) ? -20 : DHT_TEMP_GET()) + 20.0f) / 0.3137f);

  uint8_t moisture_percent = DHT_HUM_GET();

  uint8_t buf[HEARTBEAT_MAX_MESSAGE_LEN];
  uint8_t size = 0;


  // gen packet
  size = 0;

  uint16_t magic = 6;
  magic = magic << 8;
  magic += (OPER_MAGIC_COUNTER++);

  buf[size++] = 11;

  buf[size++] = OPER_HEARTBEAT_IOT;

  buf[size++] = ((uint8_t*)&magic)[0];
  buf[size++] = ((uint8_t*)&magic)[1];

  buf[size++] = ((uint8_t*)&GATEWAY_ID)[0];
  buf[size++] = ((uint8_t*)&GATEWAY_ID)[1];
  buf[size++] = ((uint8_t*)&GATEWAY_ID)[2];
  buf[size++] = ((uint8_t*)&GATEWAY_ID)[3];

  buf[size++] = ((uint8_t*)&WIFI_LAST_PACKET_TIME)[0];
  buf[size++] = ((uint8_t*)&WIFI_LAST_PACKET_TIME)[1];
  buf[size++] = degree_celsius;
  buf[size++] = moisture_percent;



  WIFI_SEND_DATA_ASYNC(buf, size);
  LOG64_SET("HEARTBEAT INFO ASYNC SENT");
  LOG64_NEW_LINE;


}

void setup()
{

  // Initial delay to give chance to the com port to cennect
  delay(2000);

  // Init random generator
  randomSeed(analogRead(0));

  // Serial Log
  LOG64_INIT();

  // Configuration over serial
  CONFIG_INIT();

  // Print version
  LOG64_SET(Version_Major_Minor_Revision);
  LOG64_NEW_LINE;

  // Id
  ID_INIT();

  //LED flashing
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  FLASHING = false;

#if defined(OLED)
  // Display (if available)
  DISPLAY_INIT();
#endif

  // Radio
  gateway = new IoTGateway(CS_PIN, INT_PIN, RST_PIN, &received, GATEWAY_KEY, GATEWAY_ID);
  gateway->init(FREQ);

  // Wifi
  WIFI_INIT();

  //DHT (temp/Moist)
  DHT_INIT();

  // Heartbeat
  HEARTBEAT();
}


///////////////////////////////////////////////////////////////////////////////////
// MAIN LOOP
void loop()
{
  {
    uint32_t t = millis();
    DHT_();
    uint32_t delta = ((uint32_t)millis()) - t;
    if (delta > 200)
    {
      LOG64_SET("DHT TOOK : ");
      LOG64_SET(delta);
      LOG64_NEW_LINE;
    }
  }

  {
    uint32_t t = millis();

    if (DO_EXECUTE(millis(), HEARTBEAT_LAST, HEARTBEAT_TIMEOUT))
    {
      HEARTBEAT_LAST = millis();

      HEARTBEAT();
    }

    uint32_t delta = ((uint32_t)millis()) - t;
    if (delta > 200)
    {
      LOG64_SET("HEARTBEAT TOOK : ");
      LOG64_SET(delta);
      LOG64_NEW_LINE;
    }
  }

  {
    uint32_t t = millis();
    gateway-> work();
    uint32_t delta = ((uint32_t)millis()) - t;
    if (delta > 200)
    {
      LOG64_SET("GATEWAY TOOK : ");
      LOG64_SET(delta);
      LOG64_NEW_LINE;
    }
  }

  {
    uint32_t t = millis();
    WIFI_();
    uint32_t delta = ((uint32_t)millis()) - t;
    if (delta > 200)
    {
      LOG64_SET("WIFI TOOK : ");
      LOG64_SET(delta);
      LOG64_NEW_LINE;
    }
  }

  {
    uint32_t t = millis();
    CONFIG_(false);
    uint32_t delta = ((uint32_t)millis()) - t;
    if (delta > 200)
    {
      LOG64_SET("CONFIG TOOK : ");
      LOG64_SET(delta);
      LOG64_NEW_LINE;
    }
  }

#if defined(OLED)
  {
    uint32_t t = millis();
    DISPLAY_WORK();
    uint32_t delta = ((uint32_t)millis()) - t;
    if (delta > 200)
    {
      LOG64_SET("OLED TOOK : ");
      LOG64_SET(delta);
      LOG64_NEW_LINE;
    }
  }
#endif

  if ((FLASHING) && (((uint32_t)(((uint32_t)millis()) - FLASH_LAST)) >= FLASH_INTERVAL))
  {
    digitalWrite(LED_PIN, LOW);
    FLASHING = false;
  }
  yield();
}


