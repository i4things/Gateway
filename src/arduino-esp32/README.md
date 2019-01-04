
# Open Source i4things Gateway implementation

folder : gateway - contains code for open source i4things gateway for ESP32 LoRA platforms 

         to compile it you also need install the provided libraries : Log64, DHTesp, RadioHead ( this version is patched to work 
	 correctly on ESP32)
		 
 	you need to decide which platform you want to compile it for : the off-the-shelf supported one are :  Heltec 
	LoRa32, TTGO v1, TTGO v2, TTGO v2,1
	
	please uncomment the one that you want and comment out the ones that you are not using e.g. to get it compiled for Heltec
	LoRa32 with DHT22 and OLED  you need :
		
	// in the beginning of gateway.ino 
		 
	#define OLED
	#define HELTEC
        #define DHT_AVAILABLE
        //#define TTGO1
        //#define TTGO2
        //#define TTGO21
		 
Log64.zip - logging library : https://github.com/vtomanov/Log64

DHTesp.zip - library to work with DHTsensors: https://github.com/beegee-tokyo/DHTesp

RadioHead.zip - patched for ESP32 : https://www.airspayce.com/mikem/arduino/RadioHead/
