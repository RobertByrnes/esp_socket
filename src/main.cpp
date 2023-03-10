#include <Arduino.h>
#include <WiFi.h>
#include <FS.h>
#include <ESPSocket.h>
#include <TCallBattery.h>

#define AP_SSID "CrowdedHouse"
#define AP_PASS "kF4QMhzc3xcS"
#define LED_PIN 13
#define DELAY_TIME 5000

TCallBattery battery;
AsyncWebServer server(80);


void setup()
{
  Serial.begin(115200);
  Serial.printf("");
  WiFi.mode(WIFI_STA);

  WiFi.begin(AP_SSID, AP_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  
  Serial.println(WiFi.localIP());

  ESPSocket.begin(&server, DEFAULT_CALLBACK);
  
  server.begin();
  
  Serial.println("ESP Socket OK");
}

void loop()
{
  ESPSocket.printBatteryInfo(battery);
  ESPSocket.printWiFiInfo();
  ESPSocket.printESPInfo();
  delay(DELAY_TIME);
}
