#include "ESPSocket.h"
#include "esp32_socket_webpage.h"
#include "favicon.h"

/* Default message callback of ESPSocket */
void recvMsg(uint8_t *data, size_t len)
{
    ESPSocket.println("Received Data...");
    String d = "";
    for (int i = 0; i < len; i++)
    {
        d += char(data[i]);
    }
    ESPSocket.println(d);
}

void ESPSocketClass::begin(AsyncWebServer *server, int defaultCallback, const char *url)
{
    if (defaultCallback == 1)
    {
        this->msgCallback(recvMsg);
    }

    _url = url;
    _server = server;
    _ws = new AsyncWebSocket("/ws");

    _server->on(_url, HTTP_GET, [](AsyncWebServerRequest *request)
                { request->send_P(200, "text/html", data_index_html); });

    _server->on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request)
                { request->send_P(200, "text/x-icon", favicon); });

    _server->on("/cdn-cgi/rum?", HTTP_POST, [](AsyncWebServerRequest *request)
                { request->send(200); });

    _ws->onEvent([&](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) -> void
                 {
        switch (type) {
            case WS_EVT_CONNECT:
#if defined(DEBUG_ESP_SOCKET)
                    Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
#endif
                break;
            case WS_EVT_DISCONNECT:
#if defined(DEBUG_ESP_SOCKET)
                    Serial.printf("WebSocket client #%u disconnected\n", client->id());
#endif
                break;
            case WS_EVT_DATA:
#if defined(DEBUG_ESP_SOCKET)
                    debugSocket("Received Websocket Data");
#endif
                if (_RecvFunc != NULL) {
                    _RecvFunc(data, len);
                }
                break;
            case WS_EVT_PONG:
            case WS_EVT_ERROR:
            default:
#if defined(DEBUG_ESP_SOCKET)
                    debugSocket("Websocket Error");
#endif
        }
        _ws->cleanupClients(); });

    _server->addHandler(_ws);

#if defined(DEBUG_ESP_SOCKET)
    debugSocket("Attached AsyncWebServer along with Websockets");
#endif
}

void ESPSocketClass::msgCallback(RecvMsgHandler _recv)
{
    _RecvFunc = _recv;
}

void ESPSocketClass::print(String m)
{
    _ws->textAll(m);
}

void ESPSocketClass::print(const char *m)
{
    _ws->textAll(m);
}

void ESPSocketClass::print(char *m)
{
    _ws->textAll(m);
}

void ESPSocketClass::print(int m)
{
    _ws->textAll(String(m));
}

void ESPSocketClass::print(uint8_t m)
{
    _ws->textAll(String(m));
}

void ESPSocketClass::print(uint16_t m)
{
    _ws->textAll(String(m));
}

void ESPSocketClass::print(uint32_t m)
{
    _ws->textAll(String(m));
}

void ESPSocketClass::print(double m)
{
    _ws->textAll(String(m));
}

void ESPSocketClass::print(float m)
{
    _ws->textAll(String(m));
}

void ESPSocketClass::println(String m)
{
    _ws->textAll(m + "\n");
}

void ESPSocketClass::println(const char *m)
{
    _ws->textAll(String(m) + "\n");
}

void ESPSocketClass::println(char *m)
{
    _ws->textAll(String(m) + "\n");
}

void ESPSocketClass::println(int m)
{
    _ws->textAll(String(m) + "\n");
}

void ESPSocketClass::println(uint8_t m)
{
    _ws->textAll(String(m) + "\n");
}

void ESPSocketClass::println(uint16_t m)
{
    _ws->textAll(String(m) + "\n");
}

void ESPSocketClass::println(uint32_t m)
{
    _ws->textAll(String(m) + "\n");
}

void ESPSocketClass::println(float m)
{
    _ws->textAll(String(m) + "\n");
}

void ESPSocketClass::println(double m)
{
    _ws->textAll(String(m) + "\n");
}

size_t ESPSocketClass::printf(const char *format, ...)
{
    char loc_buf[64];
    char *temp = loc_buf;
    va_list arg;
    va_list copy;
    va_start(arg, format);
    va_copy(copy, arg);
    int len = vsnprintf(temp, sizeof(loc_buf), format, copy);
    va_end(copy);
    if (len < 0)
    {
        va_end(arg);
        return 0;
    };
    if (len >= sizeof(loc_buf))
    {
        temp = (char *)malloc(len + 1);
        if (temp == NULL)
        {
            va_end(arg);
            return 0;
        }
        len = vsnprintf(temp, len + 1, format, arg);
    }
    va_end(arg);
    _ws->textAll(String(temp) + "\n");
    // len = write((uint8_t*)temp, len);
    if (temp != loc_buf)
    {
        free(temp);
    }
    return len;
}

void ESPSocketClass::printBatteryInfo(TCallBattery &battery)
{
    this->print(
        "[:BATT:] Charge: " +
        String(battery.getPercentage()) +
        "%, Voltage: " +
        String(battery.getSampledVoltage()) +
        "VDc\n");
}

void ESPSocketClass::printWiFiInfo()
{
    this->print(
        "[:WIFI:]RSSI: " +
        String(WiFi.RSSI()) +
        "dBm, Host: " +
        WiFi.getHostname() +
        ", Gateway: " +
        String(WiFi.gatewayIP()) +
        "\n");
}

void ESPSocketClass::printESPInfo()
{
    this->print(
        "[:ESP:]Model: " +
        String(ESPSocket._chipModel) +
        ", Rev: " +
        ESPSocket._chipRevision +
        ", Freq: " +
        ESPSocket._cpuFreq +
        "MHz\n");
}

#ifdef DEBUG_ESP_SOCKET
void ESPSocketClass::debugSocket(const char *message)
{
    Serial.printf("[ESP Socket] %s\n", message);
}
#endif

ESPSocketClass ESPSocket;
