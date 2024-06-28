/**
* @file AudioVisualNotifications.h
* @brief Implementation of the AudioVisualNotifications library for RGB LED and audio status indication.
*
* This file contains the implementation for the AudioVisualNotifications library, which facilitates
* the indication of device status through a NeoPixel LED and audio feedback. The library provides separate functions
* to control the LED for displaying status in terms of colors, as well as functions to play various melodies for auditory feedback.
* It is designed to be easily integrated into Arduino projects for visualizing various device states.
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
#include "AudioVisualNotifications.h"
#include "Adafruit_NeoPixel.h"

/**
* @brief Constructs an instance of the AudioVisualNotifications class.
*
* Initializes an instance of the AudioVisualNotifications class with the provided configurations.
* The NeoPixel pin should be set up as OUTPUT before calling this constructor.
*
* @param neoPixelPin The pin connected to the NeoPixel LED strip.
* @param neoPixelCount The number of NeoPixels in the LED strip.
* @param neoPixelBrightness The brightness level of the NeoPixels (0-255).
* @param speakerPin The pin connected to the speaker for audio feedback.
*/
AudioVisualNotifications::AudioVisualNotifications(int neoPixelPin, int neoPixelCount, int neoPixelBrightness, int speakerPin)
  : _neoPixelPin(neoPixelPin),
    _neoPixelCount(neoPixelCount),
    _neoPixelBrightness(neoPixelBrightness),
    _speakerPin(speakerPin),
    _neoPixel(neoPixelCount, neoPixelPin, NEO_GRB + NEO_KHZ800) {
}

/**
* @brief Initializes the NeoPixel LED strip.
*
* This function initializes the NeoPixel LED strip with the specified pin and settings
* provided during the construction of the SensoryAlert object.
* It should be called once at the beginning of the program or whenever the NeoPixel strip needs to be re-initialized.
*/
void AudioVisualNotifications::initializeVisualNotifications() {
  _neoPixel.begin();                             // INITIALIZE NeoPixel strip object (REQUIRED)
  _neoPixel.setBrightness(_neoPixelBrightness);  // Set BRIGHTNESS to about 1/5 (max = 255)
}

/**
* @brief Clears all visual notifications.
*
* This function turns off all NeoPixels in the LED strip, effectively clearing any previous colors or patterns.
* It can be used to reset the NeoPixel strip to its default state or turn off any active visual feedback.
*/
void AudioVisualNotifications::clearAllVisualNotifications() {
  _neoPixel.clear();
  _neoPixel.show();
}

/**
* @brief Plays an introductory melody.
*
* This function plays a melody indicating the start of the device operation.
* It can be used to provide auditory feedback when the device is powered on or initialized.
*/
void AudioVisualNotifications::introAudioNotification() {
  tone(_speakerPin, NOTE_E6);
  delay(120);
  noTone(_speakerPin);

  tone(_speakerPin, NOTE_F6);
  delay(120);
  noTone(_speakerPin);

  tone(_speakerPin, NOTE_G6);
  delay(320);
  noTone(_speakerPin);
}

/**
* @brief Plays a maintenance melody.
*
* This function plays a melody indicating that the device is in maintenance mode.
* It can be used to provide auditory feedback when the device is undergoing maintenance or configuration changes.
*/
void AudioVisualNotifications::maintenanceAudioNotification() {
  // First part.
  tone(_speakerPin, NOTE_E6);
  delay(120);
  noTone(_speakerPin);

  delay(80);

  tone(_speakerPin, NOTE_E6);
  delay(120);
  noTone(_speakerPin);

  delay(80);

  tone(_speakerPin, NOTE_F6);
  delay(120);
  noTone(_speakerPin);

  delay(80);

  tone(_speakerPin, NOTE_G6);
  delay(280);
  noTone(_speakerPin);

  // Second part.
  tone(_speakerPin, NOTE_E6);
  delay(120);
  noTone(_speakerPin);

  tone(_speakerPin, NOTE_F6);
  delay(120);
  noTone(_speakerPin);

  tone(_speakerPin, NOTE_G6);
  delay(320);
  noTone(_speakerPin);
}

/**
* @brief Displays a not-ready indication.
*
* This function visually indicates that the device is not yet ready for operation
* by alternating the color of the first two NeoPixels between red and black.
* It can be used as part of the device initialization process or when certain conditions are not met for operation.
*
* @note This function alternates the color of the first two NeoPixels between red and black to indicate a not-ready state,
*       pausing for a brief interval between color changes.
*/
void AudioVisualNotifications::notReadyVisualNotification() {
  uint32_t interval = 240;

  _neoPixel.setPixelColor(0, _neoPixel.Color(255, 0, 0));
  _neoPixel.setPixelColor(1, _neoPixel.Color(0, 0, 0));
  _neoPixel.show();

  delay(interval);

  _neoPixel.setPixelColor(0, _neoPixel.Color(0, 0, 0));
  _neoPixel.setPixelColor(1, _neoPixel.Color(255, 0, 0));
  _neoPixel.show();

  // clearNeoPixel();
  delay(interval);
}

/**
* @brief Displays a ready-to-send indication.
*
* This function visually indicates that the device is ready to send data or perform its main function
* by blinking the first two NeoPixels in green color for a specified number of times in bursts.
* It can be used to indicate that the device has completed its initialization process and is ready for operation.
*
* @param delayBeforeNextBurst The delay (in milliseconds) before starting the next burst of blinks.
* @param blinkCount The number of times the NeoPixels should blink in one burst.
*/
void AudioVisualNotifications::readyToSendVisualNotification() {
  uint32_t delayBeforeNextBurst = 1200;
  int blinkCount = 4;

  // Loop through the specified number of blinks in one burst.
  for (int i = 0; i < blinkCount; ++i) {
    _neoPixel.setPixelColor(0, _neoPixel.Color(0, 255, 0));
    _neoPixel.setPixelColor(1, _neoPixel.Color(0, 255, 0));
    _neoPixel.show();

    delay(40);
    clearAllVisualNotifications();
    delay(40);
  }

  // Add a delay before starting the next burst.
  delay(delayBeforeNextBurst);
}

/**
* @brief Displays a waiting for GNSS fix indication.
*
* This function visually indicates that the device is waiting to acquire a GNSS (Global Navigation Satellite System) fix
* by alternating the color of the first two NeoPixels between blue and black.
* It can be used in applications where GNSS data is required for operation and the device is waiting for a valid signal.
*
* @note This function alternates the color of the first two NeoPixels between blue and black to indicate a waiting state for a GNSS fix,
*       pausing for a brief interval between color changes.
*/
void AudioVisualNotifications::waitingGnssFixVisualNotification() {
  uint32_t interval = 240;

  _neoPixel.setPixelColor(0, _neoPixel.Color(0, 0, 255));
  _neoPixel.setPixelColor(1, _neoPixel.Color(0, 0, 0));
  _neoPixel.show();

  delay(interval);

  _neoPixel.setPixelColor(0, _neoPixel.Color(0, 0, 0));
  _neoPixel.setPixelColor(1, _neoPixel.Color(0, 0, 255));
  _neoPixel.show();

  // clearNeoPixel();
  delay(interval);
}

/**
* @brief Displays a loading indication.
*
* This function visually indicates a loading state by alternating the color of the first two NeoPixels between magenta and black.
* It can be used to provide visual feedback when the device is performing initialization or loading tasks.
*
* @note This function alternates the color of the first two NeoPixels between magenta and black to indicate a loading state,
*       pausing for a brief interval between color changes.
*/
void AudioVisualNotifications::loadingVisualNotification() {
  uint32_t interval = 240;

  _neoPixel.setPixelColor(0, _neoPixel.Color(255, 0, 255));
  _neoPixel.setPixelColor(1, _neoPixel.Color(0, 0, 0));
  _neoPixel.show();

  delay(interval);

  _neoPixel.setPixelColor(0, _neoPixel.Color(0, 0, 0));
  _neoPixel.setPixelColor(1, _neoPixel.Color(255, 0, 255));
  _neoPixel.show();

  // clearNeoPixel();
  delay(interval);
}

/**
* @brief Displays a maintenance indication.
*
* This function visually indicates maintenance mode by illuminating the first two NeoPixels with magenta color.
* It can be used to temporarily signal that the device is undergoing maintenance or configuration changes,
* without actually setting the device to a specific mode.
*
* @note This function illuminates the first two NeoPixels with magenta color to indicate maintenance mode,
*       pauses for a brief interval, then clears the NeoPixel strip.
*/
void AudioVisualNotifications::maintenanceVisualNotification() {
  uint32_t interval = 240;

  _neoPixel.setPixelColor(0, _neoPixel.Color(255, 0, 255));
  _neoPixel.setPixelColor(1, _neoPixel.Color(255, 0, 255));
  _neoPixel.show();

  delay(interval);
  clearAllVisualNotifications();
  delay(interval);
}