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
// DHT ( Humidity/Temp)

#if defined(DHT_AVAILABLE)
#include "DHTesp.h"


// use this to simulate DHT readings
//#define DHT_SIMULATION


#define DHT_TYPE DHTesp::DHT22

DHTesp dht;

//60sec 60 * 1000 (60000)
#define DHT_TIMEOUT  60000
uint32_t DHT_LAST_EXECUTE;

#if not defined(DHT_SIMULATION)
float DHT_TEMP_VALUE;
float DHT_HUM_VALUE;
#endif

#endif

inline void DHT_INIT()
{
#if defined(DHT_AVAILABLE)
  //important make sure DHT is initialized
  dht.setup(DHT_PIN, DHTesp::DHT22); // Connect DHT sensor to GPIO 17
  delay(2000);

  DHT_LAST_EXECUTE = millis();

#if defined(DHT_SIMULATION)
  LOG64_SET(F("DHT: INIT"));
#else

  DHT_TEMP_VALUE = GATEWAY_NOT_AVAILABLE;
  DHT_HUM_VALUE = GATEWAY_NOT_AVAILABLE;


  float h =  dht.getHumidity();
  float t = dht.getTemperature();


  if (isnan(h) || isnan(t))
  {
    LOG64_SET(F("DHT: FAIED"));
  }
  else
  {
    DHT_TEMP_VALUE = t;
    DHT_HUM_VALUE = (uint8_t) h;
    LOG64_SET(F("DHT: INIT"));
  }

#endif

  LOG64_NEW_LINE;
#endif
}

inline void DHT_()
{
#if defined(DHT_AVAILABLE)
#if not defined(DHT_SIMULATION)
  if (DO_EXECUTE(millis(), DHT_LAST_EXECUTE, DHT_TIMEOUT))
  {
    DHT_LAST_EXECUTE = millis();


    float h =  dht.getHumidity();
    float t = dht.getTemperature();

    if ((isnan(h)) || (isnan(t)))
    {
      LOG64_SET("DHT: READ FAILED");
      LOG64_NEW_LINE;
    }
    else
    {

      if (DHT_HUM_VALUE != GATEWAY_NOT_AVAILABLE)
      {
        DHT_HUM_VALUE = (uint8_t)((((uint8_t)DHT_HUM_VALUE) + ((uint8_t) h)) / 2);
      }
      else
      {
        DHT_HUM_VALUE = (uint8_t) h;
      }

      if (DHT_TEMP_VALUE != GATEWAY_NOT_AVAILABLE)
      {
        DHT_TEMP_VALUE = (DHT_TEMP_VALUE + t) / 2;
      }
      else
      {
        DHT_TEMP_VALUE = t;
      }

    }

    //    LOG64_SET("DHT: T[");
    //    LOG64_SET(String(DHT_TEMP_VALUE, 1));
    //    LOG64_SET("] H[");
    //    LOG64_SET(DHT_HUM_VALUE);
    //    LOG64_SET("]");
    //    LOG64_NEW_LINE;

  }
#endif
#endif
}

inline uint8_t DHT_HUM_GET()
{
#if not defined(DHT_AVAILABLE)
  return 0;
#else
#if not defined(DHT_SIMULATION)
  if (DHT_HUM_VALUE == GATEWAY_NOT_AVAILABLE)
  {
    return 0;
  }

  return DHT_HUM_VALUE;
#else
  return  (uint8_t)random(40, 80);
#endif
#endif
}

inline float DHT_TEMP_GET()
{

#if not defined(DHT_AVAILABLE)
  return -20.0f;
#else
#if not defined(DHT_SIMULATION)
  return DHT_TEMP_VALUE;
#else
  return ((float)random(100, 230)) / 10.0f;
#endif
#endif
}




