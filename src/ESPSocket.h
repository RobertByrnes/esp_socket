#ifndef ESP_SOCKET_H
#define ESP_SOCKET_H

#include "Arduino.h"
#include "stdlib_noniso.h"
#include <functional>
#include <TCallBattery.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#if defined(ESP8266)
#define HARDWARE "ESP8266"
#include "ESP8266WiFi.h"
#include "ESPAsyncTCP.h"
#include "ESPAsyncWebServer.h"
#elif defined(ESP32)
#define HARDWARE "ESP32"
#include "WiFi.h"
#include "AsyncTCP.h"
#include "ESPAsyncWebServer.h"
#endif

/**
 * @brief Message handler function typedef for socket events.
 * To be passed into ESPSocketClass using msgCallback
 * method and before calling the begin method.
 *
 * @typedef RecvMsgHandler
 */
typedef std::function<void(uint8_t *data, size_t len)> RecvMsgHandler;

class ESPSocketClass
{
public:
    #ifdef ESP32
    const char *_chipModel = ESP.getChipModel();
    uint32_t _chipRevision = ESP.getChipRevision();
    void printBatteryInfo(TCallBattery &battery);
    #endif
    uint32_t _cpuFreq = ESP.getCpuFreqMHz();

    void begin(AsyncWebServer *server, int defaultCallback = 0, const char *url = "/esp");
    void msgCallback(RecvMsgHandler _recv);
    void printWiFiInfo(bool verbose = false);
    void printESPInfo(bool verbose = false);

    // Print
    void print(String m = "");
    void print(const char *m);
    void print(char *m);
    void print(int m);
    void print(uint8_t m);
    void print(uint16_t m);
    void print(uint32_t m);
    void print(double m);
    void print(float m);
    // Print with New Line
    void println(String m = "");
    void println(const char *m);
    void println(char *m);
    void println(int m);
    void println(uint8_t m);
    void println(uint16_t m);
    void println(uint32_t m);
    void println(float m);
    void println(double m);
    // Print formatted string with arguments
    size_t printf(const char *format, ...);

private:
    const char *_url;
    AsyncWebServer *_server;
    AsyncWebSocket *_ws;
    RecvMsgHandler _RecvFunc = NULL;
};

extern ESPSocketClass ESPSocket;

#endif