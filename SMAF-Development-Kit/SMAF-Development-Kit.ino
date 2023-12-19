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
#include "DeviceStatusVisualizer.h"

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

// SoftAP configuration parameters.
String configNetworkName = "SMAF-DK-SAP-CONFIG";
String configNetworkPass = "Kurwe01!";
String preferencesNamespace = "SMAF-DK";
uint16_t configServerPort = 80;

// WiFiConfig instance with the specified configuration.
WiFiConfig config(configNetworkName, configNetworkPass, configServerPort);

// Instantiate DeviceStatusVisualizer with RGB LED pins: LED_RED, LED_GREEN, LED_BLUE.
DeviceStatusVisualizer statusVisualizer(LED_RED, LED_GREEN, LED_BLUE);

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
    debug(LOG, "SoftAP Name: '" + config.getConfigNetworkName() + "'");
    debug(LOG, "SoftAP Password: '" + config.getConfigNetworkPass() + "'");
    debug(LOG, "SoftAP Server IP address: '" + config.getConfigServerIP() + "'");
    debug(LOG, "SoftAP Server port: '" + String(config.getConfigServerPort()) + "'");
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
  while (deviceStatus == MAINTENANCE_MODE) {
    config.renderConfigPage();
  }

  debug(LOG, "Hello world.");
  delay(800);
}

/**
* @brief Thread function for handling device status indications through an RGB LED.
*
* This thread continuously updates the RGB LED status based on the current device status.
* It uses the DeviceStatusEnum values to determine the appropriate LED indication.
*
* @param pvParameters Pointer to task parameters (not used in this function).
*/
void DeviceStatusThread(void* pvParameters) {
  while (true) {
    // Turn off all LEDs before updating the status indication.
    statusVisualizer.shutOffAll();

    // Update LED status based on the current device status.
    switch (deviceStatus) {
      case NONE:
        // No specific indication for 'NONE' status.
        statusVisualizer.shutOffAll();
        break;

      case NOT_READY:
        // Blink the LED in red to indicate 'NOT_READY' status.
        statusVisualizer.blinkRed(240);
        break;

      case READY_TO_SEND:
        // Burst the LED in green to indicate 'READY_TO_SEND' status.
        statusVisualizer.burstGreen(40, 1200, 4);
        break;

      case WAITING_GNSS:
        // Blink the LED in blue to indicate 'WAITING_GNSS' status.
        statusVisualizer.blinkBlue(240);
        break;

      case MAINTENANCE_MODE:
        // Blink the LED in purple to indicate 'MAINTENANCE_MODE' status.
        statusVisualizer.blinkPurple(240);
        break;
    }
  }
}