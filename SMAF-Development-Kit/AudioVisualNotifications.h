/**
* @file AudioVisualNotifications.h
* @brief Declaration of the AudioVisualNotifications library for RGB LED and audio status indication.
*
* This file contains the declaration for the AudioVisualNotifications library, which facilitates
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

#ifndef AUDIO_VISUAL_NOTIFICATIONS_H
#define AUDIO_VISUAL_NOTIFICATIONS_H

#include "Arduino.h"
#include "Adafruit_NeoPixel.h"

// Define piano notes.
#define NOTE_B0 31
#define NOTE_C1 33
#define NOTE_CS1 35
#define NOTE_D1 37
#define NOTE_DS1 39
#define NOTE_E1 41
#define NOTE_F1 44
#define NOTE_FS1 46
#define NOTE_G1 49
#define NOTE_GS1 52
#define NOTE_A1 55
#define NOTE_AS1 58
#define NOTE_B1 62
#define NOTE_C2 65
#define NOTE_CS2 69
#define NOTE_D2 73
#define NOTE_DS2 78
#define NOTE_E2 82
#define NOTE_F2 87
#define NOTE_FS2 93
#define NOTE_G2 98
#define NOTE_GS2 104
#define NOTE_A2 110
#define NOTE_AS2 117
#define NOTE_B2 123
#define NOTE_C3 131
#define NOTE_CS3 139
#define NOTE_D3 147
#define NOTE_DS3 156
#define NOTE_E3 165
#define NOTE_F3 175
#define NOTE_FS3 185
#define NOTE_G3 196
#define NOTE_GS3 208
#define NOTE_A3 220
#define NOTE_AS3 233
#define NOTE_B3 247
#define NOTE_C4 262
#define NOTE_CS4 277
#define NOTE_D4 294
#define NOTE_DS4 311
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_FS4 370
#define NOTE_G4 392
#define NOTE_GS4 415
#define NOTE_A4 440
#define NOTE_AS4 466
#define NOTE_B4 494
#define NOTE_C5 523
#define NOTE_CS5 554
#define NOTE_D5 587
#define NOTE_DS5 622
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_FS5 740
#define NOTE_G5 784
#define NOTE_GS5 831
#define NOTE_A5 880
#define NOTE_AS5 932
#define NOTE_B5 988
#define NOTE_C6 1047
#define NOTE_CS6 1109
#define NOTE_D6 1175
#define NOTE_DS6 1245
#define NOTE_E6 1319
#define NOTE_F6 1397
#define NOTE_FS6 1480
#define NOTE_G6 1568
#define NOTE_GS6 1661
#define NOTE_A6 1760
#define NOTE_AS6 1865
#define NOTE_B6 1976
#define NOTE_C7 2093
#define NOTE_CS7 2217
#define NOTE_D7 2349
#define NOTE_DS7 2489
#define NOTE_E7 2637
#define NOTE_F7 2794
#define NOTE_FS7 2960
#define NOTE_G7 3136
#define NOTE_GS7 3322
#define NOTE_A7 3520
#define NOTE_AS7 3729
#define NOTE_B7 3951
#define NOTE_C8 4186
#define NOTE_CS8 4435
#define NOTE_D8 4699
#define NOTE_DS8 4978

class AudioVisualNotifications {
public:
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
  AudioVisualNotifications(int neoPixelPin, int neoPixelCount, int neoPixelBrightness, int speakerPin);

  /**
  * @brief Initializes the NeoPixel LED strip.
  *
  * This function initializes the NeoPixel LED strip with the specified pin and settings
  * provided during the construction of the SensoryAlert object.
  * It should be called once at the beginning of the program or whenever the NeoPixel strip needs to be re-initialized.
  */
  void initializeVisualNotifications();

  /**
  * @brief Clears all visual notifications.
  *
  * This function turns off all NeoPixels in the LED strip, effectively clearing any previous colors or patterns.
  * It can be used to reset the NeoPixel strip to its default state or turn off any active visual feedback.
  */
  void clearAllVisualNotifications();

  /**
  * @brief Plays an introductory melody.
  *
  * This function plays a melody indicating the start of the device operation.
  * It can be used to provide auditory feedback when the device is powered on or initialized.
  */
  void introAudioNotification();

  /**
  * @brief Plays a maintenance melody.
  *
  * This function plays a melody indicating that the device is in maintenance mode.
  * It can be used to provide auditory feedback when the device is undergoing maintenance or configuration changes.
  */
  void maintenanceAudioNotification();

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
  void notReadyVisualNotification();

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
  void readyToSendVisualNotification();

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
  void waitingGnssFixVisualNotification();

  /**
  * @brief Displays a loading indication.
  *
  * This function visually indicates a loading state by alternating the color of the first two NeoPixels between magenta and black.
  * It can be used to provide visual feedback when the device is performing initialization or loading tasks.
  *
  * @note This function alternates the color of the first two NeoPixels between magenta and black to indicate a loading state,
  *       pausing for a brief interval between color changes.
  */
  void loadingVisualNotification();

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
  void maintenanceVisualNotification();

private:
  int _neoPixelPin;
  int _neoPixelCount;
  int _neoPixelBrightness;
  int _speakerPin;
  Adafruit_NeoPixel _neoPixel;  // Declare neoPixel as a member variable
};

#endif
