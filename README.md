# 🏠 ESP32 Home Automation System

A simple and practical **Embedded Systems project** for controlling home appliances using an **ESP32 microcontroller**. The system supports manual control through a web interface and automatic control using sensors.

## 📌 Project Overview

The Home Automation System is designed to control appliances such as lights and fans while monitoring the home environment.

The ESP32 acts as the main controller. It receives input from sensors and controls appliances through relay modules.

### Main Features

- 💡 Manual ON/OFF control of appliances
- 🌙 Automatic light control using an LDR
- 🚶 Motion detection using a PIR sensor
- 🌡️ Temperature and humidity monitoring using a DHT11
- 🌐 Web-based control interface hosted directly by ESP32
- 🔌 Relay-based appliance control
- 🔄 Automatic and manual operating modes
- 🖥️ Sensor values displayed on the web page

## 🛠️ Hardware Required

| Component | Quantity | Purpose |
|---|---:|---|
| ESP32 Development Board | 1 | Main controller |
| 2-Channel Relay Module | 1 | Controls appliances |
| PIR Motion Sensor | 1 | Detects human motion |
| LDR | 1 | Detects light intensity |
| 10kΩ Resistor | 1 | LDR voltage divider |
| DHT11 Sensor | 1 | Temperature & humidity |
| LED | 1–2 | Safe demonstration load |
| Resistor 220Ω | 1–2 | LED current limiting |
| Jumper Wires | As required | Connections |
| Breadboard | 1 | Prototyping |

> **Safety:** For a student demonstration, use LEDs or other low-voltage loads. Do not connect mains AC appliances directly to a breadboard. If mains switching is required, use an appropriately rated enclosed relay/contactor and qualified supervision.

## 🔌 Pin Configuration

| Component | ESP32 Pin |
|---|---|
| Relay 1 – Light | GPIO 26 |
| Relay 2 – Fan | GPIO 27 |
| PIR OUT | GPIO 14 |
| LDR Analog Output | GPIO 34 |
| DHT11 DATA | GPIO 4 |

### LDR Connection

Use the LDR and 10kΩ resistor as a voltage divider:

```text
3.3V
 |
[LDR]
 |
 +-------- GPIO 34
 |
[10kΩ]
 |
GND
```

### Basic Block Diagram

```text
             ┌─────────────────┐
             │      ESP32      │
             │  Main Controller│
             └───────┬─────────┘
                     │
       ┌─────────────┼─────────────┐
       │             │             │
       ▼             ▼             ▼
    ┌─────┐       ┌─────┐       ┌─────┐
    │ PIR │       │ LDR │      │DHT11 │
    └─────┘       └─────┘       └─────┘
       │             │             │
       └─────────────┼─────────────┘
                     │
                     ▼
              ┌────────────┐
              │ Control    │
              │   Logic    │
              └─────┬──────┘
                    │
             ┌──────┴──────┐
             ▼             ▼
        ┌─────────┐   ┌─────────┐
        │ Relay 1 │   │ Relay 2 │
        │  Light  │   │   Fan   │
        └─────────┘   └─────────┘
```

## ⚙️ How It Works

1. ESP32 starts and creates a Wi-Fi access point.
2. A phone/laptop connects to the ESP32 Wi-Fi network.
3. Open `192.168.4.1` in a browser.
4. The web interface displays temperature, humidity, motion and light readings.
5. The user can manually switch the Light and Fan ON/OFF.
6. In **Automatic Mode**:
   - The light is turned ON when the environment becomes dark.
   - The light can also respond to motion.
   - The fan is turned ON when temperature reaches the configured threshold.
7. The ESP32 continuously reads sensors and updates the appliance states.

## 💻 Software Requirements

- Arduino IDE
- ESP32 board package
- `DHT sensor library` by Adafruit
- `Adafruit Unified Sensor` library

### ESP32 Board Installation

In Arduino IDE:

**File → Preferences → Additional Boards Manager URLs**

Add the ESP32 board package URL:

```text
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
```

Then:

**Tools → Board → Boards Manager → Search "esp32" → Install**

## 🚀 How to Run

### 1. Clone the repository

```bash
git clone https://github.com/YOUR_USERNAME/Home-Automation-System.git
cd Home-Automation-System
```

### 2. Open the code

Open:

```text
src/Home_Automation_System.ino
```

in Arduino IDE.

### 3. Install libraries

Install:

- DHT sensor library
- Adafruit Unified Sensor

### 4. Select board

Choose:

```text
ESP32 Dev Module
```

### 5. Upload

Connect the ESP32 through USB and upload the program.

### 6. Connect to the ESP32

After startup, the ESP32 creates:

```text
Wi-Fi Name: HomeAutomation
Password: 12345678
```

Connect your phone/laptop to this network.

Open:

```text
http://192.168.4.1
```

## 🌐 Web Interface

The interface provides:

- Current temperature
- Humidity
- Light intensity
- Motion status
- Light control
- Fan control
- Automatic/manual mode selection

## 📂 Project Structure

```text
Home-Automation-System/
│
├── README.md
├── LICENSE
├── .gitignore
│
├── src/
│   └── Home_Automation_System.ino
│
└── docs/
    └── circuit_notes.md
```

## 🔮 Future Improvements

- Mobile application using Blynk/Flutter
- MQTT-based IoT control
- Google Assistant/Alexa integration
- Energy consumption monitoring
- Real-time cloud dashboard
- Password-protected web interface
- Scheduling appliances
- More rooms and appliances
- Current/voltage sensors for energy monitoring

## 🎯 Learning Outcomes

This project demonstrates:

- Microcontroller programming
- Digital and analog sensor interfacing
- Relay control
- Embedded C/C++ programming
- Wi-Fi communication
- HTTP web server concepts
- Automation logic
- Hardware-software integration

## 👨‍💻 Author

**Sukrity Roy**

Embedded Systems / IoT Project

