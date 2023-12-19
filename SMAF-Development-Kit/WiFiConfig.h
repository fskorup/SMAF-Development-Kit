/**
* @file WiFiConfig.h
* @brief Declaration of WiFiConfig functions for SoftAP configurator.
*
* This file contains the declarations of helper functions used in the Arduino project
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

#ifndef WIFICONFIG_H
#define WIFICONFIG_H

#include "Arduino.h"
#include "WiFi.h"
#include "WiFiServer.h"
#include "Preferences.h"
#include "Helpers.h"

// Define constant strings for Wi-Fi network configuration
#define NETWORK_NAME "netName"  // Wi-Fi network name
#define NETWORK_PASS "netPass"  // Wi-Fi network password

// Define constant strings for MQTT configuration
#define MQTT_SERVER_ADDRESS "mqttSrvAdr"  // MQTT server address
#define MQTT_SERVER_PORT "mqttSrvPort"    // MQTT server port
#define MQTT_USERNAME "mqttUser"          // MQTT username
#define MQTT_PASS "mqttPass"              // MQTT password
#define MQTT_CLIENT_ID "mqttClient"       // MQTT client ID
#define MQTT_TOPIC "mqttTopic"            // MQTT topic

class WiFiConfig {
public:
  /**
  * @brief Constructor for WiFiConfig class.
  *
  * Initializes an instance of the WiFiConfig class with the provided configuration parameters.
  *
  * @param configNetworkName The Wi-Fi network name for configuration.
  * @param configNetworkPass The Wi-Fi network password for configuration.
  * @param configServerPort The port for the SoftAP configuration server.
  */
  WiFiConfig(String configNetworkName, String configNetworkPass, uint16_t configServerPort);

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
  void startConfig();

  /**
  * @brief Get the configured network name for SoftAP.
  * 
  * @return String representing the configured network name.
  *         If empty, returns "NULL".
  */
  String getConfigNetworkName();

  /**
  * @brief Get the configured network password for SoftAP.
  * 
  * @return String representing the configured network password.
  *         If empty, returns "NULL".
  */
  String getConfigNetworkPass();

  /**
  * @brief Get the IP address of the SoftAP.
  * 
  * @return String representing the IP address of the SoftAP.
  *         If empty, returns "NULL".
  */
  String getConfigServerIP();

  /**
  * @brief Get the configured configuration server port.
  * 
  * @return uint16_t representing the configuration server port.
  */
  uint16_t getConfigServerPort();

  /**
  * @brief Render the configuration page for device setup.
  * 
  * This function serves an HTML configuration page to the connected client.
  * It processes the form submission and saves the configuration settings.
  * 
  * @note The HTML structure and styling are included for presentation purposes.
  */
  void renderConfigPage();

  /**
  * @brief Set the preferences namespace.
  *
  * This method sets the preferences namespace, allowing for a customized namespace
  * when saving and loading preferences related to Wi-Fi and MQTT configuration.
  *
  * @param name The preferences namespace name to be set.
  */
  void setPreferencesNamespace(String name);

  /**
  * @brief Load Wi-Fi and MQTT configuration preferences.
  *
  * This method reads configuration parameters from non-volatile storage using the
  * Preferences library. It loads Wi-Fi network name, password, MQTT server address,
  * port, username, password, client ID, and topic. After loading, it checks if all
  * essential configuration parameters are non-empty and valid to determine the
  * overall configuration validity.
  */
  void loadPreferences();

  /**
  * @brief Save Wi-Fi and MQTT configuration preferences.
  *
  * This method stores the current configuration parameters to non-volatile storage
  * using the Preferences library. It saves Wi-Fi network name, password, MQTT server
  * address, port, username, password, client ID, and topic for future use.
  *
  * @note The Preferences library is used to store and retrieve non-volatile data.
  *       Uncommenting the ESP.restart() line will restart the device after saving preferences,
  *       useful if a restart is desired after configuration changes.
  */
  void savePreferences();

  /**
  * @brief Clear all preferences within a specific namespace.
  *
  * This function clears all preferences stored within the specified namespace,
  * effectively resetting them to their default values.
  *
  * @param name The namespace for which preferences should be cleared.
  */
  void clearPreferencesInNamespace(String name);

  /**
  * @brief Check if the Wi-Fi configuration is valid.
  *
  * @return True if the configuration is valid, false otherwise.
  */
  bool isConfigValid();

  /**
  * @brief Get the configured preferences namespace.
  * 
  * @return String representing the configured preferences namespace.
  *         If empty, returns "NULL".
  */
  String getPreferencesNamespace();

  /**
  * @brief Get the configured Wi-Fi network name.
  * 
  * @return String representing the Wi-Fi network name.
  *         If empty, returns "NULL".
  */
  String getNetworkName();

  /**
  * @brief Get the configured Wi-Fi network password.
  * 
  * @return String representing the Wi-Fi network password.
  *         If empty, returns "NULL".
  */
  String getNetworkPass();

  /**
  * @brief Get the configured MQTT server address.
  * 
  * @return String representing the MQTT server address.
  *         If empty, returns "NULL".
  */
  String getMqttServerAddress();

  /**
  * @brief Get the configured MQTT username.
  * 
  * @return String representing the MQTT username.
  *         If empty, returns "NULL".
  */
  String getMqttUsername();

  /**
  * @brief Get the configured MQTT password.
  * 
  * @return String representing the MQTT password.
  *         If empty, returns "NULL".
  */
  String getMqttPass();

  /**
  * @brief Get the configured MQTT client ID.
  * 
  * @return String representing the MQTT client ID.
  *         If empty, returns "NULL".
  */
  String getMqttClientId();

  /**
  * @brief Get the configured MQTT topic.
  * 
  * @return String representing the MQTT topic.
  *         If empty, returns "NULL".
  */
  String getMqttTopic();

  /**
  * @brief Get the MQTT server port.
  *
  * @return The MQTT server port.
  */
  uint16_t getMqttServerPort();

private:
  // Server instance for handling SoftAP configuration.
  WiFiServer _configServerInstance;

  // SoftAP SSID name, password, port and IP.
  String _configNetworkName;       // Name of the SoftAP (Access Point).
  String _configNetworkPass;       // Password for the SoftAP.
  String _configServerIP;          // IP of the SoftAP.
  uint16_t _configServerPort = 0;  // Port for the SoftAP.

  // Preferences namespace.
  String _preferencesNamespace;

  // Network and MQTT configuration parameters.
  String _networkName;           // Wi-Fi network name.
  String _networkPass;           // Wi-Fi network password.
  String _mqttServerAddress;     // MQTT server address.
  String _mqttUsername;          // MQTT username.
  String _mqttPass;              // MQTT password.
  String _mqttClientId;          // MQTT client ID.
  String _mqttTopic;             // MQTT topic.
  uint16_t _mqttServerPort = 0;  // MQTT server port.

  // Flag indicating whether the configuration is valid.
  bool _isConfigValid;

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
  String parseFieldValue(String data, String fieldId);

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
  String decodeResponse(String input);

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
  String removeSpaces(String str);

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
  byte hexToByte(char c);

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
  uint16_t stringToUint16(String str);
};

#endif