/**
WSclient.h

MIT License

Copyright (c) 2018 Dominick Lee

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */

#ifndef WEBSOCKETSCLIENT_H_
#define WEBSOCKETSCLIENT_H_

#include "WS.h"

class WebSocketsClient: private WebSockets {
    public:
#ifdef __AVR__
        typedef void (*WebSocketClientEvent)(WStype_t type, uint8_t * payload, size_t length);
#else
        typedef std::function<void (WStype_t type, uint8_t * payload, size_t length)> WebSocketClientEvent;
#endif


        WebSocketsClient(void);
        virtual ~WebSocketsClient(void);

        void begin(const char *host, uint16_t port, const char * url = "/", const char * protocol = "arduino");
        void begin(String host, uint16_t port, String url = "/", String protocol = "arduino");

#if (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266) || (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP32)
        void beginSSL(const char *host, uint16_t port, const char * url = "/", const char * = "", const char * protocol = "arduino");
        void beginSSL(String host, uint16_t port, String url = "/", String fingerprint = "", String protocol = "arduino");
#endif

        void beginSocketIO(const char *host, uint16_t port, const char * url = "/socket.io/?EIO=3", const char * protocol = "arduino");
        void beginSocketIO(String host, uint16_t port, String url = "/socket.io/?EIO=3", String protocol = "arduino");

#if (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266) || (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP32)
        void beginSocketIOSSL(const char *host, uint16_t port, const char * url = "/socket.io/?EIO=3", const char * protocol = "arduino");
        void beginSocketIOSSL(String host, uint16_t port, String url = "/socket.io/?EIO=3", String protocol = "arduino");
#endif

#if (WEBSOCKETS_NETWORK_TYPE != NETWORK_ESP8266_ASYNC)
        void loop(void);
#else
        // Async interface not need a loop call
        void loop(void) __attribute__ ((deprecated)) {}
#endif

        void onEvent(WebSocketClientEvent cbEvent);

        bool sendTXT(uint8_t * payload, size_t length = 0, bool headerToPayload = false);
        bool sendTXT(const uint8_t * payload, size_t length = 0);
        bool sendTXT(char * payload, size_t length = 0, bool headerToPayload = false);
        bool sendTXT(const char * payload, size_t length = 0);
        bool sendTXT(String & payload);

        bool sendBIN(uint8_t * payload, size_t length, bool headerToPayload = false);
        bool sendBIN(const uint8_t * payload, size_t length);

        bool sendPing(uint8_t * payload = NULL, size_t length = 0);
        bool sendPing(String & payload);

        void disconnect(void);

        void setAuthorization(const char * user, const char * password);
        void setAuthorization(const char * auth);
	
        void setExtraHeaders(const char * extraHeaders = NULL);

        void setReconnectInterval(unsigned long time);

    protected:
        String _host;
        uint16_t _port;

#if (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266) || (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP32)
        String _fingerprint;
#endif
        WSclient_t _client;

        WebSocketClientEvent _cbEvent;

        unsigned long _lastConnectionFail;
        unsigned long _reconnectInterval;

        void messageReceived(WSclient_t * client, WSopcode_t opcode, uint8_t * payload, size_t length, bool fin);

        void clientDisconnect(WSclient_t * client);
        bool clientIsConnected(WSclient_t * client);

#if (WEBSOCKETS_NETWORK_TYPE != NETWORK_ESP8266_ASYNC)
        void handleClientData(void);
#endif

        void sendHeader(WSclient_t * client);
        void handleHeader(WSclient_t * client, String * headerLine);

        void connectedCb();
        void connectFailedCb();

#if (WEBSOCKETS_NETWORK_TYPE == NETWORK_ESP8266_ASYNC)
        void asyncConnect();
#endif

        /**
         * called for sending a Event to the app
         * @param type WStype_t
         * @param payload uint8_t *
         * @param length size_t
         */
        virtual void runCbEvent(WStype_t type, uint8_t * payload, size_t length) {
            if(_cbEvent) {
                _cbEvent(type, payload, length);
            }
        }

};

#endif /* WEBSOCKETSCLIENT_H_ */
