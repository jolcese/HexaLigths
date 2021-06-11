# Inspiration
* Based on ESP8266 Fastled Webserver by Jason Coon: https://github.com/jasoncoon/esp8266-fastled-webserver
* Based on ColorTwinkles by Mark Kriegsman: https://gist.github.com/kriegsman/5408ecd397744ba0393e
* Based on TwinkleFOX by Mark Kriegsman: https://gist.github.com/kriegsman/756ea6dcae8e30845b5a
* Based on DemoReel100 by FastLED: https://github.com/FastLED/FastLED/blob/master/examples/DemoReel100/DemoReel100.ino

# Dependencies
* Boards
  * ESP32 1.0.6 https://github.com/espressif/arduino-esp32
* Libraries
  * FastLED 3.4.0 https://github.com/FastLED/FastLED
  * Websockets 2.3.5 https://github.com/Links2004/arduinoWebSockets
  * Wifi Manager 2.0.3-alpha https://github.com/tzapu/WiFiManager
* Tools
  * SPIFFS uploader 1.0: https://github.com/me-no-dev/arduino-esp32fs-plugin/releases/

# Notes
* For local HTML/JS development, you can start a webserver on the data directory (i.e. `python3 -m http.server`) and create a `remote.json` file with these contents: `{"ip":"192.168.30.15"}` 

# To Do
* HTML 
  * Handle delaypostvalue
  * Fix Hue to ColorSlider
  * ~~Implement websockets and emit~~
  * ~~Fix SolidColor~~
* EEPROM
  * ~~Provide clean mechanism (delete Magic value)~~
  * ~~Add pattern specific variables~~
  * ~~Reenable~~
  * ~~Add more variables~~
* Wifi
  * Test first connection (Does MDNS work?)
  * Update firmware OTA
  * ~~Provide credentials cleanup mechanism~~
  * ~~MDNS~~
* Patterns
  * FRAMES_PER_SECOND
  * rainbow all around edge
  * ~~Add independent palette to different patterns~~
  * ~~More pattern (twinkle fox, pride)~~
  * ~~Autoplay~~
* Rearchitect for component isolation (each pattern in a self contasined class, no dependencies between storage, wifi, fields, etc)
* Better REST Logging
* Remove SolidColor endpoint
* Fix ColorFieldType
* palettes and palettesNames in same structure
* ~~Fix Purple and Green~~
* ~~Per pattern variables~~
* ~~Unify gHostNameString and gHostNameChar~~
* ~~Turn off red led (hardwired)~~
* ~~Add bounds check on SetPattern and SetPalette~~



