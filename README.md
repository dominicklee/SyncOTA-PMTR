SyncOTA for Purdue Mechatronics
===========================================

An implementation of Makitronics SyncOTA for Purdue Mechatronics Club. Based on websocket compliance RFC6455 standard.


##### Supported features of RFC6455 #####
 - text frame
 - binary frame
 - connection close
 - ping
 - pong
 - continuation frame

##### Limitations #####
 - max input length is limited to the ram size and the ```WEBSOCKETS_MAX_DATA_SIZE``` define
 - max output length has no limit (the hardware is the limit)
 - Client send big frames with mask 0x00000000 (on AVR all frames)
 - continuation frame reassembly need to be handled in the application code

##### Supported Hardware #####
 - ESP8266 [Arduino for ESP8266](https://github.com/esp8266/Arduino/)
 - ESP32 [Arduino for ESP32](https://github.com/espressif/arduino-esp32)

###### Note: ######

  version 2.0 and up is not compatible with AVR/ATmega, check ATmega branch.

  Arduino for AVR not supports std namespace of c++.

### wss / SSL ###
 supported for:
 - wss client on the ESP8266
 - wss / SSL is not natively supported in WebSocketsServer however it is possible to achieve secure websockets
   by running the device behind an SSL proxy. See [Nginx](examples/Nginx/esp8266.ssl.reverse.proxy.conf) for a
   sample Nginx server configuration file to enable this.

### ESP Async TCP ###

This libary can run in Async TCP mode on the ESP.

The mode can be activated in the ```WebSockets.h``` (see WEBSOCKETS_NETWORK_TYPE define).

[ESPAsyncTCP](https://github.com/me-no-dev/ESPAsyncTCP) libary is required.


### High Level Client API ###

 - `begin` : Initiate connection sequence to the websocket host.
```
void begin(const char *host, uint16_t port, const char * url = "/", const char * protocol = "arduino");
void begin(String host, uint16_t port, String url = "/", String protocol = "arduino");
 ```
 - `onEvent`: Callback to handle for websocket events

 ```
 void onEvent(WebSocketClientEvent cbEvent);
 ```

 - `WebSocketClientEvent`: Handler for websocket events
 ```
 void (*WebSocketClientEvent)(WStype_t type, uint8_t * payload, size_t length)
 ```
Where `WStype_t type` is defined as:
  ```
  typedef enum {
      WStype_ERROR,
      WStype_DISCONNECTED,
      WStype_CONNECTED,
      WStype_TEXT,
      WStype_BIN,
  	WStype_FRAGMENT_TEXT_START,
  	WStype_FRAGMENT_BIN_START,
  	WStype_FRAGMENT,
  	WStype_FRAGMENT_FIN,
  } WStype_t;
  ```


### License and credits ###

Based on [arduinoWebSockets](https://github.com/Links2004/arduinoWebSockets/issues) implementation.

[libb64](http://libb64.sourceforge.net/) written by Chris Venter. It is distributed under Public Domain see [LICENSE](https://github.com/Links2004/arduinoWebSockets/blob/master/src/libb64/LICENSE).
