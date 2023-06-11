#if not defined(MOCK_ESP8266_WIFI_H)
#define MOCK_ESP8266_WIFI_H

#include <Arduino.h>
#include <Emulator.h>

class ESP8266WiFI : public Emulator {
public:
	ESP8266WiFI() {}
	~ESP8266WiFI() {}
	static int RSSI() {
		return 0;
	}
	static String getHostname() {
		return "test";
	}
	static IPAddress gatewayIP() {
		return IPAddress(0, 0, 0, 0);
	}

protected:

private:

};

extern ESP8266WiFI WiFi;

#endif
