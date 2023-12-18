/**
* @file SMAF-Development-Kit.ino
* @brief Main Arduino sketch for the SMAF-Development-Kit project.
*
* @license MIT License
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*
*/

#include "Helpers.h"
#include "WiFi.h"
#include "WiFiConfig.h"

// Define constants for ESP32 core numbers.
#define ESP32_CORE_PRIMARY 0    // Numeric value representing the primary core.
#define ESP32_CORE_SECONDARY 1  // Numeric value representing the secondary core.

// Enum to represent different device statuses
enum DeviceStatusEnum : byte {
  NONE,             // Disable RGB led.
  NOT_READY,        // Device is not ready.
  READY_TO_SEND,    // Device is ready to send data.
  WAITING_GNSS,     // Device is waiting for GNSS data.
  MAINTENANCE_MODE  // Device is in maintenance mode.
};

// Variable to store the current device status.
DeviceStatusEnum deviceStatus = NONE;  // Initial state is set to NOT_READY.

// Function prototype for the DeviceStatusThread function.
void DeviceStatusThread(void* pvParameters);

// Define the pin for the configuration button.
int configurationButton = D2;

String configNetworkName = "SMAF-DK-SAP-CONFIG";
String configNetworkPass = "Kurwe01!";
uint16_t configServerPort = 8080;
WiFiConfig config(configNetworkName, configNetworkPass, configServerPort);

/**
* @brief Initializes the SMAF-Development-Kit and runs once at the beginning.
*
* This function is responsible for the initial setup of the SMAF-Development-Kit.
* It is executed once when the Arduino board starts or is reset.
* Configuration settings, pin modes, and other one-time setup tasks can be performed here.
*
*/
void setup() {
  // Initialize serial communication at a baud rate of 115200.
  Serial.begin(115200);

  // Turn off all LEDs initially.
  digitalWrite(LED_RED, !LOW);
  digitalWrite(LED_GREEN, !LOW);
  digitalWrite(LED_BLUE, !LOW);

  // Set the pin mode for the configuration button to INPUT.
  pinMode(configurationButton, INPUT);

  // Delay for 2400 milliseconds (2.4 seconds).
  delay(2400);

  // Print a formatted welcome message with build information.
  String buildVersion = "v0.1 (Revision-01)";
  String buildDate = "January 2024.";
  Serial.printf("\n\rSMAF-DEVELOPMENT-KIT, Crafted with love in Europe.\n\rBuild version: \t%s\n\rBuild date: \t%s\n\r\n\r", buildVersion.c_str(), buildDate.c_str());

  // Create a new task (DeviceStatusThread) and assign it to the primary core (ESP32_CORE_PRIMARY).
  xTaskCreatePinnedToCore(
    DeviceStatusThread,    // Function to implement the task.
    "DeviceStatusThread",  // Name of the task.
    8000,                  // Stack size in words.
    NULL,                  // Task input parameter (e.g., delay).
    1,                     // Priority of the task.
    NULL,                  // Task handle.
    ESP32_CORE_PRIMARY     // Core where the task should run.
  );

  // Set preferences namespace.
  String preferencesNamespace = "SMAF-DK";
  debug(CMD, "Setting preferences namespace to '" + preferencesNamespace + "'.");
  config.setPreferencesNamespace(preferencesNamespace);
  debug(SCS, "Preferences namespace set to'" + config.getPreferencesNamespace() + "'.");

  // Clear all preferences in namespace.
  // config.clearPreferencesInNamespace(config.getPreferencesNamespace());

  // Load Wi-Fi and MQTT configuration preferences.
  debug(CMD, "Loading preferences from '" + config.getPreferencesNamespace() + "' namespace.");
  config.loadPreferences();

  // Log preferences information.
  debug(LOG, "Network Name: '" + config.getNetworkName() + "'.");
  debug(LOG, "Network Password: '" + config.getNetworkPass() + "'.");
  debug(LOG, "MQTT Server address: '" + config.getMqttServerAddress() + "'.");
  debug(LOG, "MQTT Server port: '" + String(config.getMqttServerPort()) + "'.");
  debug(LOG, "MQTT Username: '" + config.getMqttUsername() + "'.");
  debug(LOG, "MQTT Password: '" + config.getMqttPass() + "'.");
  debug(LOG, "MQTT Client ID: '" + config.getMqttClientId() + "'.");
  debug(LOG, "MQTT Topic: '" + config.getMqttTopic() + "'.");

  // Check if configuration preferences are valid and log the result.
  (!config.isConfigValid()) ? debug(ERR, "Preferences not valid.") : debug(SCS, "Preferences are valid.");

  // Check if SoftAP configuration server should be started.
  if ((digitalRead(configurationButton) == HIGH) || (!config.isConfigValid())) {
    // Set device status to Maintenance Mode.
    deviceStatus = MAINTENANCE_MODE;

    // Log information about starting SoftAP configuration server.
    debug(CMD, "Starting the SoftAP configuration server either because it was initiated by the user or the configuration was incomplete.");
    config.startConfig();
    debug(SCS, "SoftAP configuration server started.");
  } else {
    // Set device status to Not Ready.
    deviceStatus = NOT_READY;
  }
}

/**
* @brief Main execution loop for the SMAF-Development-Kit.
*
* This function runs repeatedly in a loop after the initial setup.
* It is the core of your Arduino program, where continuous tasks and operations should be placed.
* Be mindful of keeping the loop efficient and avoiding long blocking operations.
*
*/
void loop() {
  if (deviceStatus == MAINTENANCE_MODE) {
    config.renderConfigPage();
  } else {
    debug(LOG, "Hello world.");
    delay(800);
  }
}

/**
* @brief Device status handling thread.
*
* This function is a FreeRTOS thread that continuously monitors the device status
* and takes corresponding actions based on the status.
*
* @param pvParameters Pointer to task parameters (not used in this function).
*/
void DeviceStatusThread(void* pvParameters) {
  while (true) {
    // Turn off all LEDs initially.
    digitalWrite(LED_RED, !LOW);
    digitalWrite(LED_GREEN, !LOW);
    digitalWrite(LED_BLUE, !LOW);

    // Handle different device statuses.
    switch (deviceStatus) {
      case NONE:
        // Disable RGB led.
        digitalWrite(LED_RED, !LOW);
        digitalWrite(LED_GREEN, !LOW);
        digitalWrite(LED_BLUE, !LOW);
        break;

      case NOT_READY:
        // Blink the red LED to indicate the device is not ready.
        digitalWrite(LED_RED, !HIGH);
        delay(160);
        digitalWrite(LED_RED, !LOW);
        delay(160);
        break;

      case READY_TO_SEND:
        // Blink the green LED multiple times to indicate the device is ready to send.
        for (int i = 0; i < 4; ++i) {
          digitalWrite(LED_GREEN, !HIGH);
          delay(40);
          digitalWrite(LED_GREEN, !LOW);
          delay(40);
        }

        // Additional delay after blinking for visibility.
        delay(1200);
        break;

      case WAITING_GNSS:
        // Blink the blue LED to indicate the device is waiting for GNSS signal lock.
        digitalWrite(LED_BLUE, !HIGH);
        delay(160);
        digitalWrite(LED_BLUE, !LOW);
        delay(160);
        break;

      case MAINTENANCE_MODE:
        // Blink both blue and red LEDs to indicate the device is in maintenance mode.
        digitalWrite(LED_BLUE, !HIGH);
        digitalWrite(LED_RED, !HIGH);
        delay(160);
        digitalWrite(LED_BLUE, !LOW);
        digitalWrite(LED_RED, !LOW);
        delay(160);
        break;
    }
  }
}