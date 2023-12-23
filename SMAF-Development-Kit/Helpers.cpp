/**
* @file Helpers.cpp
* @brief Implementation of helper functions for Arduino project.
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
#include "Helpers.h"
#include "esp_task_wdt.h"

// Define the variable for message type.
MessageTypeEnum messageType = LOG;

/**
* @brief Debugging function to print messages with different types.
*
* This function prints debug messages to the Serial monitor with a specified message type.
*
* @param messageType The type of the message (LOG, ERR, SCS, CMD).
* @param format The format string for the message.
* @param ... Additional arguments to be formatted.
*/
void debug(MessageTypeEnum messageType, const char *format, ...) {
  // Set up an empty string to store the message type as a string
  String messageTypeStr;

  // Switch statement to determine the message type string based on the input byte
  switch (messageType) {
    case LOG:
      messageTypeStr = "LOG";  // For LOG, set the message type string to "LOG".
      break;

    case ERR:
      messageTypeStr = "ERROR";  // For ERR, set the message type string to "ERROR".
      break;

    case SCS:
      messageTypeStr = "OK";  // For SCS, set the message type string to "OK".
      break;

    case CMD:
      messageTypeStr = "CMD";  // For CMD, set the message type string to "CMD".
      break;
  }

  // Format the variable arguments directly.
  va_list args;
  va_start(args, format);
  char buffer[256];  // Adjust the buffer size according to your needs.
  vsnprintf(buffer, sizeof(buffer), format, args);
  va_end(args);

  // Print the formatted debug message to the Serial monitor.
  Serial.printf("CORE-%02d | %5s | %s\n\r", xPortGetCoreID(), messageTypeStr.c_str(), buffer);
}

/**
* @brief Initializes the ESP32 Watchdog Timer with specified timeout and panic behavior.
*
* This function initializes the ESP32 Watchdog Timer with the provided timeout duration
* and panic behavior. The Watchdog Timer is used to monitor the execution of the program
* and reset the ESP32 in case of a thread or system failure.
*
* @param timeout The timeout duration for the Watchdog Timer in milliseconds.
* @param panic If set to true, enables panic behavior, causing the ESP32 to restart
*              when a timeout occurs. If set to false, the Watchdog Timer triggers a
*              reset without causing a restart.
*/
void initWatchdog(uint32_t timeout, bool panic) {
  esp_task_wdt_init(timeout, panic);
  esp_task_wdt_add(NULL);  // Add current thread to WDT watch.

  // Log the status in the terminal.
  debug(LOG, "Watchdog timer intialized.");
}

/**
* @brief Resets the ESP32 Watchdog Timer.
*
* This function resets the ESP32 Watchdog Timer, preventing it from triggering a
* reset due to a timeout. It is typically used to indicate that the program is
* still executing as expected and to avoid unintended system resets.
*
* @note It is recommended to reset the Watchdog Timer periodically within the
*       program to prevent it from timing out and triggering a system reset.
*/
void resetWatchdog() {
  // Reset WDT.
  esp_task_wdt_reset();

  // Log the status in the terminal.
  debug(LOG, "Watchdog reset.");
}

/**
* @brief Suspends the ESP32 Watchdog Timer.
*
* This function disables the ESP32 Watchdog Timer, temporarily suspending its monitoring
* of the program execution. It is useful in scenarios where certain operations or tasks
* may take longer than the Watchdog Timer's timeout duration and need to be exempt from
* triggering a watchdog reset.
*
* @note It is important to resume the Watchdog Timer using resumeWatchdog() after
*       completing the critical operations to avoid unintended system resets.
*/
void suspendWatchdog() {
  // Disable WDT.
  esp_task_wdt_delete(NULL);

  // Log the status in the terminal.
  debug(LOG, "Watchdog suspended.");
}

/**
* @brief Adds a leading zero to a single-digit integer, converting it to a string.
*
* This function takes an integer value and converts it to a string. If the string
* representation has only one digit, a leading zero is added. Otherwise, the original
* string representation is returned.
*
* @param value The integer value to be converted.
* @return A string representing the integer with a leading zero if it is a single digit.
*/
String addLeadingZero(int value) {
  return (String(value).length() == 1) ? "0" + String(value) : String(value);
}