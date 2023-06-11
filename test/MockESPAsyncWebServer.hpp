#if not defined(MOCK_ESP_ASYNC_WEB_SERVER_H)
#define MOCK_ESP_ASYNC_WEB_SERVER_H

#include <Arduino.h>
#include <Emulator.h>
#include <functional>

#define HTTP_GET			 	1
#define HTTP_POST 				2
#define HTTP_PUT 				3	
#define HTTP_DELETE 			4
#define HTTP_OPTIONS 			5
#define HTTP_HEAD 				6
#define HTTP_PATCH 				7
#define WS_EVT_CONNECT 			8
#define WS_EVT_DISCONNECT 		9
#define WS_EVT_DATA 			10
#define WS_EVT_PONG 			11
#define WS_EVT_ERROR 			12

// using AsyncWebHandler = decltype(std::declval<void(*)(AsyncWebServerRequest*)>());

typedef int AwsEventType;

class AsyncWebSocketClient : public Emulator {
public:
	AsyncWebSocketClient() {}
	~AsyncWebSocketClient() {}
	int text(const char * a) { return this->mock<int>("text"); }

protected:

private:

};

class AsyncWebSocket : public Emulator {
public:
	AsyncWebSocket(const char * ws) {}
	~AsyncWebSocket() {}
	void onEvent(std::function<void(AsyncWebSocket*, AsyncWebSocketClient*, AwsEventType, void*, uint8_t*, size_t)> handler) {}
	int textAll(const char * a) { return this->mock<int>("textAll"); }
	int textAll(String a) { return this->mock<int>("textAll"); }
	int cleanupClients() { return this->mock<int>("cleanupClients"); }

protected:

private:

};

class AsyncWebServerRequest : public Emulator {
public:
	AsyncWebServerRequest() {}
	~AsyncWebServerRequest() {}
	int send_P(int a, const char * b, const char * c) { return this->mock<int>("send_P"); }
	int send(int a) { return this->mock<int>("send"); }


protected:

private:

};

// AsyncWebServerRequest request;
// typedef void (*AsyncWebHandler)(AsyncWebServerRequest *request);

class AsyncWebServer : public Emulator {
public:
	AsyncWebServer() {}
	~AsyncWebServer() {}
	int on(const char * a, int b, void (*c)(AsyncWebServerRequest *)) { return this->mock<int>("on"); }
	int on(const char * a, int b, void (*c)(AsyncWebServerRequest *, const char *, size_t, size_t)) { return this->mock<int>("on"); }
	int on(const char * a, int b, void (*c)(AsyncWebServerRequest *, uint8_t *, size_t, size_t)) { return this->mock<int>("on"); }
	int addHandler(AsyncWebSocket * a) { return this->mock<int>("addHandler"); }


protected:

private:

};

#endif
