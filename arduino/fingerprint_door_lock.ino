#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

// WiFi credentials
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Server endpoint
const char* serverURL = "https://your-server.render.com/api/fingerprint";

// Hardware pins
#define FINGERPRINT_RX 2
#define FINGERPRINT_TX 3
#define DOOR_LOCK_PIN 4
#define LED_GREEN_PIN 5
#define LED_RED_PIN 6
#define BUZZER_PIN 7

// Create fingerprint sensor object
SoftwareSerial mySerial(FINGERPRINT_RX, FINGERPRINT_TX);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

// System variables
bool doorLocked = true;
unsigned long lastScanTime = 0;
const unsigned long SCAN_INTERVAL = 2000; // 2 seconds between scans

void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  // Initialize pins
  pinMode(DOOR_LOCK_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  
  // Initialize door in locked state
  digitalWrite(DOOR_LOCK_PIN, HIGH); // HIGH = locked
  digitalWrite(LED_RED_PIN, HIGH);   // Red LED on = locked
  digitalWrite(LED_GREEN_PIN, LOW);
  
  Serial.println("\n\nFingerprint Door Lock System Starting...");
  
  // Initialize fingerprint sensor
  finger.begin(57600);
  delay(5);
  
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }
  
  // Get sensor info
  finger.getParameters();
  Serial.print("Status: 0x"); Serial.println(finger.status_reg, HEX);
  Serial.print("Sys ID: 0x"); Serial.println(finger.system_id, HEX);
  Serial.print("Capacity: "); Serial.println(finger.capacity);
  Serial.print("Security level: "); Serial.println(finger.security_level);
  Serial.print("Device address: "); Serial.println(finger.device_addr, HEX);
  Serial.print("Packet len: "); Serial.println(finger.packet_len);
  Serial.print("Baud rate: "); Serial.println(finger.baud_rate);
  
  // Connect to WiFi
  connectToWiFi();
  
  Serial.println("System ready! Place finger on sensor...");
  playStartupTone();
}

void loop() {
  // Check WiFi connection
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected. Reconnecting...");
    connectToWiFi();
  }
  
  // Scan for fingerprint every SCAN_INTERVAL
  if (millis() - lastScanTime > SCAN_INTERVAL) {
    lastScanTime = millis();
    scanFingerprint();
  }
  
  delay(100);
}

void connectToWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.println("WiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println();
    Serial.println("WiFi connection failed!");
  }
}

void scanFingerprint() {
  int result = finger.getImage();
  
  if (result != FINGERPRINT_OK) {
    return; // No finger detected
  }
  
  Serial.println("Finger detected! Processing...");
  
  // Convert image to template
  result = finger.image2Tz();
  if (result != FINGERPRINT_OK) {
    Serial.println("Error converting image");
    signalError();
    return;
  }
  
  // Search for match
  result = finger.fingerFastSearch();
  
  if (result == FINGERPRINT_OK) {
    // Authorized fingerprint
    Serial.print("Match found! ID: ");
    Serial.print(finger.fingerID);
    Serial.print(" Confidence: ");
    Serial.println(finger.confidence);
    
    handleAuthorizedAccess();
    sendNotificationToServer("authorized", finger.fingerID, finger.confidence);
    
  } else {
    // Unauthorized fingerprint
    Serial.println("No match found - UNAUTHORIZED ACCESS ATTEMPT!");
    handleUnauthorizedAccess();
    sendNotificationToServer("unauthorized", -1, 0);
  }
}

void handleAuthorizedAccess() {
  Serial.println("ACCESS GRANTED");
  
  // Unlock door
  doorLocked = false;
  digitalWrite(DOOR_LOCK_PIN, LOW);  // LOW = unlocked
  digitalWrite(LED_RED_PIN, LOW);
  digitalWrite(LED_GREEN_PIN, HIGH); // Green LED on
  
  // Success tone
  playSuccessTone();
  
  // Keep door unlocked for 5 seconds
  delay(5000);
  
  // Lock door again
  doorLocked = true;
  digitalWrite(DOOR_LOCK_PIN, HIGH); // HIGH = locked
  digitalWrite(LED_GREEN_PIN, LOW);
  digitalWrite(LED_RED_PIN, HIGH);   // Red LED on
  
  Serial.println("Door locked again");
}

void handleUnauthorizedAccess() {
  Serial.println("ACCESS DENIED");
  
  // Flash red LED and sound alarm
  for (int i = 0; i < 5; i++) {
    digitalWrite(LED_RED_PIN, LOW);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(200);
    digitalWrite(LED_RED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, LOW);
    delay(200);
  }
}

void sendNotificationToServer(String accessType, int fingerID, int confidence) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected. Cannot send notification.");
    return;
  }
  
  HTTPClient http;
  http.begin(serverURL);
  http.addHeader("Content-Type", "application/json");
  
  // Create JSON payload
  StaticJsonDocument<200> doc;
  doc["timestamp"] = millis();
  doc["accessType"] = accessType;
  doc["fingerID"] = fingerID;
  doc["confidence"] = confidence;
  doc["deviceID"] = "ESP32_DOOR_001";
  doc["location"] = "Front Door";
  
  String jsonString;
  serializeJson(doc, jsonString);
  
  Serial.println("Sending notification to server...");
  Serial.println("Payload: " + jsonString);
  
  int httpResponseCode = http.POST(jsonString);
  
  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("Server response code: " + String(httpResponseCode));
    Serial.println("Server response: " + response);
  } else {
    Serial.println("Error sending notification. Response code: " + String(httpResponseCode));
  }
  
  http.end();
}

void playSuccessTone() {
  tone(BUZZER_PIN, 1000, 200);
  delay(250);
  tone(BUZZER_PIN, 1200, 200);
  delay(250);
  tone(BUZZER_PIN, 1400, 300);
}

void playStartupTone() {
  tone(BUZZER_PIN, 800, 100);
  delay(150);
  tone(BUZZER_PIN, 1000, 100);
  delay(150);
  tone(BUZZER_PIN, 1200, 100);
}

void signalError() {
  for (int i = 0; i < 3; i++) {
    tone(BUZZER_PIN, 400, 100);
    delay(150);
  }
}