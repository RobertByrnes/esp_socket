// #define DEBUG_ESP_SOCKET

#include <Arduino.h>
#include <WiFi.h>
#include <FS.h>
#include <ESPSocket.h>
#include <TCallBattery.h>

#define AP_SSID "ssid"
#define AP_PASS "password"
#define LED_PIN 13
#define DELAY_TIME 5000

TCallBattery battery;
AsyncWebServer server(80);

std::string ESP_REBOOT = "reboot";
std::string ESP_UPDATE = "update";
std::string ESP_PROVISION = "provision";

bool restart = false;

void update() {
  // implement wifi update function
}

void provision() {
  // implement provisioning function
}

void onMsgHandler(uint8_t *data, size_t len) {
  ESPSocket.print("[");
  ESPSocket.print((int)millis());
  ESPSocket.print("] ");

  std::string action = "";
  for (int i = 0; i < len; i++) {
    action += char(data[i]);
  }

  // ESPSocket.println(action.c_str());

  if (action == ESP_REBOOT) {
    ESPSocket.println("Rebooting in 5 seconds, this page will reload...");
    ESPSocket.println("[:RELOAD:]");
    restart = true;
    return;
  }

  if (action == ESP_UPDATE) {
    ESPSocket.println("Processing update signal...");
    update();
    return;
  }

  if (action == ESP_PROVISION) {
    ESPSocket.println("Provisioning...");
    provision();
    return;
  }

  ESPSocket.println("Invalid message...");
}

void setup() {
  Serial.begin(115200);
  Serial.printf("");
  WiFi.mode(WIFI_STA);

  WiFi.begin(AP_SSID, AP_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println(WiFi.localIP());

  ESPSocket.msgCallback(onMsgHandler);
  ESPSocket.begin(&server);

  server.begin();

  Serial.println("ESP Socket OK");
}

void loop() {
  ESPSocket.printBatteryInfo(battery);
  ESPSocket.printWiFiInfo();
  ESPSocket.printESPInfo();
  ESPSocket.printf("Runtime: %u", millis());
  delay(DELAY_TIME);

  if (restart == true) {
    ESP.restart();
  }
}
