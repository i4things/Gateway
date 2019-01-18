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
