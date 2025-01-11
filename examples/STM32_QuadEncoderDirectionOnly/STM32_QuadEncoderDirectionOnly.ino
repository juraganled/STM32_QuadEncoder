#include "STM32_QuadEncoder.h"

// This example demonstrates how to use the STM32_QuadEncoder library to determine the rotation direction
// of a rotary encoder. By setting the Pulses Per Revolution (PPR) to 2, the code calculates the encoder's
// direction of rotation when an overflow event occurs.

// Copyright @ 2025 by Ricky Suprayudi https://github.com/juraganled/STM32_QuadEncoder

// This is an example that by setting PPR value to 2, 
// and when everytime overflow occurs we can find the rotation direction

const int ROTARY4_A = PB6;
const int ROTARY4_B = PB7;

STM32_QuadEncoder QE4; // QuadEncoder object

#define PPR 2 // Pulses Per Revolution for the rotary encoder

// Callback function to handle direction detection when an overflow event occurs
void findDirectionCallback() {
  Serial.println(QE4.getDirection() == IS_COUNTING_UP ? "UP" : "DOWN");
}

void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.print("STM32 QuadEncoder Example, using Timer ");

  // Initialize the rotary encoder with specified parameters
  QE4.begin(ROTARY4_A, ROTARY4_B, NO_PULLUP, PPR, DIRECTION_NORMAL);

  // Print the timer used by the library for this encoder instance
  Serial.println(QE4.getTimerNumber());

  // Attach the direction detection callback function
  QE4.attach(findDirectioCallback);
}

void loop() {
  // Main loop is left empty as the library handles encoder logic and callbacks
}
