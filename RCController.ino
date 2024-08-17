#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>

#include <functional>

ESP8266WebServer server(80);

int forwardPin = D8;
int backwardPin = D7;
int leftPin = D2;
int rightPin = D1;
int debugPin = LED_BUILTIN;

int len = 4;
int OnStatus = LOW;
int OffStatus = HIGH;
int locatedPins[] = { forwardPin, backwardPin, leftPin, rightPin };
bool pinStatus[] = { false, false, false, false };
String charsPins[] = { "W", "S", "A", "D" };

void setup() {
  Serial.begin(115200);

  // useWifi();
  useWifiHotspot();

  for (byte i = 0; i < len; i = i + 1) {
    pinMode(locatedPins[i], OUTPUT);
    digitalWrite(locatedPins[i], OffStatus);
  }

  server.on("/", handle_OnConnect);
  server.on("/state", serverHandler);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  for (byte i = 0; i < len; i = i + 1) {
    digitalWrite(locatedPins[i], pinStatus[i] ? OnStatus : OffStatus);
  }
  delay(10);
}