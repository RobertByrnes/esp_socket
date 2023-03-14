#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>
#include <WebSocketsClient.h>

WiFiMulti wiFiMulti;
WebSocketsClient webSocket;

// #define DEBUG_ESP_SOCKET_CLIENT

#if defined(DEBUG_ESP_SOCKET_CLIENT)
#define DEBUG_SOCKET(format, ...)                Serial.printf(const char *format, ...)
#else
#define DEBUG_SOCKET(format, ...)
#endif

std::string UPDATE = "update";
std::string REBOOT = "reboot";
std::string PROVISION = "provision";
std::string RUNTIME = "runtime";

void hexdump(const void *mem, uint32_t len, uint8_t cols = 16) {
	const uint8_t* src = (const uint8_t*) mem;
	Serial.printf("\n[HEXDUMP] Address: 0x%08X len: 0x%X (%d)", (ptrdiff_t)src, len, len);
	for(uint32_t i = 0; i < len; i++) {
		if(i % cols == 0) {
			Serial.printf("\n[0x%08X] 0x%08X: ", (ptrdiff_t)src, i);
		}
		Serial.printf("%02X ", *src);
		src++;
	}
	Serial.printf("\n");
}

std::string fnPrintArray(uint8_t * input) {
  std::string str;
  return str = reinterpret_cast<char *>(input);
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
    std::string msg; 
    msg = fnPrintArray(payload);
    switch(type) {
        case WStype_DISCONNECTED:
            DEBUG_SOCKET("[%u] Disconnected!\n", millis());
            break;
        case WStype_CONNECTED:
            {
              DEBUG_SOCKET("[WSc] Connected to url: %s\n",  payload);
              // send message to server when Connected
              webSocket.sendTXT("Connected");
            }
            break;
        case WStype_TEXT: 
            DEBUG_SOCKET("[WSc] get text: %s\n", msg);
            if (msg == UPDATE) { webSocket.sendTXT("updating"); }
            if (msg == REBOOT) { webSocket.sendTXT("rebooting"); ESP.restart(); }
            if (msg == PROVISION) { webSocket.sendTXT("provising"); }
            if (msg == RUNTIME) { webSocket.sendTXT(millis());  };
            break;
        case WStype_BIN:
            DEBUG_SOCKET("[WSc] get binary length: %u\n", length);
            hexdump(payload, length);
            // webSocket.sendBIN(payload, length);
            break;
		case WStype_ERROR:			
		case WStype_FRAGMENT_TEXT_START:
		case WStype_FRAGMENT_BIN_START:
		case WStype_FRAGMENT:
		case WStype_FRAGMENT_FIN:
			break;
    }

}

void setup() {
    // Serial.begin(921600);
    Serial.begin(115200);

    //Serial.setDebugOutput(true);
    Serial.setDebugOutput(true);

    Serial.println();
    Serial.println();
    Serial.println();

      for(uint8_t t = 4; t > 0; t--) {
          Serial.printf("[SETUP] BOOT WAIT %d...\n", t);
          Serial.flush();
          delay(1000);
      }

    wiFiMulti.addAP("SSID", "PASSWORD");

    //WiFi.disconnect();
    while(wiFiMulti.run() != WL_CONNECTED) {
        delay(100);
    }

    webSocket.beginSSL("192.168.0.89", 81);
    webSocket.onEvent(webSocketEvent);
    webSocket.sendTXT("What do I do now?");
}

void loop() {
    webSocket.loop();
}
