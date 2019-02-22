# Gateway

Gateway Configuration Tools and related docs

- Download and install USB driver for your gateway from: https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers

- Download and unzip the gateway software : gateway_heltec.zip or gateway_ttgo.zip ( depending on the hardware that you choose for your gateway )

- Connect the device using USB to micro USB cable.

- Edit the file upload.cmd - to have the correct COM port on which the gateways is attached

- Execute upload.cmd 

- Download and install the configurator from  from this repository ( GatewayUI)


1. Start The configurator
2. Click “Refresh”
3. Click “Connect”
4. Click “Get Configuration”
5. IF all is OK the SSID, PASS, GATEWAY ID, GATEWAY KEY, FREQUENCY should be filled with values.  - if not check the connection.
6. Setup the WiFi configuration: fill SSID and PASS and click “Send  to Gateway”.
7. Setup Gateway Details: fill GATEWAY ID and GATEWAY KEY ( provided from the user area of www.i4things.com) and click "Send to Gateway”.
8. Setup Frequency : fill the FREQUENCY with one of the following : 868.1 , 868.3 or 868.5 ( only this frequencies are supported for private gateway) and click "Send to Gateway”.
9 Finally Click “Get Configuration” and if all is OK the SSID, PASS, GATEWAY ID, GATEWAY KEY, FREQUENCY should have the values you have configured.
10. Restart The Gateway (non mandatory) 

# If you do not have access to Windows computer

'''
MACOS/UNIX - HowTo configure a gateway from console:

1. Find the com port on which the device is connected:

macos: ls /dev/tty.*

linux: ls /dev/ttyACM*

2. Connect a terminal to the com port

macos: screen /dev/tty.board_name 115200

linux: screen /dev/ttyACM0 115200

3. To see the current configuration type:

21041043-7AE3-47E0-85B7-9287181052BC[and press Enter]

4. To send configuration for WIFI:

C4E4D104-5F0F-457F-BA94-88B020531276[and press Enter]
[type ssid><and press Enter]
[type password><and press Enter]

4. To send configuration for GATEWAY ID and KEY
03AD745F-AFA4-41A4-ADC7-119A111A1B47[and press Enter]
[type gateway id][and press Enter]
[type gateway key][and press Enter]

5. To send configuration for FREQUENCY
2009B290-57A9-4ADE-BCA6-902EAC723674[and press Enter]
[type 1146685030 for 868.1Mhz ,type 1146688307 for 868.3Mhz, type 1146691584 for 868.5Mhz][and press Enter]
'''
