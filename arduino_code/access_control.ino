// -----------------------------------------------------------------------------
// Main sketch for the Smart Car Parking Access Control System
// This file contains the primary setup and loop functions.
// It acts as the orchestrator for the different modules.
// -----------------------------------------------------------------------------

#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// Include our custom modules. We will create these files next.
#include "rfid_handler.h"
#include "gate_handler.h"
#include "system_state.h"
#include "slot_handler.h"
#include "lcd_handler.h"

// --- Global System Configuration ---
const char* WIFI_SSID = "Vraddd's s24+";
const char* WIFI_PASSWORD = "00000000";
String GOOGLE_SCRIPT_URL = "https://script.google.com/macros/s/AKfycbyRcDtN5GZJNZLhHhud2-y1SFkVKnksgVBEqPpy3og8myL6VQOumz0wz_iZxcMJ-ed7EQ/exec";

// --- MQTT Configuration ---
const char* MQTT_BROKER = "your_mqtt_broker_address"; // <-- IMPORTANT: Change this
const int MQTT_PORT = 1883;
const char* MQTT_TOPIC_DOOR_OPEN = "parksensei/gate/control";

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);


bool userJustValidated = false;
// -----------------------------------------------------------------------------
// SETUP: Runs once on boot.
// -----------------------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  Serial.println("Booting Access Control System...");

  // Initialize modules one by one
  setupWifi();
  setupMqtt();
  setupRfid();
  setupGate();
  setupSlots();
  setupLcd();

  Serial.println("System Initialized. Ready.");
}

// -----------------------------------------------------------------------------
// LOOP: Runs continuously.
// This is the core of our non-blocking system. Each handler function
// will be designed to run quickly and not use delay().
// -----------------------------------------------------------------------------
void loop() {
  mqttClient.loop(); // Handles MQTT messages
  handleRfid(); // Checks for new cards and handles validation
  handleGate(); // Checks the gate timer and closes the gate if needed
  handleSlots();
}


// -----------------------------------------------------------------------------
// Helper function to connect to WiFi.
// We can move this to a "network_handler.cpp" file later to be even cleaner.
// -----------------------------------------------------------------------------
void setupWifi() {
  Serial.print("Connecting to WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected.");
}

// -----------------------------------------------------------------------------
// MQTT callback function. This is called when a message is received.
// -----------------------------------------------------------------------------
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  Serial.println("Message arrived on topic: " + String(topic));

  // 1. Deserialize the JSON document
  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc, payload, length);

  // 2. Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }

  // 3. Extract values from the JSON document
  const char* action = doc["action"]; // e.g., "open"
  const char* barrier = doc["barrier"]; // e.g., "entry" or "exit"
  int slotNumber = doc["slotNumber"]; // e.g., 5

  // 4. Perform actions based on the content of the message
  if (strcmp(action, "open") == 0) {
    if (strcmp(barrier, "entry") == 0) {
      Serial.println("Received command to open ENTRY barrier.");
      openEntryGate();
      displayEntryMessage(slotNumber);
    } else if (strcmp(barrier, "exit") == 0) {
      Serial.println("Received command to open EXIT barrier.");
      openExitGate();
      displayExitMessage();
    } else {
      Serial.println("Unknown barrier type in message.");
    }
  } else {
    Serial.println("Unknown action in message.");
  }
}


// -----------------------------------------------------------------------------
// Helper function to connect to the MQTT broker.
// -----------------------------------------------------------------------------
void setupMqtt() {
  mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
  mqttClient.setCallback(mqttCallback);

  while (!mqttClient.connected()) {
    Serial.print("Connecting to MQTT broker...");
    if (mqttClient.connect("access-control-system")) {
      Serial.println("connected.");
      mqttClient.subscribe(MQTT_TOPIC_DOOR_OPEN);
      Serial.println("Subscribed to topic: " + String(MQTT_TOPIC_DOOR_OPEN));
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}