/**
* @file DeviceStatusVisualizer.h
* @brief Declaration of the DeviceStatusVisualizer library for RGB LED status indication.
*
* This file contains the declarations for the DeviceStatusVisualizer library, which facilitates
* the indication of device status through an RGB LED. The library provides separate functions
* to control the LED for displaying status in terms of red, green, and blue colors. It is designed
* to be easily integrated into Arduino projects for visualizing various device states.
*
* @note This library assumes the presence of an RGB LED with common anode or common cathode
* configuration, and the specific pin connections can be configured in the setup function.
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

#ifndef DEVICESTATUSVISUALIZER_H
#define DEVICESTATUSVISUALIZER_H

#include "Arduino.h"

class DeviceStatusVisualizer {
public:
  /**
  * @brief Constructor for DeviceStatusVisualizer class.
  *
  * Initializes an instance of the DeviceStatusVisualizer class with the provided pin configurations.
  * The RGB LED pins should be set up as OUTPUT before calling this constructor.
  *
  * @param redPin The pin connected to the red channel of the RGB LED.
  * @param greenPin The pin connected to the green channel of the RGB LED.
  * @param bluePin The pin connected to the blue channel of the RGB LED.
  */
  DeviceStatusVisualizer(int redPin, int greenPin, int bluePin);

  /**
  * @brief Blink the RGB LED with a red color.
  *
  * This method creates a blinking effect with a red color on the RGB LED.
  *
  * @param interval The duration of each on and off state of the blinking, in milliseconds.
  */
  void blinkRed(uint32_t interval);

  /**
  * @brief Blink the RGB LED with a green color.
  *
  * This method creates a blinking effect with a green color on the RGB LED.
  *
  * @param interval The duration of each on and off state of the blinking, in milliseconds.
  */
  void blinkGreen(uint32_t interval);

  /**
  * @brief Blink the RGB LED with a blue color.
  *
  * This method creates a blinking effect with a blue color on the RGB LED.
  *
  * @param interval The duration of each on and off state of the blinking, in milliseconds.
  */
  void blinkBlue(uint32_t interval);

  /**
  * @brief Blink the RGB LED with a purple color.
  *
  * This method creates a blinking effect with a purple color on the RGB LED by
  * turning on the red and blue channels simultaneously.
  *
  * @param interval The duration of each on and off state of the blinking, in milliseconds.
  */
  void blinkPurple(uint32_t interval);

  /**
  * @brief Create a burst effect with the red color on the RGB LED.
  *
  * This method generates a burst effect by rapidly turning on and off the red
  * channel of the RGB LED for the specified number of bursts.
  *
  * @param interval The duration of each on and off state within a burst, in milliseconds.
  * @param delayBeforeNextBurst The delay between the end of one burst and the start of the next, in milliseconds.
  * @param blinkCount The number of blinks in one burst.
  */
  void burstRed(uint32_t interval, uint32_t delayBeforeNextBurst, int blinkCount);

  /**
  * @brief Create a burst effect with the green color on the RGB LED.
  *
  * This method generates a burst effect by rapidly turning on and off the green
  * channel of the RGB LED for the specified number of bursts.
  *
  * @param interval The duration of each on and off state within a burst, in milliseconds.
  * @param delayBeforeNextBurst The delay between the end of one burst and the start of the next, in milliseconds.
  * @param blinkCount The number of blinks in one burst.
  */
  void burstGreen(uint32_t interval, uint32_t delayBeforeNextBurst, int blinkCount);

  /**
  * @brief Create a burst effect with the blue color on the RGB LED.
  *
  * This method generates a burst effect by rapidly turning on and off the blue
  * channel of the RGB LED for the specified number of bursts.
  *
  * @param interval The duration of each on and off state within a burst, in milliseconds.
  * @param delayBeforeNextBurst The delay between the end of one burst and the start of the next, in milliseconds.
  * @param blinkCount The number of blinks in one burst.
  */
  void burstBlue(uint32_t interval, uint32_t delayBeforeNextBurst, int blinkCount);

  /**
  * @brief Create a burst effect with the purple color on the RGB LED.
  *
  * This method generates a burst effect by rapidly turning on and off both the red
  * and blue channels of the RGB LED for the specified number of bursts.
  *
  * @param interval The duration of each on and off state within a burst, in milliseconds.
  * @param delayBeforeNextBurst The delay between the end of one burst and the start of the next, in milliseconds.
  * @param blinkCount The number of blinks in one burst.
  */
  void burstPurple(uint32_t interval, uint32_t delayBeforeNextBurst, int blinkCount);

  /**
  * @brief Turn off all channels of the RGB LED.
  *
  * This method turns off all three channels (red, green, and blue) of the RGB LED.
  */
  void shutOffAll();

private:
  int _redPin;
  int _greenPin;
  int _bluePin;
};

#endif