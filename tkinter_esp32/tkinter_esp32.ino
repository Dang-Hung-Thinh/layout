#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "P813";
const char* password = "dddddddd";
const char* api_token = "123456"; // Token bảo mật

WebServer server(80);

// LED pins
const int ledPins[] = {2, 4, 5}; // GPIO2, GPIO4, GPIO5
const int numLeds = sizeof(ledPins) / sizeof(ledPins[0]);

void handleLedControl(bool state) {
  String token = server.arg("token");
  String ledIndexStr = server.arg("led");
  if (token != api_token) {
    server.send(403, "text/plain", "Forbidden: Invalid token");
    return;
  }

  int ledIndex = ledIndexStr.toInt();
  if (ledIndex < 0 || ledIndex >= numLeds) {
    server.send(400, "text/plain", "Bad Request: Invalid LED index");
    return;
  }

  digitalWrite(ledPins[ledIndex], state ? HIGH : LOW);
  String message = "LED" + String(ledIndex) + (state ? " ON" : " OFF");
  server.send(200, "text/plain", message);
}

void handleStatus() {
  String token = server.arg("token");
  if (token != api_token) {
    server.send(403, "text/plain", "Forbidden: Invalid token");
    return;
  }

  String status = "";
  for (int i = 0; i < numLeds; i++) {
    status += "LED" + String(i) + ":" + (digitalRead(ledPins[i]) ? "ON" : "OFF") + ";";
  }
  server.send(200, "text/plain", status);
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
  Serial.println(WiFi.localIP());

  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }

  server.on("/led_on", HTTP_GET, []() { handleLedControl(true); });
  server.on("/led_off", HTTP_GET, []() { handleLedControl(false); });
  server.on("/status", HTTP_GET, handleStatus);

  server.begin();
}

void loop() {
  server.handleClient();
}
