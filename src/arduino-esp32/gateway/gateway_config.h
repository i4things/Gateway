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
// CONFIGURATION OVER SERIAL

#define CONFIG_BUF_MAX_SIZE 200
uint8_t CONFIG_BUF[CONFIG_BUF_MAX_SIZE];
uint8_t CONFIG_BUF_SIZE;


#define CONFIG_MAGIC_SIZE 36
char CONFIG_MAGIC_GET[] = "21041043-7AE3-47E0-85B7-9287181052BC";
char CONFIG_MAGIC_GATEWAY[] = "03AD745F-AFA4-41A4-ADC7-119A111A1B47";
char CONFIG_MAGIC_WIFI[] = "C4E4D104-5F0F-457F-BA94-88B020531276";
char CONFIG_MAGIC_FREQ[] = "2009B290-57A9-4ADE-BCA6-902EAC723674";

#define GATEWAY_ID_CODE 1
#define GATEWAY_KEY_CODE 2
#define SSID_CODE 3
#define PASS_CODE 4
#define FREQ_CODE 5

uint8_t NEXT;

inline void CONFIG_INIT()
{
#if not defined(LOG64_ENABLED)
  Serial.begin(115200);
#endif

  CONFIG_BUF_SIZE = 0;
  NEXT = 0;
}

inline uint8_t CONFIG_CHAR_INT(char input)
{
  if (input >= '0' && input <= '9')
  {
    return input - '0';
  }
  else if ((input >= 'A') && (input <= 'F'))
  {
    return input - 'A' + 10;
  }
  else if ((input >= 'a') && (input <= 'f'))
  {
    return input - 'a' + 10;
  }
  return 0;
}

// This function assumes src to be a zero terminated sanitized string with
// an even number of [0-9a-f] characters, and target to be sufficiently large
inline void CONFIG_HEX_TO_KEY(uint8_t src[], uint8_t target[16])
{
  for (int i = 0; i < 16; i++)
  {
    target[i] = (CONFIG_CHAR_INT(src[i << 1]) << 4) | CONFIG_CHAR_INT(src[(i << 1) + 1]);
  }
}

inline bool CONFIG_PARSE(bool no_reconnect)
{
  bool ret = false;

  //  CONFIG_BUF[CONFIG_BUF_SIZE] = 0;
  //  LOG64_SET("@");
  //  LOG64_SET((char *)CONFIG_BUF);
  //  LOG64_NEW_LINE;

  if (CONFIG_BUF_SIZE > 0)
  {
    if ((CONFIG_BUF_SIZE == CONFIG_MAGIC_SIZE) && (memcmp(CONFIG_BUF, CONFIG_MAGIC_GET, CONFIG_BUF_SIZE) == 0))
    {
      // this is get command
      Serial.write((const uint8_t *)CONFIG_MAGIC_GATEWAY, CONFIG_MAGIC_SIZE); Serial.write(13); Serial.write(10);
      Serial.print(GATEWAY_ID); Serial.write(13); Serial.write(10);
      for (uint8_t i = 0; i < 16; i++)
      {
        String bx = String(GATEWAY_KEY[i], HEX);
        if (bx.length() < 2)
        {
          bx = "0" + bx;
        }
        bx.toUpperCase();
        Serial.print(bx);
      }
      Serial.write(13); Serial.write(10);
      Serial.write((const uint8_t *)CONFIG_MAGIC_WIFI, CONFIG_MAGIC_SIZE); Serial.write(13); Serial.write(10);
      Serial.print(WIFI_SSID); Serial.write(13); Serial.write(10);
      Serial.print(WIFI_PASS); Serial.write(13); Serial.write(10);
      Serial.write((const uint8_t *)CONFIG_MAGIC_FREQ, CONFIG_MAGIC_SIZE); Serial.write(13); Serial.write(10);
      union fi
      {
        float f;
        uint32_t i;
      };
      fi f = { FREQ };
      Serial.print(f.i); Serial.write(13); Serial.write(10);
    }
    else if ((CONFIG_BUF_SIZE == CONFIG_MAGIC_SIZE) && (memcmp(CONFIG_BUF, CONFIG_MAGIC_GATEWAY, CONFIG_BUF_SIZE) == 0))
    {
      NEXT = GATEWAY_ID_CODE;
    }
    else if ((CONFIG_BUF_SIZE == CONFIG_MAGIC_SIZE) && (memcmp(CONFIG_BUF, CONFIG_MAGIC_WIFI, CONFIG_BUF_SIZE) == 0))
    {
      NEXT = SSID_CODE;
    }
    else if ((CONFIG_BUF_SIZE == CONFIG_MAGIC_SIZE) && (memcmp(CONFIG_BUF, CONFIG_MAGIC_FREQ, CONFIG_BUF_SIZE) == 0))
    {
      NEXT = FREQ_CODE;
    }
    else
    {
      switch (NEXT)
      {
        case GATEWAY_ID_CODE:
          {
            CONFIG_BUF[CONFIG_BUF_SIZE] = 0;
            uint32_t GATEWAY_ID = (uint32_t)atoi((const char *)CONFIG_BUF);
            ID_SET_GATEWAY_ID(GATEWAY_ID);
            gateway->set_id((uint64_t)GATEWAY_ID);
            NEXT = GATEWAY_KEY_CODE;
          }
          break;
        case GATEWAY_KEY_CODE:
          {
            CONFIG_BUF[CONFIG_BUF_SIZE] = 0;
            uint8_t GATEWAY_KEY_BUF[16];
            CONFIG_HEX_TO_KEY(CONFIG_BUF, GATEWAY_KEY_BUF);
            ID_SET_GATEWAY_KEY(GATEWAY_KEY_BUF);
            gateway->set_key(GATEWAY_KEY_BUF);
            CONFIG_BUF_SIZE = 0;

            if (!no_reconnect)
            {
              // reset everything except wifi
              HEARTBEAT_LAST = millis();
              HEARTBEAT();
            }
            else
            {
              ret = true;
            }

            NEXT = 0;
          }
          break;
        case SSID_CODE:
          {
            CONFIG_BUF[CONFIG_BUF_SIZE] = 0;
            ID_SET_WIFI_SSID((char *) CONFIG_BUF);
            NEXT = PASS_CODE;
          }
          break;
        case PASS_CODE:
          {
            CONFIG_BUF[CONFIG_BUF_SIZE] = 0;
            ID_SET_WIFI_PASS((char *) CONFIG_BUF);
            CONFIG_BUF_SIZE = 0;

            if (!no_reconnect)
            {
              // reset everything
              WIFI_RESET();
              HEARTBEAT_LAST = millis();
              WIFI_INIT();
              RF_INIT();
              HEARTBEAT();
            }
            else
            {
              ret = true;
            }
            NEXT = 0;
          }
          break;

        case FREQ_CODE:
          {
            union fi
            {
              float f;
              uint32_t i;
            };
            fi f;
            CONFIG_BUF[CONFIG_BUF_SIZE] = 0;
            f.i = (uint32_t)atoi((const char *)CONFIG_BUF);
            ID_SET_FREQ(f.f);
            CONFIG_BUF_SIZE = 0;

            if (!no_reconnect)
            {
              // reset everything
              RF_INIT();
            }
            else
            {
              ret = true;
            }
            NEXT = 0;
          }
          break;
        default: NEXT = 0; break;
      }
    }
  }
  CONFIG_BUF_SIZE = 0;
  return ret;
}

// do not make inline as forward definition is required
bool CONFIG_(bool no_reconnect)
{

  bool ret = false;

  if (Serial)
  {
    if (Serial.available() > 0)
    {
      uint8_t tmp = Serial.read();
      if ((tmp == 0) || (tmp == 13) || (tmp == 10))
      {
        ret = CONFIG_PARSE(no_reconnect);
      }
      else
      {
        CONFIG_BUF[CONFIG_BUF_SIZE++] = tmp;

        if (CONFIG_BUF_SIZE == CONFIG_BUF_MAX_SIZE)
        {
          CONFIG_BUF_SIZE = 0;
        }
      }
    }
  }

  return ret;

}

