/**
* @file WiFiConfig.cpp
* @brief Implementation of WiFiConfig functions for SoftAP configurator.
*
* This file contains the implementation of helper functions used in the Arduino project
* for configuring Wi-Fi and MQTT settings using a SoftAP (Access Point). It includes
* functionality to start the SoftAP configuration process, render a configuration page,
* load and save configuration preferences, and more.
*
* @note This library assumes the use of the Preferences library for storing configuration
* parameters in non-volatile memory.
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
*/

#include "Arduino.h"
#include "WiFi.h"
#include "WiFiServer.h"
#include "Preferences.h"
#include "WiFiConfig.h"
#include "Helpers.h"

/**
* @brief Constructor for WiFiConfig class.
*
* Initializes an instance of the WiFiConfig class with the provided configuration parameters.
*
* @param configNetworkName The Wi-Fi network name for configuration.
* @param configNetworkPass The Wi-Fi network password for configuration.
* @param configServerPort The port for the SoftAP configuration server.
* @param preferencesNamespace The namespace for storing configuration preferences.
*/
WiFiConfig::WiFiConfig(const char* configNetworkName, const char* configNetworkPass, uint16_t configServerPort, const char* preferencesNamespace)
  : _configNetworkName(configNetworkName),
    _configNetworkPass(configNetworkPass),
    _configServerPort(configServerPort),
    _configServerInstance(configServerPort),
    _preferencesNamespace(preferencesNamespace) {
  // Constructor implementation goes here
}

/**
* @brief Start the Wi-Fi configuration process.
*
* This method initiates the Wi-Fi configuration process by setting up a SoftAP
* (Access Point) with the specified network name and password. It introduces a delay
* for SoftAP initialization and then begins the SoftAP configuration server instance.
*
* @note Ensure that the SoftAP configuration server instance has been initialized
*       before calling this method.
*/
void WiFiConfig::startConfiguration() {
  // Start SoftAP with the specified network name and password.
  WiFi.softAP(_configNetworkName, _configNetworkPass);

  // Introduce a delay for SoftAP initialization.
  delay(800);

  // Begin the configuration server instance.
  _configServerInstance.begin();

  // Display SoftAP information.
  debug(CMD, "Starting configuration server.");
  debug(SCS, "SoftAP configuration server started. Use the credentials below to enter configuration mode.");
  debug(LOG, "SoftAP Name: '%s'.", getConfigNetworkName());
  debug(LOG, "SoftAP Password: '%s'.", getConfigNetworkPass());
  debug(LOG, "SoftAP Server IP address: '%s'.", getConfigServerIp());
  debug(LOG, "SoftAP Server port: '%d'.", getConfigServerPort());
}

/**
* @brief Render the configuration page for device setup.
* 
* This function serves an HTML configuration page to the connected client.
* It processes the form submission and saves the configuration settings.
* 
* @note The HTML structure and styling are included for presentation purposes.
*/
void WiFiConfig::renderConfigurationPage() {
  // Check if a client has connected.
  WiFiClient client = _configServerInstance.accept();

  if (!client) {
    return;  // No client, exit the loop.
  }

  // Wait until the client sends some data.
  while (!client.available()) {
    delay(10);
  }

  // Read the first line of the request.
  String request = client.readStringUntil('\r');
  //client.flush();

  /**
  * @note THIS WILL BE UPDATED IN FUTURE VERSION.
  */
  // Serve the HTML page.
  String html = String();

  html += "<!DOCTYPE html>";
  html += "<html lang=\"en\">";
  html += "<head>";
  html += "<meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">";
  html += "<title>SMAF-DK-SAP</title>";
  html += "<script> function refreshScan() {window.location.href = '/refresh';} </script>";
  html += "<style>";
  html += ":root {";
  html += "--monochrome-100: hsl(210, 10%, 10%); --monochrome-125: hsl(210, 10%, 50%); --monochrome-150: hsl(210, 10%, 70%); --monochrome-200: hsl(210, 10%, 85%); --monochrome-250: hsl(210, 10%, 95%); --monochrome-300: hsl(0, 0%, 100%);";
  html += "--info-50: hsl(210, 100%, 20%); --info-75: hsl(210, 100%, 35%); --info-100: hsl(210, 100%, 50%); --info-200: hsl(210, 100%, 95%);";
  html += "--success-50: hsl(130, 100%, 15%); --success-75: hsl(130, 100%, 25%); --success-100: hsl(130, 100%, 40%); --success-200: hsl(130, 100%, 95%);";
  html += "--error-50: hsl(0, 100%, 24%); --error-75: hsl(0, 100%, 35%); --error-100: hsl(0, 100%, 60%); --error-200: hsl(0, 100%, 97%);";
  html += "}";
  html += "* {font-family: system-ui, sans-serif; font-size: 16px; line-height: 1.5; color: var(--monochrome-100); margin: 0; padding: 0; box-sizing: border-box; outline: none; list-style: none; word-wrap: break-words; cursor: default;}";
  html += "body {display: flex;flex-direction: column;flex-wrap: nowrap;align-items: center;padding: 1.5rem 1.5rem 8rem;}";
  html += "h1, h2, h3, h4, h5, h6 {color: inherit; line-height: 1.15; margin-top: 3.5rem; margin-bottom: 1rem; font-weight: 700; letter-spacing: -0.2px}";
  html += "h1 {font-size: 2.027rem; font-weight: 700;}";
  html += "h2 {font-size: 1.802rem;}";
  html += "h3 {font-size: 1.602rem;}";
  html += "h4 {font-size: 1.424rem;}";
  html += "h5 {font-size: 1.266rem; margin-bottom: 0.5rem;}";
  html += "h6 {font-size: 1.125rem; margin-bottom: 0.5rem;}";
  html += "p {color: inherit; margin-top: 1rem; margin-bottom: 1rem;}";
  html += "label {font-weight: 500;}";
  html += "form {max-width: 460px;}";
  html += "input[type='text'], input[type='submit'], input[type='reset'], select, input[type='checkbox'], button {all: unset;}";
  html += "input[type='text'], select {font-family: monospace, sans-serif; padding: 0.75rem 1rem; box-shadow: 0 0 0 1px var(--monochrome-200) inset; cursor: text;}";
  html += "input[type='text']:hover, select:hover {box-shadow: 0 0 0 2px var(--monochrome-200) inset;}";
  html += "input[type='text']:focus, select:focus {box-shadow: 0 0 0 2px var(--info-100) inset;}";
  html += "input[type='submit'], input[type='reset'], button {font-weight: 500; cursor: pointer; padding: 1rem 1.5rem; flex-grow: 2; text-align: center;}";
  html += "input[type='submit'] {background: var(--info-100); color: var(--monochrome-300);}";
  html += "input[type='reset'], button {box-shadow: 0 0 0 1px var(--monochrome-200) inset; flex-shrink: 2; flex-grow: 1;}";
  html += "input[type='submit']:hover {background: var(--info-75);}";
  html += "input[type='submit']:active {background: var(--info-50);}";
  html += "input[type='reset']:hover, button:hover {box-shadow: 0 0 0 2px var(--monochrome-200) inset;}";
  html += "input[type='reset']:active, button:active {box-shadow: 0 0 0 2px var(--monochrome-200) inset; background: var(--monochrome-250);}";
  html += ".horizontal-frame {display: flex; flex-wrap: wrap; flex-direction: row; gap: 1.0rem; margin-top: 1.0rem;}";
  html += "section {border-left: 3px solid var(--info-100); background: var(--info-200); color: var(--info-50); padding: 1rem 1.25rem; margin: 1.5rem 0rem;}";
  html += "section.success {border-left: 3px solid var(--success-100); background: var(--success-200); color: var(--success-50);}";
  html += "section p {margin: 0; padding: 0;}";
  html += "section h6 {margin-top: 0;}";
  html += ".frame {display: flex; flex-direction: column; gap: 1.5rem; margin-top: 1.5rem;}";
  html += ".input-frame {display: flex; flex-direction: column; gap: 0.25rem;}";
  html += ".checkbox-frame {display: flex; flex-direction: row; justify-content: space-between; align-content: center; align-items: center; gap: 0.5rem;}";
  html += ".switch {position: relative; display: flex; flex-shrink: 0; width: 40px; height: 24px;}";
  html += ".track {cursor: pointer; display: flex; justify-content: flex-start; align-items: center; background-color: var(--monochrome-200); box-shadow: 0 0 0 3px var(--monochrome-200); width: 100%; height: 100%; border-radius: 100px;}";
  html += ".track:hover {background-color: var(--monochrome-150); box-shadow: 0 0 0 3px var(--monochrome-150);}";
  html += ".track:active {background-color: var(--monochrome-125); box-shadow: 0 0 0 3px var(--monochrome-125);}";
  html += ".thumb {display: flex; justify-content: center; align-items: center; width: 24px; height: 24px; pointer-events: none; border-radius: 100%; box-shadow: 0 0 0 9.5px var(--monochrome-300) inset;}";
  html += "input:checked + .track {background-color: var(--info-100); box-shadow: 0 0 0 3px var(--info-100); justify-content: flex-end;}";
  html += "input:checked + .track:hover {background-color: var(--info-75); box-shadow: 0 0 0 3px var(--info-75);}";
  html += "input:checked + .track:active {background-color: var(--info-50); box-shadow: 0 0 0 3px var(--info-50);}";
  html += ".h1-override {margin-top: 1.5rem; margin-bottom: 1.5rem;}";
  html += ".fake-link {text-decoration: underline; color: var(--info-100); font-weight: 500; cursor: pointer;}";
  html += "em {all: unset; color: var(--error-100); font-weight: 500;}";
  html += "</style>";
  html += "</head>";
  html += "<body>";

  html += "<form action='/configuration' method='get'>";
  html += "<h1>🤙</h1>";
  html += "<h1 class=\"h1-override\">Ready to update<br>your settings?</h1>";
  html += "<p>Welcome to SMAF Config Hub! Quickly set up your SMAF device to connect via WiFi and transmit data using MQTT.</p>";

  // Check if the request is a form submission and save preferences.
  if (request.indexOf("/configuration") != -1) {
    // Display a success message with the saved configuration.
    html += "<section class='success' style=\"display: block;\">";
    html += "<h6>Success!</h6>";
    html += "<p>Your SMAF device has successfully absorbed the new configuration. It's now all set to rock and roll with the updated settings.</p>";
    html += "</section>";
  }

  html += "<h4>WiFi router<br>configuration</h4>";
  html += "<p>Secure connectivity by entering your WiFi details - SSID and password. SMAF stays linked to the network for seamless operation.</p>";
  html += "<p class=\"fake-link\" onclick=\"refreshScan()\">Refresh network list</p>";
  html += "<div class=\"frame\">";
  html += "<div class=\"input-frame\">";
  html += "<label for='" + String(NETWORK_NAME) + "'>Select SSID<em>*</em></label>";
  html += "<select id='" + String(NETWORK_NAME) + "' type='text' name='" + String(NETWORK_NAME) + "' required>";

  html += scanNetworks();

  html += "</select>";
  html += "</div>";
  html += "<div class=\"input-frame\">";
  html += "<label for='" + String(NETWORK_PASS) + "'>SSID Password<em>*</em></label>";
  html += "<input id='" + String(NETWORK_PASS) + "' type='text' name='" + String(NETWORK_PASS) + "' value='" + getNetworkPass() + "' required>";
  html += "</div>";
  html += "</div>";
  html += "<h4>MQTT server<br>configuration</h4>";
  html += "<p>Tune communication with MQTT server settings. Enter the broker's address, port, and authentication details for a robust connection.</p>";
  html += "<div class=\"frame\">";
  html += "<div class=\"input-frame\">";
  html += "<label for='" + String(MQTT_SERVER_ADDRESS) + "'>MQTT Server<em>*</em></label>";
  html += "<input id='" + String(MQTT_SERVER_ADDRESS) + "' type='text' name='" + String(MQTT_SERVER_ADDRESS) + "' value='" + getMqttServerAddress() + "' required>";
  html += "</div>";
  html += "<div class=\"input-frame\">";
  html += "<label for='" + String(MQTT_SERVER_PORT) + "'>MQTT Port<em>*</em></label>";
  html += "<input id='" + String(MQTT_SERVER_PORT) + "' type='text' inputmode='numeric' pattern='[0-9]*' name='" + String(MQTT_SERVER_PORT) + "' value='" + String(getMqttServerPort()) + "' required>";
  html += "</div>";
  html += "<div class=\"input-frame\">";
  html += "<label for='" + String(MQTT_USERNAME) + "'>MQTT Username<em>*</em></label>";
  html += "<input id='" + String(MQTT_USERNAME) + "' type='text' name='" + String(MQTT_USERNAME) + "' value='" + getMqttUsername() + "' required>";
  html += "</div>";
  html += "<div class=\"input-frame\">";
  html += "<label for='" + String(MQTT_PASS) + "'>MQTT Password<em>*</em></label>";
  html += "<input id='" + String(MQTT_PASS) + "' type='text' name='" + String(MQTT_PASS) + "' value='" + getMqttPass() + "' required>";
  html += "</div>";
  html += "</div>";
  html += "<h4>MQTT client & topic<br>configuration</h4>";
  html += "<p>Personalize MQTT settings for SMAF by defining client specifics and choosing an optimal topic. Seamless communication is just a click away.</p>";
  html += "<div class=\"frame\">";
  html += "<div class=\"input-frame\">";
  html += "<label for='" + String(MQTT_CLIENT_ID) + "'>MQTT Client ID<em>*</em></label>";
  html += "<input id='" + String(MQTT_CLIENT_ID) + "' type='text' name='" + String(MQTT_CLIENT_ID) + "' value='" + getMqttClientId() + "' required>";
  html += "</div>";
  html += "<div class=\"input-frame\">";
  html += "<label for='" + String(MQTT_TOPIC) + "'>MQTT Topic<em>*</em></label>";
  html += "<input id='" + String(MQTT_TOPIC) + "' type='text' name='" + String(MQTT_TOPIC) + "' value='" + getMqttTopic() + "' required>";
  html += "</div>";
  html += "</div>";
  html += "<h4>Audio/Visual<br>notifications</h4>";
  html += "<p>Your device is equipped with a buzzer and two RGB LEDs to show various statuses of connection. You can enable or disable those if you are irritated by the power of the LEDs or the sound of the buzzer.</p>";
  html += "<div class=\"frame\">";
  html += "<div class=\"checkbox-frame\">";
  html += "<label for='" + String(AUDIO_NOTIFICATIONS) + "'>Enable audio notifications</label>";
  html += "<label class=\"switch\">";
  html += "<input id='" + String(AUDIO_NOTIFICATIONS) + "' type=\"checkbox\" name='" + String(AUDIO_NOTIFICATIONS) + "' value=\"true\"" + (getAudioNotificationsStatus() ? "Checked" : "") + ">";
  html += "<div class=\"track\">";
  html += "<div class=\"thumb\"></div>";
  html += "</div>";
  html += "</label>";
  html += "</div>";
  html += "<div class=\"checkbox-frame\">";
  html += "<label for='" + String(VISUAL_NOTIFICATIONS) + "'>Enable visual notifications</label>";
  html += "<label class=\"switch\">";
  html += "<input id='" + String(VISUAL_NOTIFICATIONS) + "' type=\"checkbox\" name='" + String(VISUAL_NOTIFICATIONS) + "' value=\"true\"" + (getVisualNotificationsStatus() ? "Checked" : "") + ">";
  html += "<div class=\"track\">";
  html += "<div class=\"thumb\"></div>";
  html += "</div>";
  html += "</label>";
  html += "</div>";
  html += "</div>";
  html += "<h4>Finish<br>configuration</h4>";
  html += "<p>Ready to roll? Click \"Upload Configuration\" to apply changes, and SMAF will initiate its own reset to seamlessly implement the updated settings.</p>";
  html += "<section class='info'>";
  html += "<p>Note: Ensure all necessary data is entered correctly; SMAF won't connect or transmit data if something with the data is wrong.</p>";
  html += "</section>";
  html += "<div class=\"horizontal-frame\">";
  html += "<input type=\"reset\" value=\"Reset form\">";
  html += "<input type=\"submit\" value=\"Upload configuration\">";
  html += "</div>";
  html += "</form>";
  html += "</body>";
  html += "</html>";

  // Send the response to the client.
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();
  client.println(html);


  // Check if the request is a form submission and save preferences.
  if (request.indexOf("/configuration") != -1) {
    // Show debug message.
    debug(CMD, "Saving preferences to '%s' namespace.", _preferencesNamespace);

    // Save preferences.
    saveString(NETWORK_NAME, parseFieldValue(request, NETWORK_NAME));
    saveString(NETWORK_PASS, parseFieldValue(request, NETWORK_PASS));
    saveString(MQTT_SERVER_ADDRESS, parseFieldValue(request, MQTT_SERVER_ADDRESS));
    saveInt(MQTT_SERVER_PORT, stringToUint16(parseFieldValue(request, MQTT_SERVER_PORT)));
    saveString(MQTT_USERNAME, parseFieldValue(request, MQTT_USERNAME));
    saveString(MQTT_PASS, parseFieldValue(request, MQTT_PASS));
    saveString(MQTT_CLIENT_ID, parseFieldValue(request, MQTT_CLIENT_ID));
    saveString(MQTT_TOPIC, parseFieldValue(request, MQTT_TOPIC));

    if (parseFieldValue(request, AUDIO_NOTIFICATIONS).isEmpty()) {
      saveBool(AUDIO_NOTIFICATIONS, false);
    } else {
      saveBool(AUDIO_NOTIFICATIONS, true);
    }

    if (parseFieldValue(request, VISUAL_NOTIFICATIONS).isEmpty()) {
      saveBool(VISUAL_NOTIFICATIONS, false);
    } else {
      saveBool(VISUAL_NOTIFICATIONS, true);
    }

    // Show debug message.
    debug(SCS, "Saving preferences to '%s' namespace done.", _preferencesNamespace);
    debug(CMD, "Restarting device to apply preferences.");

    // Short delay before restart.
    delay(2400);

    // Uncomment the following line if a device restart is desired after saving preferences.
    ESP.restart();
  }
}

/**
* @brief Load Wi-Fi and MQTT configuration preferences.
*
* This method reads configuration parameters from non-volatile storage using the
* Preferences library. It loads Wi-Fi network name, password, MQTT server address,
* port, username, password, client ID, and topic. After loading, it checks if all
* essential configuration parameters are non-empty and valid to determine the
* overall configuration validity.
*/
bool WiFiConfig::loadPreferences() {
  // Show debug message.
  debug(CMD, "Loading preferences from '%s' namespace.", _preferencesNamespace);

  // Load all preferences to variables.
  static const char* networkName = getNetworkName();
  static const char* networkPass = getNetworkPass();
  static const char* mqttServerAddress = getMqttServerAddress();
  static const char* mqttUsername = getMqttUsername();
  static const char* mqttPass = getMqttPass();
  static const char* mqttClientId = getMqttClientId();
  static const char* mqttTopic = getMqttTopic();
  static uint16_t mqttServerPort = getMqttServerPort();
  static bool audioNotifications = getAudioNotificationsStatus();
  static bool visualNotifications = getVisualNotificationsStatus();

  // Log preferences information to console.
  debug(LOG, "Network Name: '%s'.", networkName);
  debug(LOG, "Network Password: '%s'.", networkPass);
  debug(LOG, "MQTT Server address: '%s'.", mqttServerAddress);
  debug(LOG, "MQTT Server port: '%d'.", mqttServerPort);
  debug(LOG, "MQTT Username: '%s'.", mqttUsername);
  debug(LOG, "MQTT Password: '%s'.", mqttPass);
  debug(LOG, "MQTT Client ID: '%s'.", mqttClientId);
  debug(LOG, "MQTT Topic: '%s'.", mqttTopic);
  debug(LOG, "Audio notifications %s.", audioNotifications ? "enabled" : "disabled");
  debug(LOG, "Visual notifications %s.", visualNotifications ? "enabled" : "disabled");

  bool isDataValid = true;

  if (isEmpty(networkName) || networkName == "Unknown") {
    isDataValid = false;
  }

  if (isEmpty(networkPass) || networkPass == "Unknown") {
    isDataValid = false;
  }

  if (isEmpty(mqttServerAddress) || mqttServerAddress == "Unknown") {
    isDataValid = false;
  }

  if (isEmpty(mqttUsername) || mqttUsername == "Unknown") {
    isDataValid = false;
  }

  if (isEmpty(mqttPass) || mqttPass == "Unknown") {
    isDataValid = false;
  }

  if (isEmpty(mqttClientId) || mqttClientId == "Unknown") {
    isDataValid = false;
  }

  if (isEmpty(mqttTopic) || mqttTopic == "Unknown") {
    isDataValid = false;
  }

  if (mqttServerPort == 0) {
    isDataValid = false;
  }

  // Show debug message.
  if (isDataValid) {
    debug(SCS, "Preferences data is valid.");
  } else {
    debug(ERR, "Preferences data is not valid. Default values are not sufficient for a successful network connection.");
  }

  return isDataValid;
}

/**
* @brief Clear all preferences within a specific namespace.
*
* This function clears all preferences stored within the specified namespace,
* effectively resetting them to their default values.
*/
void WiFiConfig::clearPreferences() {
  // Create a Preferences instance with the specified namespace.
  Preferences preferences;

  // Begin preferences with the specified namespace.
  preferences.begin(_preferencesNamespace, false);

  // Clear all preferences in the specified namespace.
  preferences.clear();

  // End preferences.
  preferences.end();

  // Uncomment the following line if a device restart is desired after saving preferences.
  // ESP.restart();
}

/**
* @brief Get the configured Wi-Fi network name.
* 
* @return const char* representing the Wi-Fi network name.
*         If empty, returns "NULL".
* 
* @note The returned pointer is valid until the class instance is destroyed,
*       or until the next call to a function that modifies the Wi-Fi network name.
*/
const char* WiFiConfig::getNetworkName() {
  static String data = loadString(NETWORK_NAME);
  return data.c_str();
}

/**
* @brief Get the configured Wi-Fi network password.
* 
* @return const char* representing the Wi-Fi network password.
*         If empty, returns "NULL".
* 
* @note The returned pointer is valid until the class instance is destroyed,
*       or until the next call to a function that modifies the Wi-Fi network password.
*/
const char* WiFiConfig::getNetworkPass() {
  static String data = loadString(NETWORK_PASS);
  return data.c_str();
}

/**
* @brief Get the configured MQTT server address.
* 
* @return const char* representing the MQTT server address.
*         If empty, returns "NULL".
* 
* @note The returned pointer is valid until the class instance is destroyed,
*       or until the next call to a function that modifies the MQTT server address.
*/
const char* WiFiConfig::getMqttServerAddress() {
  static String data = loadString(MQTT_SERVER_ADDRESS);
  return data.c_str();
}

/**
* @brief Get the configured MQTT username.
* 
* @return const char* representing the MQTT username.
*         If empty, returns "NULL".
* 
* @note The returned pointer is valid until the class instance is destroyed,
*       or until the next call to a function that modifies the MQTT username.
*/
const char* WiFiConfig::getMqttUsername() {
  static String data = loadString(MQTT_USERNAME);
  return data.c_str();
}

/**
* @brief Get the configured MQTT password.
* 
* @return const char* representing the MQTT password.
*         If empty, returns "NULL".
* 
* @note The returned pointer is valid until the class instance is destroyed,
*       or until the next call to a function that modifies the MQTT password.
*/
const char* WiFiConfig::getMqttPass() {
  static String data = loadString(MQTT_PASS);
  return data.c_str();
}

/**
* @brief Get the configured MQTT client ID.
* 
* @return const char* representing the MQTT client ID.
*         If empty, returns "NULL".
* 
* @note The returned pointer is valid until the class instance is destroyed,
*       or until the next call to a function that modifies the MQTT client ID.
*/
const char* WiFiConfig::getMqttClientId() {
  static String data = loadString(MQTT_CLIENT_ID);
  return data.c_str();
}

/**
* @brief Get the configured MQTT topic.
* 
* @return const char* representing the MQTT topic.
*         If empty, returns "NULL".
* 
* @note The returned pointer is valid until the class instance is destroyed,
*       or until the next call to a function that modifies the MQTT topic.
*/
const char* WiFiConfig::getMqttTopic() {
  static String data = loadString(MQTT_TOPIC);
  return data.c_str();
}

/**
* @brief Get the status of audio notifications.
* 
* @return bool representing the status of audio notifications.
*         Returns true if audio notifications are enabled, false otherwise.
* 
* @note The returned value is cached after the first call and remains the same for subsequent calls.
*/
bool WiFiConfig::getAudioNotificationsStatus() {
  static bool data = loadBool(AUDIO_NOTIFICATIONS);
  return data;
}

/**
* @brief Get the status of visual notifications.
* 
* @return bool representing the status of visual notifications.
*         Returns true if visual notifications are enabled, false otherwise.
* 
* @note The returned value is cached after the first call and remains the same for subsequent calls.
*/
bool WiFiConfig::getVisualNotificationsStatus() {
  static bool data = loadBool(VISUAL_NOTIFICATIONS);
  return data;
}

/**
* @brief Get the configured MQTT server port.
* 
* @return uint16_t representing the MQTT server port.
*         If not configured, returns a default value.
* 
* @note The returned value is cached after the first call and remains the same for subsequent calls.
*/
uint16_t WiFiConfig::getMqttServerPort() {
  static uint16_t data = loadInt(MQTT_SERVER_PORT);
  return data;
}

/**
*
*
*
* PRIVATE FUNCTIONS
*
*
*
*/

/**
* @brief Get the configured network name for SoftAP.
* 
* @return const char* representing the configured network name.
*         If empty, returns "NULL".
* 
* @note The returned pointer is valid until the class instance is destroyed,
*       or until the next call to a function that modifies the network name.
*/
const char* WiFiConfig::getConfigNetworkName() {
  if (_configNetworkName == nullptr || _configNetworkName[0] == '\0') {
    return "NULL";
  }

  return _configNetworkName;
}

/**
* @brief Get the configured network password for SoftAP.
* 
* @return const char* representing the configured network password.
*         If empty, returns "NULL".
* 
* @note The returned pointer is valid until the class instance is destroyed,
*       or until the next call to a function that modifies the network password.
*/
const char* WiFiConfig::getConfigNetworkPass() {
  if (_configNetworkPass == nullptr || _configNetworkPass[0] == '\0') {
    return "NULL";
  }

  return _configNetworkPass;
}

/**
* @brief Get the IP address of the SoftAP.
* 
* @return const char* representing the IP address of the SoftAP.
*         If empty, returns "0.0.0.0".
* 
* @note The returned pointer is valid until the class instance is destroyed,
*       or until the next call to a function that modifies the SoftAP IP address.
*       It is recommended to avoid modifying the returned pointer directly.
*       If dynamic memory allocation is a concern, consider using a static buffer.
*/
const char* WiFiConfig::getConfigServerIp() {
  static char configServerIpChr[16];  // Assuming IPv4 address, e.g., "192.168.1.1".

  // Get the SoftAP IP address.
  String configServerIpStr = WiFi.softAPIP().toString();

  // Copy the IP address to the static buffer.
  strncpy(configServerIpChr, configServerIpStr.c_str(), sizeof(configServerIpChr) - 1);
  configServerIpChr[sizeof(configServerIpChr) - 1] = '\0';  // Ensure null-termination.

  // Check if the IP address is empty.
  if (configServerIpStr.isEmpty()) {
    return "0.0.0.0";
  }

  return configServerIpChr;
}

/**
* @brief Get the configured configuration server port.
* 
* @return uint16_t representing the configuration server port.
* 
* @note The returned port value is valid for the lifetime of the class instance.
*/
uint16_t WiFiConfig::getConfigServerPort() {
  return _configServerPort;
}

/**
* @brief Scan for available Wi-Fi networks and return them in HTML option format.
* 
* @return String containing the HTML option elements for each available network.
*         If no networks are found, an empty string is returned.
* 
* @note The function scans for Wi-Fi networks, formats them as HTML option elements,
*       and returns the resulting string. It also frees memory used for the scan results
*       after processing.
*/
String WiFiConfig::scanNetworks() {
  int networksFound = WiFi.scanNetworks();
  String networks = String();

  if (networksFound != 0) {
    for (int i = 0; i < networksFound; ++i) {
      String network = String(WiFi.SSID(i).c_str());
      networks += "<option value=\"" + network + "\">" + network + "</option>";
    }

    // delay(10);
  }

  // Delete the scan result to free memory for code below.
  WiFi.scanDelete();

  return networks;
}

/**
* @brief Load a string value from the preferences storage.
* 
* @param key The key associated with the string value to be loaded.
* 
* @return String containing the value associated with the specified key.
*         If the key does not exist, "Unknown" is stored and returned.
*         If the loading fails, a default value is returned.
* 
* @note The function initializes a Preferences instance with the specified namespace,
*       checks if the key exists, stores a default value if it does not, loads the value,
*       and ends the preferences session. If loading fails, an error message is logged
*       and a default value is returned.
*/
String WiFiConfig::loadString(const char* key) {
  // Create a Preferences instance with the specified namespace.
  Preferences preferences;
  static String data;

  if (preferences.begin(_preferencesNamespace, READ_WRITE_MODE)) {
    // Check if key exists and store default value if FALSE.
    if (!preferences.isKey(key)) {
      preferences.putString(key, "Unknown");
    }

    // Load value from key.
    data = preferences.getString(key);

    // End preferences session.
    preferences.end();
  } else {
    // Return a default value if loading fails
    debug(ERR, "Loading '%s' key from '%s' namespace failed. Will use default value.", key, _preferencesNamespace);
  }

  return data.c_str();
}

/**
* @brief Save a string value to the specified key in the preferences namespace.
* 
* @param key The key to which the string value will be saved.
* @param value The string value to save.
* 
* @note This function creates a Preferences instance, attempts to save the value associated 
*       with the given key, and ensures the Preferences session is properly ended. If saving 
*       fails, an error message is logged.
*/
void WiFiConfig::saveString(const char* key, String value) {
  // Create a Preferences instance with the specified namespace.
  Preferences preferences;

  if (preferences.begin(_preferencesNamespace, READ_WRITE_MODE)) {
    // Save the value to the specified key.
    preferences.putString(key, value);

    // End preferences session.
    preferences.end();

    // Show success message.
    debug(SCS, "Data saved to '%s' key in '%s' namespace.", key, _preferencesNamespace);
  } else {
    // Log an error message if saving fails.
    debug(ERR, "Saving data to '%s' key in '%s' namespace failed.", key, _preferencesNamespace);
  }
}

/**
* @brief Load an integer value from the specified key in the preferences namespace.
* 
* @param key The key for the integer value to load.
* 
* @return uint16_t containing the value associated with the key.
*         If the key does not exist, initializes it with a default value of 0 and returns 0.
*         If loading fails, returns 0.
* 
* @note This function creates a Preferences instance, attempts to load the value associated 
*       with the given key, and ensures the Preferences session is properly ended. If the key 
*       does not exist, it initializes the key with a default value of 0.
*/
uint16_t WiFiConfig::loadInt(const char* key) {
  // Create a Preferences instance with the specified namespace.
  Preferences preferences;
  static int data;

  if (preferences.begin(_preferencesNamespace, READ_WRITE_MODE)) {
    // Check if key exists and store default value if FALSE.
    if (preferences.isKey(key) == false) {
      preferences.putInt(key, 0);
    }

    // Load value from key.
    data = preferences.getInt(key);

    // End preferences session.
    preferences.end();
  } else {
    // Log an error message and return a default value if loading fails.
    debug(ERR, "Loading '%s' key from '%s' namespace failed. Will use default value.", key, _preferencesNamespace);
    data = 0;
  }

  return data;
}

/**
* @brief Save an integer value to the specified key in the preferences namespace.
* 
* @param key The key to which the integer value will be saved.
* @param value The integer value to save.
* 
* @note This function creates a Preferences instance, attempts to save the value associated 
*       with the given key, and ensures the Preferences session is properly ended. If saving 
*       fails, an error message is logged.
*/
void WiFiConfig::saveInt(const char* key, uint16_t value) {
  // Create a Preferences instance with the specified namespace.
  Preferences preferences;

  if (preferences.begin(_preferencesNamespace, READ_WRITE_MODE)) {
    // Save the value to the specified key.
    preferences.putInt(key, value);

    // End preferences session.
    preferences.end();

    // Show success message.
    debug(SCS, "Data saved to '%s' key in '%s' namespace.", key, _preferencesNamespace);
  } else {
    // Log an error message if saving fails.
    debug(ERR, "Saving data to '%s' key in '%s' namespace failed.", key, _preferencesNamespace);
  }
}

/**
* @brief Load a boolean value from the specified key in the preferences namespace.
* 
* @param key The key for the boolean value to load.
* 
* @return bool containing the value associated with the key.
*         If the key does not exist, initializes it with a default value of true and returns true.
*         If loading fails, returns false.
* 
* @note This function creates a Preferences instance, attempts to load the value associated 
*       with the given key, and ensures the Preferences session is properly ended. If the key 
*       does not exist, it initializes the key with a default value of true.
*/
bool WiFiConfig::loadBool(const char* key) {
  // Create a Preferences instance with the specified namespace.
  Preferences preferences;
  static bool data;

  if (preferences.begin(_preferencesNamespace, READ_WRITE_MODE)) {
    // Check if key exists and store default value if FALSE.
    if (preferences.isKey(key) == false) {
      preferences.putBool(key, true);
    }

    // Load value from key.
    data = preferences.getBool(key);

    // End preferences session.
    preferences.end();
  } else {
    // Log an error message and return a default value if loading fails.
    debug(ERR, "Loading '%s' key from '%s' namespace failed. Will use default value.", key, _preferencesNamespace);
    data = false;
  }

  return data;
}

/**
* @brief Save a boolean value to the specified key in the preferences namespace.
* 
* @param key The key to which the boolean value will be saved.
* @param value The boolean value to save.
* 
* @note This function creates a Preferences instance, attempts to save the value associated 
*       with the given key, and ensures the Preferences session is properly ended. If saving 
*       fails, an error message is logged.
*/
void WiFiConfig::saveBool(const char* key, bool value) {
  // Create a Preferences instance with the specified namespace.
  Preferences preferences;

  if (preferences.begin(_preferencesNamespace, READ_WRITE_MODE)) {
    // Save the value to the specified key.
    preferences.putBool(key, value);

    // End preferences session.
    preferences.end();

    // Show success message.
    debug(SCS, "Data saved to '%s' key in '%s' namespace.", key, _preferencesNamespace);
  } else {
    // Log an error message if saving fails.
    debug(ERR, "Saving data to '%s' key in '%s' namespace failed.", key, _preferencesNamespace);
  }
}

/**
* @brief Parse and extract the value of a field from a URL-encoded String.
*
* This function takes a URL-encoded String containing field-value pairs and
* extracts the value associated with the specified field ID. The field ID should
* be provided as a parameter. The extracted value is then URL-decoded and leading
* and trailing spaces are removed. If the field is not found or the extracted value
* is empty, an empty String is returned.
*
* @param data The URL-encoded String containing field-value pairs.
* @param fieldId The field ID for which to extract the value.
* @return The extracted and decoded value of the specified field, or an empty String if not found.
*
* @see decodeResponse()
* @see removeSpaces()
*/
String WiFiConfig::parseFieldValue(String data, String fieldId) {
  // Find the index of the specified field ID in the data String.
  int startIndex = data.indexOf(fieldId + "=");

  // If the field ID is not found, return an empty String.
  if (startIndex == -1) {
    return String();
  }

  // Adjust the startIndex to the position of the value after '='
  startIndex += fieldId.length() + 1;
  String value = String();

  // Find the indices of the next ampersand (&) and " HTTP" in the data String.
  int ampIndex = data.indexOf("&", startIndex);
  int httpIndex = data.indexOf(" HTTP", startIndex);

  // Determine the end index based on the minimum of ampIndex and httpIndex, or the end of the data String.
  int endIndex = data.length();

  if (ampIndex != -1 && (ampIndex < httpIndex || httpIndex == -1)) {
    endIndex = ampIndex;
  } else if (httpIndex != -1) {
    endIndex = httpIndex;
  }

  // Extract the value substring based on the determined indices.
  value = data.substring(startIndex, endIndex);

  // Return an empty String if the extracted value is empty, otherwise, URL-decode and remove spaces.
  return value.isEmpty() ? String() : removeSpaces(decodeResponse(value));
}

/**
* @brief Decode a URL-encoded String.
*
* This function takes a URL-encoded String as input and decodes it, replacing
* percent-encoded characters with their corresponding ASCII characters. It also
* replaces the plus sign (+) with a space (' '). The decoded String is returned.
*
* @param input The URL-encoded String to decode.
* @return The decoded String.
*/
String WiFiConfig::decodeResponse(String input) {
  // Initialize an empty String to store the decoded result.
  String decoded = String();

  // Temporary variables to store hexadecimal characters during decoding.
  char a, b;

  // Iterate through the characters in the input String.
  for (size_t i = 0; i < input.length(); i++) {
    // If a percent sign (%) is encountered, extract the two hexadecimal characters
    // following it, convert them to a byte, and append the corresponding ASCII character
    // to the decoded String. Increment the loop index accordingly.
    if (input[i] == '%') {
      a = input[i + 1];
      b = input[i + 2];

      decoded += char(hexToByte(a) * 16 + hexToByte(b));

      // Skip the next two characters since they have been processed.
      i += 2;
    }
    // If a plus sign (+) is encountered, append a space to the decoded String.
    else if (input[i] == '+') {
      decoded += ' ';
    }
    // If neither percent sign nor plus sign is encountered, append the character
    // unchanged to the decoded String.
    else {
      decoded += input[i];
    }
  }

  // Return the final decoded String.
  return decoded;
}

/**
* @brief Remove spaces from a String.
*
* This function takes a String as input and removes leading and trailing spaces.
* If the input String has no spaces, it is returned unchanged. If the input String
* has leading or trailing spaces, those spaces are removed, and the modified String
* is returned.
*
* @param str The input String from which to remove spaces.
* @return The modified String with leading and trailing spaces removed.
*/
String WiFiConfig::removeSpaces(String str) {
  // Check if the input String has a length greater than 0.
  if (str.length() > 0) {
    // Iterate through the characters in the String.
    for (size_t i = 0; i < str.length(); ++i) {
      // If a non-space character is encountered, return the original String.
      if (str[i] != ' ') {
        return str;
      }
    }

    // If the entire String consists of spaces, return an empty String.
    return String();
  }

  // Return the original String if it has no length.
  return str;
}

/**
* @brief Convert a hexadecimal character to a byte.
*
* This function takes a single character and converts it to a byte based on its
* hexadecimal representation. If the character is a digit ('0' to '9'), the
* corresponding decimal value is returned. If the character is a lowercase
* hexadecimal digit ('a' to 'f'), the corresponding decimal value (10 to 15) is
* returned. If the character is an uppercase hexadecimal digit ('A' to 'F'),
* the corresponding decimal value (10 to 15) is returned. If the input character
* is not a valid hexadecimal digit, 0 is returned.
*
* @param c The hexadecimal character to convert.
* @return The decimal value of the hexadecimal character or 0 if not a valid digit.
*/
byte WiFiConfig::hexToByte(char c) {
  // Check if the character is a digit ('0' to '9').
  if ('0' <= c && c <= '9') {
    return c - '0';
  }

  // Check if the character is a lowercase hexadecimal digit ('a' to 'f').
  if ('a' <= c && c <= 'f') {
    return c - 'a' + 10;
  }

  // Check if the character is an uppercase hexadecimal digit ('A' to 'F').
  if ('A' <= c && c <= 'F') {
    return c - 'A' + 10;
  }

  // Return 0 if the input character is not a valid hexadecimal digit.
  return 0;
}

/**
* @brief Convert a String to a uint16_t.
*
* This function converts the provided String to an integer and checks if it
* falls within the valid range for a uint16_t (0 to 65535). If the value is
* within the range, it is cast to uint16_t and returned; otherwise, 0 is returned.
*
* @param str The String to convert to uint16_t.
* @return The converted uint16_t value or 0 if the conversion is out of range.
*/
uint16_t WiFiConfig::stringToUint16(String str) {
  // Convert the String to an integer.
  int intValue = str.toInt();

  // Check if the converted value is within the valid range for uint16_t.
  if (intValue >= 0 && intValue <= UINT16_MAX) {
    // Cast the integer value to uint16_t and return it.
    return static_cast<uint16_t>(intValue);
  } else {
    // Return 0 if the conversion is out of range.
    return 0;
  }
}