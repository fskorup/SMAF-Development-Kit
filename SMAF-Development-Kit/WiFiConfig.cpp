/**
* @file WiFiConfig.cpp
* @brief Implementation of WiFiConfig functions for SoftAP configurator.
*
* This file contains the implementation of helper functions used in the Arduino project.
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
*/
WiFiConfig::WiFiConfig(String configNetworkName, String configNetworkPass, uint16_t configServerPort)
  : _configNetworkName(configNetworkName),
    _configNetworkPass(configNetworkPass),
    _configServerPort(configServerPort),
    _configServerInstance(80) {
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
* before calling this method.
*/
void WiFiConfig::startConfig() {
  // Start SoftAP with the specified network name and password.
  WiFi.softAP(_configNetworkName.c_str(), _configNetworkPass.c_str());

  // Introduce a delay for SoftAP initialization.
  delay(800);

  // Begin the configuration server instance.
  _configServerInstance.begin();
}

/**
* @brief Render the configuration page for device setup.
* 
* This function serves an HTML configuration page to the connected client.
* It processes the form submission and saves the configuration settings.
* 
* @note The HTML structure and styling are included for presentation purposes.
*/
void WiFiConfig::renderConfigPage() {
  // Check if a client has connected
  WiFiClient client = _configServerInstance.available();

  if (!client) {
    return;  // No client, exit the loop
  }

  // Wait until the client sends some data
  while (!client.available()) {
    delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  client.flush();

  // Serve the HTML page
  String html = String();
  html = "<!DOCTYPE html>";
  html += "<html lang='en'>";
  html += "<head>";
  html += "<meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0, user-scalable=no'>";
  html += "<style>";
  html += "* { font-family: system-ui, sans-serif; font-size: 14px; line-height: 1.5; color: #202326; margin: 0; padding: 0; box-sizing: border-box; outline: none; list-style: none; word-wrap: anywhere; }";
  html += ".no-margin { margin: 0; padding: 0; }";
  html += "body { display: flex; flex-direction: column; flex-wrap: nowrap; align-items: center; }";
  html += "header, section, .frame-primary, .frame-secondary, form { display: flex; flex-direction: column; gap: 20px; }";
  html += "header { padding-bottom: 8px; }";
  html += ".frame-secondary { gap: 4px; }";
  html += ".frame-horizontal { gap: 20px; display: flex; flex-direction: row; justify-content: space-between; flex-wrap: wrap; }";
  html += "form { margin: 40px 24px 120px; max-width: 440px; }";
  html += "h1, h2, h3, h4, h5 { color: inherit; line-height: 1.15; }";
  html += "h1 { font-size: 2.074rem; font-weight: 700; }";
  html += "h2 { font-size: 1.44rem; font-weight: 630; margin-top: 28px; }";
  html += "p, span, label, input[type='text'], ul, li { font-size: 1rem; line-height: 1.5; color: inherit; }";
  html += "span { font-weight: 550; }";
  html += "input[type='text'] { font-family: monospace, sans-serif; padding: 12px; border: none; box-shadow: 0 0 0 1px #D7DFE8; border-radius: 0px; }";
  html += "input[type='text']:focus { box-shadow: 0 0 0 2px #0180FF; }";
  html += "input[type='submit'] { border: none; padding: 12px 24px; background: #00CC22; border-radius: 0px; font-weight: 550; color: #FFFFFF; cursor: pointer; font-size: 1.05rem; line-height: 1.5; flex-grow: 2; }";
  html += "input[type='reset'] { border: 1px solid #D7DFE8; padding: 12px 24px; background: none; border-radius: 0px; font-weight: 550; color: inherit; cursor: pointer; font-size: 1.05rem; line-height: 1.5; flex-grow: 1; }";
  html += "section { border-left: 3px solid #D7DFE8; padding: 16px 20px; }";
  html += "section.success { border-color: #00CC22; background: #F2FFF4; color: #004D0D; }";
  html += "section.info { border-color: #0180FF; background: #F2F9FF; color: #003366; }";
  html += "</style>";
  html += "</head>";
  html += "<html>";
  html += "<body>";
  html += "<form action='/configuration' method='get'>";
  html += "<header>";
  html += "<h1 class='no-margin'>🤙</h1>";
  html += "<h1>Device<br>configuration</h1>";
  html += "<p>Ribeye biltong salami, rump ham hock tail turducken meatball short loin meatloaf buffalo shank. Andouille venison pork chop chicken jowl kevin.</p>";
  html += "</header>";

  // Check if the request is a form submission
  if (request.indexOf("/configuration") != -1) {
    _networkName = parseFieldValue(request, NETWORK_NAME);
    _networkPass = parseFieldValue(request, NETWORK_PASS);
    _mqttServerAddress = parseFieldValue(request, MQTT_SERVER_ADDRESS);
    _mqttServerPort = stringToUint16(parseFieldValue(request, MQTT_SERVER_PORT));
    _mqttUsername = parseFieldValue(request, MQTT_USERNAME);
    _mqttPass = parseFieldValue(request, MQTT_PASS);
    _mqttClientId = parseFieldValue(request, MQTT_CLIENT_ID);
    _mqttTopic = parseFieldValue(request, MQTT_TOPIC);

    // Save preferences.
    savePreferences();

    // Display a success message with the saved configuration
    html += "<section class='success'>";
    html += "<p>Configuration successfully saved to device. Data saved in device memory is shown bellow.</p>";
    html += "<ul>";
    html += "<li><span>SSID Name: </span>" + _networkName + "</li>";
    html += "<li><span>SSID Password: </span>" + _networkPass + "</li>";
    html += "<li><span>MQTT Server: </span>" + _mqttServerAddress + "</li>";
    html += "<li><span>MQTT Port: </span>" + String(_mqttServerPort) + "</li>";
    html += "<li><span>MQTT Username: </span>" + _mqttUsername + "</li>";
    html += "<li><span>MQTT Password: </span>" + _mqttPass + "</li>";
    html += "<li><span>MQTT Client ID: </span>" + _mqttClientId + "</li>";
    html += "<li><span>MQTT Topic: </span>" + _mqttTopic + "</li>";
    html += "</ul>";
    html += "<p>Device will now reboot and try to connect to the configured SSID and connection with this page will be lost.</p>";
    html += "</section>";
    html += "<section class='info'>";
    html += "<p>To start the configuration again, restart the device while holding the quick config button on the development board. Keep holding the quick config button until the purple LED is lit.</p>";
    html += "</section>";
  }

  // ... HTML form for user input ...
  html += "<h2>WiFi router<br>configuration</h2>";
  html += "<p>Establish and customize the wireless network name (SSID) and configure the Access Point for a secure and reliable connection.</p>";
  html += "<div class='frame-primary'>";
  html += "<div class='frame-secondary'><label for='" + String(NETWORK_NAME) + "'>SSID Name:</label><input id='" + String(NETWORK_NAME) + "' type='text' name='" + String(NETWORK_NAME) + "' value='" + _networkName + "'></div>";
  html += "<div class='frame-secondary'><label for='" + String(NETWORK_PASS) + "'>SSID Password:</label><input id='" + String(NETWORK_PASS) + "' type='text' name='" + String(NETWORK_PASS) + "' value='" + _networkPass + "'></div>";
  html += "</div>";
  html += "<h2>MQTT server<br>configuration</h2>";
  html += "<p>Configure MQTT protocol settings, including the broker's address, port, and authentication details, to enable effective device communication.</p>";
  html += "<div class='frame-primary'>";
  html += "<div class='frame-secondary'><label for='" + String(MQTT_SERVER_ADDRESS) + "'>MQTT Server:</label><input id='" + String(MQTT_SERVER_ADDRESS) + "' type='text' name='" + String(MQTT_SERVER_ADDRESS) + "' value='" + _mqttServerAddress + "'></div>";
  html += "<div class='frame-secondary'><label for='" + String(MQTT_SERVER_PORT) + "'>MQTT Port:</label><input id='" + String(MQTT_SERVER_PORT) + "' type='text' name='" + String(MQTT_SERVER_PORT) + "' value='" + String(_mqttServerPort) + "'></div>";
  html += "<div class='frame-secondary'><label for='" + String(MQTT_USERNAME) + "'>MQTT Username:</label><input id='" + String(MQTT_USERNAME) + "' type='text' name='" + String(MQTT_USERNAME) + "' value='" + _mqttUsername + "'></div>";
  html += "<div class='frame-secondary'><label for='" + String(MQTT_PASS) + "'>MQTT Password:</label><input id='" + String(MQTT_PASS) + "' type='text' name='" + String(MQTT_PASS) + "' value='" + _mqttPass + "'></div>";
  html += "</div>";
  html += "<h2>MQTT client & topic<br>configuration</h2>";
  html += "<p>Define MQTT topic for streamlined message exchange and assign unique client IDs to device, ensuring precise and targeted communication within the MQTT network.</p>";
  html += "<div class='frame-primary'>";
  html += "<div class='frame-secondary'><label for='" + String(MQTT_CLIENT_ID) + "'>MQTT Client ID:</label><input id='" + String(MQTT_CLIENT_ID) + "' type='text' name='" + String(MQTT_CLIENT_ID) + "' value='" + _mqttClientId + "'></div>";
  html += "<div class='frame-secondary'><label for='" + String(MQTT_TOPIC) + "'>MQTT Topic:</label><input id='" + String(MQTT_TOPIC) + "' type='text' name='" + String(MQTT_TOPIC) + "' value='" + _mqttTopic + "'></div>";
  html += "</div>";
  html += "<h2>Finish<br>configuration</h2>";
  html += "<p>Upon pressing the \"Upload configuration\" button, the device will reset to apply the changes, temporarily disrupting the connection. This ensures a swift and efficient update of settings.</p>";
  html += "<section class='info'><p>Fields are not mandatory, but please double-check if all data is entered. Upon uploading the new configuration, the device will not start if any essential data is missing. Ensure all required information is provided before proceeding.</p></section>";
  html += "<div class='frame-horizontal'>";
  html += "<input type='reset' value='Reset form'>";
  html += "<input type='submit' value='Upload configuration'>";
  html += "</div>";
  html += "</form>";
  html += "</body>";
  html += "</html>";
  //*/

  // Send the response to the client
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();
  client.println(html);
}

/**
* @brief Set the preferences namespace.
*
* This method sets the preferences namespace, allowing for a customized namespace
* when saving and loading preferences related to Wi-Fi and MQTT configuration.
*
* @param name The preferences namespace name to be set.
*/
void WiFiConfig::setPreferencesNamespace(String name) {
  _preferencesNamespace = name;
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
void WiFiConfig::loadPreferences() {
  // Create a Preferences instance with the specified namespace.
  Preferences preferences;

  // Begin preferences with the specified namespace.
  preferences.begin(_preferencesNamespace.c_str(), false);

  // Load Wi-Fi network configuration.
  _networkName = preferences.getString(NETWORK_NAME, String());
  _networkPass = preferences.getString(NETWORK_PASS, String());

  // Load MQTT configuration.
  _mqttServerAddress = preferences.getString(MQTT_SERVER_ADDRESS, String());
  _mqttServerPort = preferences.getUShort(MQTT_SERVER_PORT, 0);
  _mqttUsername = preferences.getString(MQTT_USERNAME, String());
  _mqttPass = preferences.getString(MQTT_PASS, String());
  _mqttClientId = preferences.getString(MQTT_CLIENT_ID, String());
  _mqttTopic = preferences.getString(MQTT_TOPIC, String());

  // End preferences session.
  preferences.end();

  // Check if all essential configuration parameters are non-empty.
  if (_networkName.isEmpty() || _networkPass.isEmpty() || _mqttServerAddress.isEmpty() || _mqttServerPort == 0 || _mqttUsername.isEmpty() || _mqttPass.isEmpty() || _mqttClientId.isEmpty() || _mqttTopic.isEmpty()) {
    _isConfigValid = false;
  } else {
    _isConfigValid = true;
  }
}

/**
* @brief Save Wi-Fi and MQTT configuration preferences.
*
* This method stores the current configuration parameters to non-volatile storage
* using the Preferences library. It saves Wi-Fi network name, password, MQTT server
* address, port, username, password, client ID, and topic for future use.
*
* @note The Preferences library is used to store and retrieve non-volatile data.
* Uncommenting the ESP.restart() line will restart the device after saving preferences,
* useful if a restart is desired after configuration changes.
*/
void WiFiConfig::savePreferences() {
  // Create a Preferences instance with the specified namespace.
  Preferences preferences;

  // Begin preferences with the specified namespace.
  preferences.begin(_preferencesNamespace.c_str(), false);

  // Save Wi-Fi network configuration.
  preferences.putString(NETWORK_NAME, _networkName);
  preferences.putString(NETWORK_PASS, _networkPass);

  // Save MQTT configuration.
  preferences.putString(MQTT_SERVER_ADDRESS, _mqttServerAddress);
  preferences.putUShort(MQTT_SERVER_PORT, _mqttServerPort);
  preferences.putString(MQTT_USERNAME, _mqttUsername);
  preferences.putString(MQTT_PASS, _mqttPass);
  preferences.putString(MQTT_CLIENT_ID, _mqttClientId);
  preferences.putString(MQTT_TOPIC, _mqttTopic);

  // End preferences session.
  preferences.end();

  // Uncomment the following line if a device restart is desired after saving preferences.
  // ESP.restart();
}

/**
* @brief Clear all preferences within a specific namespace.
*
* This function clears all preferences stored within the specified namespace,
* effectively resetting them to their default values.
*
* @param name The namespace for which preferences should be cleared.
*/
void WiFiConfig::clearPreferencesInNamespace(String name) {
  // Create a Preferences instance with the specified namespace.
  Preferences preferences;

  // Begin preferences with the specified namespace.
  preferences.begin(name.c_str(), false);

  // Clear all preferences in the specified namespace.
  preferences.clear();

  // End preferences.
  preferences.end();
}

/**
* @brief Check if the Wi-Fi configuration is valid.
*
* @return True if the configuration is valid, false otherwise.
*
* @see loadPreferences()
*/
bool WiFiConfig::isConfigValid() {
  return _isConfigValid;
}

/**
* @brief Get the preferences namespace name.
*
* @return The preferences namespace name.
*/
String WiFiConfig::getPreferencesNamespace() {
  return _preferencesNamespace.isEmpty() ? "NULL" : _preferencesNamespace;
}

/**
* @brief Get the Wi-Fi network name.
*
* @return The Wi-Fi network name.
*/
String WiFiConfig::getNetworkName() {
  return _networkName.isEmpty() ? "NULL" : _networkName;
}

/**
* @brief Get the Wi-Fi network password.
*
* @return The Wi-Fi network password.
*/
String WiFiConfig::getNetworkPass() {
  return _networkPass.isEmpty() ? "NULL" : _networkPass;
}

/**
* @brief Get the MQTT server address.
*
* @return The MQTT server address.
*/
String WiFiConfig::getMqttServerAddress() {
  return _mqttServerAddress.isEmpty() ? "NULL" : _mqttServerAddress;
}

/**
* @brief Get the MQTT username.
*
* @return The MQTT username.
*/
String WiFiConfig::getMqttUsername() {
  return _mqttUsername.isEmpty() ? "NULL" : _mqttUsername;
}

/**
* @brief Get the MQTT password.
*
* @return The MQTT password.
*/
String WiFiConfig::getMqttPass() {
  return _mqttPass.isEmpty() ? "NULL" : _mqttPass;
}

/**
* @brief Get the MQTT client ID.
*
* @return The MQTT client ID.
*/
String WiFiConfig::getMqttClientId() {
  return _mqttClientId.isEmpty() ? "NULL" : _mqttClientId;
}

/**
* @brief Get the MQTT topic.
*
* @return The MQTT topic.
*/
String WiFiConfig::getMqttTopic() {
  return _mqttTopic.isEmpty() ? "NULL" : _mqttTopic;
}

/**
* @brief Get the MQTT server port.
*
* @return The MQTT server port.
*/
uint16_t WiFiConfig::getMqttServerPort() {
  return _mqttServerPort;
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
  int index = data.indexOf(fieldId) + fieldId.length() + 1;
  String value = String();

  // Find the indices of the next ampersand (&) and " HTTP" in the data String.
  int ampIndex = data.indexOf("&", index);
  int httpIndex = data.indexOf(" HTTP", index);

  // Determine the end index based on the minimum of ampIndex and httpIndex, or the end of the data String.
  int endIndex = min(static_cast<int>(ampIndex != -1 ? ampIndex : httpIndex), static_cast<int>(data.length()));

  // Extract the value substring based on the determined indices.
  value = data.substring(index, endIndex);

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
