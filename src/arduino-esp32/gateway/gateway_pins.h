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
// All pin defs here



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// HELTEC
#if defined(HELTEC)

#define CS_PIN 18
#define RST_PIN 14
#define INT_PIN 26

#define DHT_PIN 23

#define LED_PIN 25

#if defined(OLED)

#define DISPLAY_ADDR 0x3c
#define DISPLAY_SDA 4
#define DISPLAY_SCL 15
#define DISPLAY_GEOMETRY GEOMETRY_128_64
#define DISPLAY_RESET_PIN 16

#endif

#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TTGO1
#if defined(TTGO1)

#define CS_PIN 18
#define RST_PIN 14
#define INT_PIN 26

#define DHT_PIN 34

#define LED_PIN 23

#if defined(OLED)

#define DISPLAY_ADDR 0x3c
#define DISPLAY_SDA 4
#define DISPLAY_SCL 15
#define DISPLAY_GEOMETRY GEOMETRY_128_64
#define DISPLAY_RESET_PIN 16

#endif

#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TTGO 2.1
#if defined(TTGO21)

#define CS_PIN 18
#define RST_PIN 23
#define INT_PIN 26

#define DHT_PIN 34

#define LED_PIN 25

#if defined(OLED)

#define DISPLAY_ADDR 0x3c
#define DISPLAY_SDA 21
#define DISPLAY_SCL 22
#define DISPLAY_GEOMETRY GEOMETRY_128_64
#define DISPLAY_RESET_PIN 16

#endif

#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TTGO 2.0
#if defined(TTGO2)

#define CS_PIN 18
#define RST_PIN 9
#define INT_PIN 26

#define DHT_PIN 34

#define LED_PIN 23

#if defined(OLED)

#define DISPLAY_ADDR 0x3c
#define DISPLAY_SDA 21
#define DISPLAY_SCL 22
#define DISPLAY_GEOMETRY GEOMETRY_128_64
#define DISPLAY_RESET_PIN 16

#endif

#endif








