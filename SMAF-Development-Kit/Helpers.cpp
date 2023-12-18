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

MessageTypeEnum messageType = LOG;  // Define the variable.

/**
* @brief Debugging function to print messages with different types.
*
* This function prints debug messages to the Serial monitor with a specified message type.
*
* @param messageType The type of the message (CLR, ERR, SCS, CMD).
* @param message The message to be printed.
*/
void debug(byte messageType, String message) {
  // Set up an empty string to store the message type as a string
  String messageTypeStr = String();

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

  // Print the formatted debug message to the Serial monitor
  Serial.printf("CORE-%02d | %5s | %s\n\r", xPortGetCoreID(), messageTypeStr.c_str(), message.c_str());
}