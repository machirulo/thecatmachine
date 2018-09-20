# thecatmachine
The Cat Machine: Smart Iot Moderneishon Useless Device
This is an experimental IoT device intended to show free available information from REST APIs that shows dollar/$ARS exchange, economic ratios, date, time and weather conditions.

The purpose of this device is merelly experimental and recreational. There is no political intention behind it (btw, you can print just the case, or design another one).

IMPORTANT INFORMATION: The model on top is not my design, credits are from: https://www.thingiverse.com/thing:2490305

There is no PCB design, but you can make one.

The code is not optimized and is very crap.

All the universe is not yours.

There are two versions of the case and the code: One where you can use a OLED display see pictures) SSD1306, and the other one (which I prefer) with a 16x2 i2c display. Pay attention to the voltage needed in this case (5V). See diagram.

All APIs are property of their respective authors, and are free for access (In cases where this is not the case, it is indicated in the code that the user must register).

You'll need to install some libraries in Arduino IDE, such as Wifi Manager, and those for LCD16x2 I2c and ESP8266Wifi.

I do not assume any responsibility for any undue or malicious use of this device, nor for any damage that could be caused to people or electronic components, parts, etc. Build it under your own responsibility.

You'll need a NODEMCU 1.0 (ESP8266) and optionally a ISD1820 Module and two Leds. All is mounted in a protoboard. Power can be obtained from a power bank.

Usage: At first time, you have to connect to the Access Point created by the device, go to a browser and write "192.168.4.1", then scan the Wi-fi Networks, and save the SSID and Password, turn off the device and turn it on. Thats all.

Schematics:  

![PUTE LE QUE LEE](machirulo/thecatmachine/blob/master/Images/Esquema-conexion.png)
