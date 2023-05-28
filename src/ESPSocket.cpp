#include "ESPSocket.h"
#include "esp32_socket_webpage.h"
#include "favicon.h"

#if defined(ESP_SOCKET_DEBUG)
#define SOCKET_DEBUG(format, ...)  Serial.printf(const char *format, ...)
#else
#define SOCKET_DEBUG(format, ...)
#endif

/**
 * @brief Default message callback of ESPSocket.
 *
 * @param data uint8_t *
 * @param len size_t
 */
void recvMsg(uint8_t *data, size_t len) {
    ESPSocket.println("Received Data...");
    String d = "";
    for (int i = 0; i < len; i++) {
        d += char(data[i]);
    }
    ESPSocket.println(d);
}

/**
 * @brief Create a new instance of AsyncWebSocket. The default behaviour
 * is that no callback function is set for the onEvent listener. In
 * order to use the default (recvMsg()) pass in 1 as the second
 * argument. The url is used /url after the ip address to
 * make a connection to an iot device.
 *
 * @details AsyncWebServer starts listening for HTTP protocol events.
 * @details AsyncWebSocket starts listening for web socket events.
 *
 * @param server AsyncWebServer *
 * @param defaultCallback int
 * @param url const char *
 */
void ESPSocketClass::begin(AsyncWebServer *server, int defaultCallback, const char *url) {
    if (defaultCallback == 1) {
        this->msgCallback(recvMsg);
    }

    _url = url;
    _server = server;
    _ws = new AsyncWebSocket("/ws");

    _server->on(_url, HTTP_GET, [](AsyncWebServerRequest *request) { 
        request->send_P(200, "text/html", data_index_html); 
    });

    _server->on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send_P(200, "text/x-icon", favicon); 
    });

    _server->on("/cdn-cgi/rum?", HTTP_POST, [](AsyncWebServerRequest *request) { request->send(200); });

    _ws->onEvent([&](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) -> void {
        switch (type) {
            case WS_EVT_CONNECT:
                SOCKET_DEBUG("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
                break;
            case WS_EVT_DISCONNECT:
                SOCKET_DEBUG("WebSocket client #%u disconnected\n", client->id());
                break;
            case WS_EVT_DATA:
                SOCKET_DEBUG("Received Websocket Data");
                if (_RecvFunc != NULL) {
                    _RecvFunc(data, len);
                }
                break;
            case WS_EVT_PONG:
            case WS_EVT_ERROR:
            default:
                SOCKET_DEBUG("Websocket Error");
        }
        _ws->cleanupClients(); 
    });

    _server->addHandler(_ws);
    SOCKET_DEBUG("Attached AsyncWebServer along with Websockets");
}

/**
 * @brief Set the message handler callback function.
 *
 * @param _recv RecvMsgHandler
 */
void ESPSocketClass::msgCallback(RecvMsgHandler _recv) {
    _RecvFunc = _recv;
}

void ESPSocketClass::print(String m) {
    _ws->textAll(m);
}

void ESPSocketClass::print(const char *m) {
    _ws->textAll(m);
}

void ESPSocketClass::print(char *m) {
    _ws->textAll(m);
}

void ESPSocketClass::print(int m) {
    _ws->textAll(String(m));
}

void ESPSocketClass::print(uint8_t m) {
    _ws->textAll(String(m));
}

void ESPSocketClass::print(uint16_t m) {
    _ws->textAll(String(m));
}

void ESPSocketClass::print(uint32_t m) {
    _ws->textAll(String(m));
}

void ESPSocketClass::print(double m) {
    _ws->textAll(String(m));
}

void ESPSocketClass::print(float m) {
    _ws->textAll(String(m));
}

void ESPSocketClass::println(String m) {
    _ws->textAll(m + "\n");
}

void ESPSocketClass::println(const char *m) {
    _ws->textAll(String(m) + "\n");
}

void ESPSocketClass::println(char *m) {
    _ws->textAll(String(m) + "\n");
}

void ESPSocketClass::println(int m) {
    _ws->textAll(String(m) + "\n");
}

void ESPSocketClass::println(uint8_t m) {
    _ws->textAll(String(m) + "\n");
}

void ESPSocketClass::println(uint16_t m) {
    _ws->textAll(String(m) + "\n");
}

void ESPSocketClass::println(uint32_t m) {
    _ws->textAll(String(m) + "\n");
}

void ESPSocketClass::println(float m) {
    _ws->textAll(String(m) + "\n");
}

void ESPSocketClass::println(double m) {
    _ws->textAll(String(m) + "\n");
}

/**
 * @brief Print formatted output to the web socket.
 * 
 * @param format 
 * @param ... 
 * @return size_t 
 */
size_t ESPSocketClass::printf(const char *format, ...) {
    char loc_buf[64];
    char *temp = loc_buf;
    va_list arg;
    va_list copy;
    va_start(arg, format);
    va_copy(copy, arg);
    int len = vsnprintf(temp, sizeof(loc_buf), format, copy);
    va_end(copy);
    if (len < 0) {
        va_end(arg);
        return 0;
    }
    if (len >= sizeof(loc_buf)) {
        temp = (char *)malloc(len + 1);
        if (temp == NULL) {
            va_end(arg);
            return 0;
        }
        len = vsnprintf(temp, len + 1, format, arg);
    }
    va_end(arg);
    _ws->textAll(String(temp) + "\n");
    if (temp != loc_buf) {
        free(temp);
    }
    return len;
}

#ifdef ESP32
/**
 * @brief Print battery charge percentage and battery voltage
 * for the TTGO T-Call ESP32 and other supported boards. 
 * 
 * @param battery TCallBattery
 */
void ESPSocketClass::printBatteryInfo(TCallBattery &battery) {
    this->print(
        "[:BATT:] Charge: " +
        String(battery.getPercentage()) +
        "%, Voltage: " +
        String(battery.getSampledVoltage()) +
        "VDc\n");
}
#endif

/**
 * @brief Print WiFi RSSI, host and gateway to the web socket.
 * 
 * @param verbose 
 */
void ESPSocketClass::printWiFiInfo(bool verbose) {
    if (verbose) {
        this->print(
            "[:WIFI:]RSSI: " +
            String(WiFi.RSSI()) +
            "dBm, Host: " +
            WiFi.getHostname() +
            ", Gateway: " +
            String(WiFi.gatewayIP().toString()) +
            "\n");
    } else {
        this->print(
            "[:WIFI:]RSSI: " +
            String(WiFi.RSSI()) +
            "\n");
    }
}

#ifdef ESP32
/**
 * @brief Print chip information for ESP32.
 * 
 * @param verbose 
 */
void ESPSocketClass::printESPInfo(bool verbose) {
    if (verbose) {
        this->print(
            "[:ESP:]Model: " +
            String(ESPSocket._chipModel) +
            ", Rev: " +
            ESPSocket._chipRevision +
            ", Freq: " +
            ESPSocket._cpuFreq +
            "MHz\n");
    } else {
        this->print(
            "[:ESP:]Model: " +
            String(ESPSocket._chipModel) +
            ", " +
            ESPSocket._chipRevision +
            ", " +
            ESPSocket._cpuFreq +
            "MHz\n");
    }
}
#endif

ESPSocketClass ESPSocket;
