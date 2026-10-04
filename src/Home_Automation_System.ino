/*
 * ESP32 Home Automation System
 *
 * Features:
 * - Web-based manual appliance control
 * - Automatic light control using LDR + PIR
 * - Automatic fan control using DHT11 temperature
 * - Temperature and humidity monitoring
 *
 * Board: ESP32 Dev Module
 */

#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>

// -------------------- Wi-Fi --------------------
const char* AP_SSID = "HomeAutomation";
const char* AP_PASSWORD = "12345678";

WebServer server(80);

// -------------------- Pins --------------------
#define RELAY_LIGHT 26
#define RELAY_FAN   27
#define PIR_PIN     14
#define LDR_PIN     34
#define DHT_PIN     4
#define DHT_TYPE    DHT11

DHT dht(DHT_PIN, DHT_TYPE);

// Many relay modules are active LOW.
// Change to HIGH if your relay is active HIGH.
#define RELAY_ON  LOW
#define RELAY_OFF HIGH

// -------------------- Settings --------------------
const int DARK_THRESHOLD = 1800;
const float FAN_TEMP_THRESHOLD = 30.0;

bool automaticMode = true;
bool lightState = false;
bool fanState = false;

float temperature = 0.0;
float humidity = 0.0;
int lightLevel = 0;
bool motionDetected = false;

unsigned long lastSensorRead = 0;
const unsigned long SENSOR_INTERVAL = 2000;

// -------------------- Helper Functions --------------------

void setLight(bool state) {
  lightState = state;
  digitalWrite(RELAY_LIGHT, state ? RELAY_ON : RELAY_OFF);
}

void setFan(bool state) {
  fanState = state;
  digitalWrite(RELAY_FAN, state ? RELAY_ON : RELAY_OFF);
}

String jsonBool(bool value) {
  return value ? "true" : "false";
}

String htmlPage() {
  String page = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Home Automation</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      background: #f2f2f2;
      margin: 0;
      padding: 20px;
      text-align: center;
    }
    h1 { margin-bottom: 5px; }
    .container {
      max-width: 800px;
      margin: auto;
    }
    .card {
      background: white;
      border-radius: 12px;
      padding: 20px;
      margin: 15px 0;
      box-shadow: 0 2px 8px rgba(0,0,0,.12);
    }
    .value {
      font-size: 28px;
      font-weight: bold;
    }
    button {
      border: 0;
      padding: 12px 22px;
      margin: 6px;
      border-radius: 8px;
      cursor: pointer;
      font-size: 16px;
    }
    .on { background: #28a745; color: white; }
    .off { background: #dc3545; color: white; }
    .mode { background: #007bff; color: white; }
  </style>
</head>
<body>
<div class="container">
  <h1>🏠 Home Automation</h1>
  <p>ESP32 Embedded System</p>

  <div class="card">
    <h2>Environment</h2>
    <p>Temperature: <span id="temp" class="value">--</span> °C</p>
    <p>Humidity: <span id="humidity" class="value">--</span> %</p>
    <p>Light Level: <span id="lightLevel" class="value">--</span></p>
    <p>Motion: <span id="motion" class="value">--</span></p>
  </div>

  <div class="card">
    <h2>Mode</h2>
    <p>Current mode: <span id="mode" class="value">--</span></p>
    <button class="mode" onclick="toggleMode()">Toggle Automatic/Manual</button>
  </div>

  <div class="card">
    <h2>Appliances</h2>
    <p>Light: <span id="lightState" class="value">--</span></p>
    <button class="on" onclick="control('light','on')">Light ON</button>
    <button class="off" onclick="control('light','off')">Light OFF</button>

    <p>Fan: <span id="fanState" class="value">--</span></p>
    <button class="on" onclick="control('fan','on')">Fan ON</button>
    <button class="off" onclick="control('fan','off')">Fan OFF</button>
  </div>
</div>

<script>
async function updateStatus() {
  const response = await fetch('/status');
  const data = await response.json();

  document.getElementById('temp').innerText =
    data.temperature.toFixed(1);
  document.getElementById('humidity').innerText =
    data.humidity.toFixed(1);
  document.getElementById('lightLevel').innerText =
    data.lightLevel;
  document.getElementById('motion').innerText =
    data.motion ? "Detected" : "Not Detected";

  document.getElementById('mode').innerText =
    data.automatic ? "Automatic" : "Manual";

  document.getElementById('lightState').innerText =
    data.light ? "ON" : "OFF";
  document.getElementById('fanState').innerText =
    data.fan ? "ON" : "OFF";
}

async function control(device, action) {
  if (device === 'light') {
    await fetch('/light/' + action);
  } else {
    await fetch('/fan/' + action);
  }
  updateStatus();
}

async function toggleMode() {
  await fetch('/mode/toggle');
  updateStatus();
}

updateStatus();
setInterval(updateStatus, 2000);
</script>
</body>
</html>
)rawliteral";

  return page;
}

// -------------------- Sensor Reading --------------------

void readSensors() {
  lightLevel = analogRead(LDR_PIN);
  motionDetected = digitalRead(PIR_PIN);

  float newTemperature = dht.readTemperature();
  float newHumidity = dht.readHumidity();

  if (!isnan(newTemperature)) {
    temperature = newTemperature;
  }

  if (!isnan(newHumidity)) {
    humidity = newHumidity;
  }
}

// -------------------- Automatic Control --------------------

void automaticControl() {
  // Light:
  // Turn ON when it is dark and motion is detected.
  bool dark = lightLevel < DARK_THRESHOLD;

  if (dark && motionDetected) {
    setLight(true);
  } else if (!dark) {
    setLight(false);
  }

  // Fan:
  // Turn ON above the configured temperature.
  if (temperature >= FAN_TEMP_THRESHOLD) {
    setFan(true);
  } else {
    setFan(false);
  }
}

// -------------------- Web Handlers --------------------

void handleRoot() {
  server.send(200, "text/html", htmlPage());
}

void handleStatus() {
  String json = "{";
  json += "\"temperature\":" + String(temperature, 1) + ",";
  json += "\"humidity\":" + String(humidity, 1) + ",";
  json += "\"lightLevel\":" + String(lightLevel) + ",";
  json += "\"motion\":" + jsonBool(motionDetected) + ",";
  json += "\"automatic\":" + jsonBool(automaticMode) + ",";
  json += "\"light\":" + jsonBool(lightState) + ",";
  json += "\"fan\":" + jsonBool(fanState);
  json += "}";

  server.send(200, "application/json", json);
}

void handleLightOn() {
  if (!automaticMode) {
    setLight(true);
  }
  server.send(200, "text/plain", "OK");
}

void handleLightOff() {
  if (!automaticMode) {
    setLight(false);
  }
  server.send(200, "text/plain", "OK");
}

void handleFanOn() {
  if (!automaticMode) {
    setFan(true);
  }
  server.send(200, "text/plain", "OK");
}

void handleFanOff() {
  if (!automaticMode) {
    setFan(false);
  }
  server.send(200, "text/plain", "OK");
}

void handleModeToggle() {
  automaticMode = !automaticMode;

  // When entering automatic mode, immediately apply automatic logic.
  if (automaticMode) {
    automaticControl();
  }

  server.send(200, "text/plain", automaticMode ? "Automatic" : "Manual");
}

// -------------------- Setup --------------------

void setup() {
  Serial.begin(115200);

  pinMode(RELAY_LIGHT, OUTPUT);
  pinMode(RELAY_FAN, OUTPUT);
  pinMode(PIR_PIN, INPUT);
  pinMode(LDR_PIN, INPUT);

  setLight(false);
  setFan(false);

  dht.begin();

  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID, AP_PASSWORD);

  Serial.println();
  Serial.println("================================");
  Serial.println("ESP32 Home Automation System");
  Serial.println("================================");
  Serial.print("Wi-Fi SSID: ");
  Serial.println(AP_SSID);
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", HTTP_GET, handleRoot);
  server.on("/status", HTTP_GET, handleStatus);

  server.on("/light/on", HTTP_GET, handleLightOn);
  server.on("/light/off", HTTP_GET, handleLightOff);
  server.on("/fan/on", HTTP_GET, handleFanOn);
  server.on("/fan/off", HTTP_GET, handleFanOff);

  server.on("/mode/toggle", HTTP_GET, handleModeToggle);

  server.begin();
  Serial.println("Web server started.");
}

// -------------------- Main Loop --------------------

void loop() {
  server.handleClient();

  if (millis() - lastSensorRead >= SENSOR_INTERVAL) {
    lastSensorRead = millis();

    readSensors();

    if (automaticMode) {
      automaticControl();
    }

    Serial.print("Temp: ");
    Serial.print(temperature);
    Serial.print(" C | Humidity: ");
    Serial.print(humidity);
    Serial.print(" % | LDR: ");
    Serial.print(lightLevel);
    Serial.print(" | Motion: ");
    Serial.println(motionDetected ? "YES" : "NO");
  }
}
