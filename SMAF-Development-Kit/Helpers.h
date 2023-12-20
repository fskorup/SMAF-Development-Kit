/**
* @file Helpers.h
* @brief Declaration of helper functions for Arduino project.
*
* This file contains the declarations of helper functions used in the Arduino project.
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

#ifndef HELPERS_H
#define HELPERS_H

#include "Arduino.h"

/**
* @enum messageTypeEnum
* @brief Enumeration for message types used in the project.
*
* This enumeration defines different message types for logging purposes.
*/
enum MessageTypeEnum : byte {
  LOG,  // Info type. INFO message type displayed.
  ERR,  // Error type. ERROR message type displayed.
  SCS,  // Success type. OK message type displayed.
  CMD   // Command type. CMD message type displayed.
};

extern MessageTypeEnum messageType;  // Declare the variable.

/**
* @brief Debugging function to print messages with different types.
*
* This function prints debug messages to the Serial monitor with a specified message type.
*
* @param messageType The type of the message (CLR, ERR, SCS, CMD).
* @param message The message to be printed.
*/
void debug(MessageTypeEnum messageType, const char *format, ...);

#endif