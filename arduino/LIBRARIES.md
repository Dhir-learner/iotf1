# Libraries Required for Arduino/ESP32

## Install these libraries in Arduino IDE:

### Method 1: Arduino Library Manager
1. Open Arduino IDE
2. Go to Sketch → Include Library → Manage Libraries
3. Search and install each library:

### Required Libraries:

1. **Adafruit Fingerprint Sensor Library**
   - Author: Adafruit
   - Version: Latest stable
   - Search: "Adafruit Fingerprint"

2. **ArduinoJson**
   - Author: Benoit Blanchon
   - Version: 6.x (latest stable)
   - Search: "ArduinoJson"

3. **WiFi Library (ESP32)**
   - Usually pre-installed with ESP32 board package
   - If missing, install ESP32 board package via Board Manager

### Method 2: Manual Installation
Download and install manually if needed:
- Adafruit_Fingerprint: https://github.com/adafruit/Adafruit-Fingerprint-Sensor-Library
- ArduinoJson: https://github.com/bblanchon/ArduinoJson

### ESP32 Board Package:
1. File → Preferences
2. Additional Board Manager URLs: `https://dl.espressif.com/dl/package_esp32_index.json`
3. Tools → Board → Boards Manager
4. Search "ESP32" and install by Espressif Systems

## Compilation Notes:
- Select correct board: ESP32 Dev Module
- Set correct port
- Ensure all libraries are compatible versions
- Use Arduino IDE 1.8.x or 2.x