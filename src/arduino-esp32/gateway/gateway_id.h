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
// EGATEWAY ID

#include <EEPROM.h>

uint32_t GATEWAY_ID;
float FREQ;

char WIFI_SSID[WIFI_SSDI_MAX + 1];
char WIFI_PASS[WIFI_PASS_MAX + 1];
uint8_t GATEWAY_KEY[16];


inline void ID_REINIT()
{
  LOG64_SET(F("ID: REINIT"));
  LOG64_NEW_LINE;

  union fb4
  {
    float f;
    uint8_t b[4];
  };

  fb4 fr = { GATEWAY_INIT_FREQ };

  EEPROM.put(EEPROM_ADDRESS_GATEWAY_ID, (uint32_t)GATEWAY_INIT_GATEWAY_ID);
  //EEPROM.put(EEPROM_ADDRESS_GATEWAY_KEY, GATEWAY_INIT_GATEWAY_KEY);
  for (uint8_t i = 0; i < 16; i++)
  {
    EEPROM.put(EEPROM_ADDRESS_GATEWAY_KEY + i, GATEWAY_INIT_GATEWAY_KEY[i]);
  }
  EEPROM.put(EEPROM_ADDRESS_FREQ, fr.b);
  EEPROM.put(EEPROM_ADDRESS_WIFI_SSID, GATEWAY_INIT_WIFI_SSID);
  EEPROM.put(EEPROM_ADDRESS_WIFI_PASS, GATEWAY_INIT_WIFI_PASS);
  EEPROM.commit();
  EEPROM.end();
}

inline void ID_INIT()
{
  //  EEPROM.begin(EEPROM_SIZE);
  //  ID_REINIT();

  union fb4
  {
    float f;
    uint8_t b[4];
  };

  fb4 fr;

  EEPROM.begin(EEPROM_SIZE);
  EEPROM.get(EEPROM_ADDRESS_GATEWAY_ID, GATEWAY_ID);
  EEPROM.get(EEPROM_ADDRESS_GATEWAY_KEY, GATEWAY_KEY);
  EEPROM.get(EEPROM_ADDRESS_FREQ, fr.b);
  EEPROM.get(EEPROM_ADDRESS_WIFI_SSID, WIFI_SSID);
  EEPROM.get(EEPROM_ADDRESS_WIFI_PASS, WIFI_PASS);
  FREQ = fr.f;

  if ((GATEWAY_ID == 0xFFFF) || (WIFI_SSID[0] == 0xFF) || (WIFI_PASS[0] == 0xFF) || (fr.b[0] == 0xFF))
  {
    ID_REINIT();
    EEPROM.begin(EEPROM_SIZE);
    EEPROM.get(EEPROM_ADDRESS_GATEWAY_ID, GATEWAY_ID);
    EEPROM.get(EEPROM_ADDRESS_GATEWAY_KEY, GATEWAY_KEY);
    EEPROM.get(EEPROM_ADDRESS_FREQ, fr.b);
    EEPROM.get(EEPROM_ADDRESS_WIFI_SSID, WIFI_SSID);
    EEPROM.get(EEPROM_ADDRESS_WIFI_PASS, WIFI_PASS);
    FREQ = fr.f;
  }

  LOG64_SET(F("ID: INIT: GATEWAY_ID["));
  LOG64_SET(GATEWAY_ID);
  LOG64_SET(F("] GATEWAY_KEY["));
  for (uint8_t i = 0; i < 16; i++)
  {
    String bx = String(GATEWAY_KEY[i], HEX);
    if (bx.length() < 2)
    {
      bx = "0" + bx;
    }
    bx.toUpperCase();
    LOG64_SET(bx);
  }
  LOG64_SET("] FREQ[");
  LOG64_SET(FREQ);
  LOG64_SET("] WIFI_SSID[");
  LOG64_SET(WIFI_SSID);
  LOG64_SET("] WIFI_PASS[");
  LOG64_SET(WIFI_PASS);
  LOG64_SET("]");
  LOG64_NEW_LINE;
}

inline void ID_SET_GATEWAY_ID(uint32_t gateway_id_)
{
  EEPROM.end();
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.put(EEPROM_ADDRESS_GATEWAY_ID, gateway_id_);
  EEPROM.commit();
  EEPROM.end();
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.get(EEPROM_ADDRESS_GATEWAY_ID, GATEWAY_ID);

  LOG64_SET("ID: ID_SET_GATEWAY_ID: GATEWAY_ID[");
  LOG64_SET(GATEWAY_ID);
  LOG64_SET("]");
  LOG64_NEW_LINE;
}

inline void ID_SET_GATEWAY_KEY(uint8_t gateway_key_[16])
{
  EEPROM.end();
  EEPROM.begin(EEPROM_SIZE);
  //EEPROM.put(EEPROM_ADDRESS_GATEWAY_KEY, gateway_key_);
  for (uint8_t i = 0; i < 16; i++)
  {
    EEPROM.put(EEPROM_ADDRESS_GATEWAY_KEY + i, gateway_key_[i]);
  }
  EEPROM.commit();
  EEPROM.end();
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.get(EEPROM_ADDRESS_GATEWAY_KEY, GATEWAY_KEY);

  LOG64_SET("ID: ID_SET_GATEWAY_KEY: GATEWAY_KEY[");
  for (uint8_t i = 0; i < 16; i++)
  {
    String bx = String(GATEWAY_KEY[i], HEX);
    if (bx.length() < 2)
    {
      bx = "0" + bx;
    }
    bx.toUpperCase();
    LOG64_SET(bx);
  }
  LOG64_SET("]");
  LOG64_NEW_LINE;
}

inline void ID_SET_FREQ(float freq_)
{
  union fb4
  {
    float f;
    uint8_t b[4];
  };

  fb4 fr = { freq_ };

  EEPROM.end();
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.put(EEPROM_ADDRESS_FREQ, fr.b);
  EEPROM.commit();
  EEPROM.end();
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.get(EEPROM_ADDRESS_FREQ, fr.b);
  FREQ = fr.f;

  LOG64_SET(F("ID: ID_SET_FREQ: FREQ["));
  LOG64_SET(FREQ);
  LOG64_SET(F("]"));
  LOG64_NEW_LINE;
}

inline void ID_SET_WIFI_SSID(char * buf)
{
  char wifi_ssid[WIFI_SSDI_MAX + 1];
  memcpy(wifi_ssid, buf, WIFI_SSDI_MAX + 1);

  EEPROM.end();
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.put(EEPROM_ADDRESS_WIFI_SSID, wifi_ssid);
  EEPROM.commit();
  EEPROM.end();
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.get(EEPROM_ADDRESS_WIFI_SSID, WIFI_SSID);

  LOG64_SET(F("ID: ID_SET_WIFI_SSID: WIFI_SSID["));
  LOG64_SET(WIFI_SSID);
  LOG64_SET(F("]"));
  LOG64_NEW_LINE;
}

inline void ID_SET_WIFI_PASS(char * buf)
{
  char wifi_pass[WIFI_PASS_MAX + 1];
  memcpy(wifi_pass, buf, WIFI_PASS_MAX + 1);

  EEPROM.end();
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.put(EEPROM_ADDRESS_WIFI_PASS, wifi_pass);
  EEPROM.commit();
  EEPROM.end();
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.get(EEPROM_ADDRESS_WIFI_PASS, WIFI_PASS);

  LOG64_SET(F("ID: ID_SET_WIFI_PASS: WIFI_PASS["));
  LOG64_SET(WIFI_PASS);
  LOG64_SET(F("]"));
  LOG64_NEW_LINE;
}

