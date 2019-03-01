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
// WIFI


#include <WiFi.h>
#define ESP32_MAX_PACKET_WAIT 1000

// FORWARD
bool CONFIG_(bool no_reconnect);
void RF_INIT();
void HEARTBEAT();

// wait for response 10 sec
bool WIFI_WAIT;
uint32_t WIFI_WAIT_EXECUTE;
#define WIFI_WAIT_TIMEOUT  1000

// 1sec
uint32_t WIFI_LAST_EXECUTE;
uint32_t WIFI_TIMEOUT;
#define WIFI_DEFAULT_TIMEOUT 1000
// 3o sec
#define WIFI_EXTENDED_TIMEOUT 30000

// 60 sec
uint32_t WIFI_CONNECT_LAST_EXECUTE;
uint32_t WIFI_CONNECT_TIMEOUT;
//60000
#define WIFI_CONNECT_TIMEOUT_DEFAULT  60000

// 7 sec over 7 sec for establishing connection we do not considered it internet
// as the lost of messages wil start at 7 sec for establishing connection
//Local (~15ms RTT) - 22.5ms
//Trans-continental (~80ms RTT) - 120ms
//Inter-continental (~150ms RTT) -  225ms
//Satellite (~500ms RTT) - 750ms4e az g
//uint32_t WIFI_CLIENT_CONNECT_LAST_EXECUTE;
//uint32_t WIFI_CLIENT_CONNECT_TIMEOUT;
//#define WIFI_CLIENT_CONNECT_TIMEOUT_DEFAULT 7000

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//QUEUE SEND

#define WIFI_BUF_MAX_SIZE 1024

uint8_t WIFI_QUEUE_BUF[WIFI_BUF_MAX_SIZE];
uint16_t WIFI_BUF_SIZE;

uint8_t WIFI_QUEUE_BUF_IN_NETWORK[WIFI_BUF_MAX_SIZE];
uint16_t WIFI_BUF_SIZE_IN_NETWORK;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//DECODE RECEIVE
uint8_t WIFI_DECODE_BUF[WIFI_BUF_MAX_SIZE];
uint16_t WIFI_DECODE_SIZE = 0;

WiFiClient client;
uint16_t WIFI_LAST_PACKET_TIME;

inline uint8_t WIFI_STRENGTH()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    return 0;
  }
  int dBm = WiFi.RSSI();
  if (dBm <= -100)
  {
    return 0;
  }
  if (dBm >= -50)
  {
    return 100;
  }
  return 2 * (dBm + 100);
}

inline void WIFI_DISCONNECT()
{
  delay(10);

  WiFi.disconnect();
  WiFi.mode(WIFI_OFF); //workaround
  uint32_t my_start = millis();
  while ( ( WiFi.status() == WL_CONNECTED ) &&  ((((uint32_t)((uint32_t)millis()) - my_start)) < ((uint32_t)10000)) )
  {
    yield();
  }
  WiFi.mode(WIFI_STA);
}

inline void WIFI_RESET()
{
  //clear
  client.stop();
  WIFI_DISCONNECT();
}

inline void WIFI_INIT()
{
  WiFi.persistent (false);

  WIFI_WAIT = false;

  WIFI_TIMEOUT = WIFI_DEFAULT_TIMEOUT;

  WIFI_CONNECT_TIMEOUT = 0;
  WIFI_CONNECT_LAST_EXECUTE = millis();
  WIFI_LAST_EXECUTE = millis();

  WIFI_BUF_SIZE = 0;
  WIFI_BUF_SIZE_IN_NETWORK = 0;
  WIFI_DECODE_SIZE = 0;

  WIFI_LAST_PACKET_TIME = 0;

  WiFi.mode(WIFI_STA);

  WiFi.setAutoConnect(false);
  //WiFi.setAutoReconnect(false);

  LOG64_SET("Connecting to SSID[");
  LOG64_SET(WIFI_SSID);
  LOG64_SET("] PASS[");
  LOG64_SET(WIFI_PASS);
  LOG64_SET("] <");
  LOG64_NEW_LINE;

  WIFI_DISCONNECT();
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  uint32_t my_start = millis();
  uint32_t print_last_execute = millis();
  uint8_t print_sec = 1;

  bool re_init = false;

  if ((strcmp(GATEWAY_INIT_WIFI_SSID, WIFI_SSID) != 0) || (strcmp(GATEWAY_INIT_WIFI_PASS, WIFI_PASS) != 0))
  {
    while ( ( WiFi.status() != WL_CONNECTED ) &&  ((((uint32_t)((uint32_t)millis()) - my_start)) < ((uint32_t)30000)) )
    {
      yield();
      if (CONFIG_(true))
      {
        re_init = true;
        break;
      }

      if (DO_EXECUTE(millis(), print_last_execute, 1000))
      {
        print_last_execute = millis();

        LOG64_SET(print_sec++);
        LOG64_NEW_LINE;
      }
    }
  }

  if (WiFi.status() != WL_CONNECTED)
  {
    LOG64_SET("> Can't connect for 30sec");
    LOG64_NEW_LINE;
  }
  else
  {
    LOG64_SET("> Connected for");
    LOG64_SET((uint32_t)((((uint32_t)millis()) - ((uint32_t)my_start)) / ((uint32_t)1000)));
    LOG64_SET("sec IP[");
    LOG64_SET(String(WiFi.localIP()[0]));
    LOG64_SET(".");
    LOG64_SET(String(WiFi.localIP()[1]));
    LOG64_SET(".");
    LOG64_SET(String(WiFi.localIP()[2]));
    LOG64_SET(".");
    LOG64_SET(String(WiFi.localIP()[3]));
    LOG64_SET("]");
    LOG64_NEW_LINE;
  }



  LOG64_SET("WIFI: INIT");
  LOG64_NEW_LINE;

  LOG64_SET("Version: ");

  LOG64_SET(ESP.getSdkVersion());

  LOG64_NEW_LINE;

  if (re_init)
  {
    // reset everything
    WIFI_RESET();
    HEARTBEAT_LAST = millis();
    WIFI_INIT();
    RF_INIT();
    HEARTBEAT();
  }
}

inline void WIFI_SEND_DATA_ASYNC(uint8_t buf[], uint8_t size)
{
  if ((WIFI_BUF_SIZE + ((uint16_t)size)) > WIFI_BUF_MAX_SIZE)
  {
    LOG64_SET("WIFI: SEND_DATA ASYNC : NO SPACE : OLD DATA CLEARED");
    LOG64_NEW_LINE;
    WIFI_BUF_SIZE = 0;
  }

  memcpy((void *) & WIFI_QUEUE_BUF[WIFI_BUF_SIZE], (void *) buf, size);
  WIFI_BUF_SIZE += ((uint16_t)size);
}


inline void WIFI_TRY_DECODE()
{
  for (; client.available() > 0;)
  {
    WIFI_DECODE_BUF[WIFI_DECODE_SIZE++] = client.read();
      if (WIFI_DECODE_SIZE == 1)
    {
      if (WIFI_DECODE_BUF[0] > IoTGateway_MAX_MESSAGE_LEN)
      {
        //max length check
        LOG64_SET("WIFI TRY DECODE : UP TO 51 BYTES FOR PACKET SIZE IS SUPPORTED - RESET CONNECTION");
        LOG64_NEW_LINE;
        //clear
        // clear if there is some old response
        for (; client.available() > 0;)
        {
          client.read();
          yield();
        }
        client.flush();
        client.stop();
        WIFI_DECODE_SIZE = 0;
        return;
      }
    }

    // process response
    if (WIFI_DECODE_SIZE == (WIFI_DECODE_BUF[0] + 1))
    {
      // all other will have size > 5
      if  (WIFI_DECODE_SIZE == 5)
      {
        uint32_t server_time;
        ((uint8_t*)&server_time)[0] = WIFI_DECODE_BUF[1];
        ((uint8_t*)&server_time)[1] = WIFI_DECODE_BUF[2];
        ((uint8_t*)&server_time)[2] = WIFI_DECODE_BUF[3];
        ((uint8_t*)&server_time)[3] = WIFI_DECODE_BUF[4];

        LOG64_SET("WIFI TRY DECODE : SERVER TIME[");
        LOG64_SET((uint32_t)server_time);
        LOG64_SET("]");
        LOG64_NEW_LINE;
      }
      else
      {
        DISPATCH_RECEIVED(WIFI_DECODE_BUF, 1, WIFI_DECODE_SIZE - 1);
      }

      WIFI_DECODE_SIZE = 0;

      // reset waiting - as we have recived at least one packet
      WIFI_WAIT = false;
    }

    yield();
  }
}

inline void WIFI_()
{

  WIFI_TRY_DECODE();


  if (WIFI_WAIT)
  {
    if (DO_EXECUTE(millis(), WIFI_WAIT_EXECUTE, WIFI_WAIT_TIMEOUT))
    {
      WIFI_LAST_EXECUTE = millis();
      LOG64_SET("WIFI: RESET CONNECTION : SYNC NOT RECEIVED");
      LOG64_NEW_LINE;
      //clear
      //clear
      // clear if there is some old response
      for (; client.available() > 0;)
      {
        client.read();
        yield();
      }
      client.flush();
      client.stop();

      //WIFI_RESET();

      WIFI_WAIT = false;

      WIFI_LAST_EXECUTE = millis();

      return;
    }
  }

  if ( WiFi.status() != WL_CONNECTED)
  {

    if (DO_EXECUTE(millis(), WIFI_CONNECT_LAST_EXECUTE, WIFI_CONNECT_TIMEOUT))
    {

      WIFI_CONNECT_LAST_EXECUTE = millis();
      WIFI_CONNECT_TIMEOUT = WIFI_CONNECT_TIMEOUT_DEFAULT;

      LOG64_SET("WIFI : Schedule connecting to SSID[");
      LOG64_SET(WIFI_SSID);
      LOG64_SET("] PASS[");
      LOG64_SET(WIFI_PASS);
      LOG64_SET("]");
      LOG64_NEW_LINE;

      WIFI_DISCONNECT();
      WiFi.begin(WIFI_SSID, WIFI_PASS);
    }
  }
  else
  {

    WIFI_CONNECT_TIMEOUT = 0;
  }



  if (DO_EXECUTE(millis(), WIFI_LAST_EXECUTE, WIFI_TIMEOUT) && (!WIFI_WAIT))
  {
    WIFI_LAST_EXECUTE = millis();
    // make sure we are still connected to WIFI
    if ( WiFi.status() == WL_CONNECTED)
    {
      // check if the connection is established
      if (!client.connected())
      {
        //clear
        // clear if there is some old response
        for (; client.available() > 0;)
        {
          client.read();
          yield();
        }
        client.flush();
        client.stop();

        LOG64_SET("WIFI : RECONNECT : TRY CONNECT TO SERVER...");
        LOG64_NEW_LINE;

        if (!client.connect(SERVER_HOST, SERVER_PORT))
        {
          LOG64_SET("WIFI : RECONNECT : CAN'T CONNECT TO SERVER[");
          LOG64_SET(SERVER_HOST);
          LOG64_SET(":");
          LOG64_SET(SERVER_PORT);
          LOG64_SET("]");
          LOG64_NEW_LINE;

          //WIFI_RESET();

          WIFI_WAIT = false;

          WIFI_TIMEOUT = WIFI_EXTENDED_TIMEOUT;

          return;
        }
        else
        {
          WIFI_TIMEOUT = WIFI_DEFAULT_TIMEOUT;
          client.setNoDelay(true);
          LOG64_SET("WIFI : CONNECTED HOST:PORT[");
          LOG64_SET(SERVER_HOST);
          LOG64_SET(":");
          LOG64_SET(SERVER_PORT);
          LOG64_SET("]");
          LOG64_NEW_LINE;

          // clear if theer is some old response
          for (; client.available() > 0;)
          {
            client.read();
            yield();
          }

          client.flush();

          WIFI_DECODE_SIZE = 0;

          HEARTBEAT_LAST = millis();

          RF_INIT();
          HEARTBEAT();
        }
      }
      else
      {
        // clear the in_network buffer
        WIFI_BUF_SIZE_IN_NETWORK = 0;
      }

      if (WIFI_BUF_SIZE_IN_NETWORK > 0)
      {
        // we still have something to send from previouse disconnect
        if ((WIFI_BUF_SIZE_IN_NETWORK + WIFI_BUF_SIZE) <= WIFI_BUF_MAX_SIZE)
        {
          // we have space lets insert it
          LOG64_SET("WIFI: MOVING IN NETWORK BUFFER BACK[");
          LOG64_SET((uint16_t)WIFI_BUF_SIZE_IN_NETWORK);
          LOG64_SET("]");
          LOG64_NEW_LINE;

          memmove((void *) & WIFI_QUEUE_BUF[WIFI_BUF_SIZE_IN_NETWORK], (void *) WIFI_QUEUE_BUF, WIFI_BUF_SIZE);
          memcpy((void *) WIFI_QUEUE_BUF, (void *) WIFI_QUEUE_BUF_IN_NETWORK, WIFI_BUF_SIZE_IN_NETWORK);
          WIFI_BUF_SIZE += WIFI_BUF_SIZE_IN_NETWORK;;
        }
        else
        {
          // we have space lets insert it
          LOG64_SET("WIFI: NO SPACE LEFT IN THE WIFI BUFFER - CLEANING NETWORK BUFFER");
          LOG64_NEW_LINE;
        }
        // clean netwrok buffer
        WIFI_BUF_SIZE_IN_NETWORK = 0;
      }


      if (WIFI_BUF_SIZE > 0)
      {
        // version 2.3.0 with update for async
        //if (!client.isSendWaiting())

        // version 2.4.1
        // for ESP32 skip and try multiple retry manually
        {
          // copy current buffer to in_network buffer
          memcpy((void *) WIFI_QUEUE_BUF_IN_NETWORK, (void *) WIFI_QUEUE_BUF, WIFI_BUF_SIZE);
          WIFI_BUF_SIZE_IN_NETWORK = WIFI_BUF_SIZE;

          // try write
          bool WRITE_NOT_OK = true;
          for (uint32_t WRITE_START = (uint32_t)millis();;)
          {
            uint16_t res = client.write((const uint8_t *)WIFI_QUEUE_BUF, WIFI_BUF_SIZE);

            LOG64_SET("WIFI: BYTES WRITTEN[");
            LOG64_SET(res);
            LOG64_SET("]");
            LOG64_NEW_LINE;

            if (res == WIFI_BUF_SIZE)
            {
              WRITE_NOT_OK = false;
              break;
            }
            else if (((uint32_t) (((uint32_t)millis()) -  WRITE_START)) > ESP32_MAX_PACKET_WAIT )
            {
              break;
            }

            // compact buffer and try again
            if (res > 0)
            {
              memmove((void *) & WIFI_QUEUE_BUF[res], (void *) WIFI_QUEUE_BUF, res);
              WIFI_BUF_SIZE -= res;
            }
            yield();
          }

          if (WRITE_NOT_OK)
          {
            LOG64_SET("WIFI: SEND DATA : BYTE CANNOT BE WRITTEN IN ONE GO MESSAGE SKIPPED");
            LOG64_NEW_LINE;

            //clear
            // clear if there is some old response
            for (; client.available() > 0;)
            {
              client.read();
              yield();
            }
            client.flush();
            client.stop();

            //WIFI_RESET();

          }
          else
          {
            WIFI_WAIT = true;
            WIFI_WAIT_EXECUTE = millis();

            LOG64_SET("WIFI: SEND DATA :  SIZE[");
            LOG64_SET(WIFI_BUF_SIZE);
            LOG64_SET("] WAITING SYNC");
            LOG64_NEW_LINE;

          }

          // clean buff
          WIFI_BUF_SIZE = 0;
        }
      }
    }
  }
}


