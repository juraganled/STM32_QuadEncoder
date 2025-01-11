#include "STM32_QuadEncoder.h"

// Copyright @ 2025 by Ricky Suprayudi https://github.com/juraganled/STM32_QuadEncoder
// 
// Based on https://www.stm32duino.com/viewtopic.php?t=1962
//
// pins must be channel 1 and channel 2 on a timer that support hardware encoder mode
/*
  STM32F411CE:
  +----------+-----------+-----------+
  |          |  CLK      |  DT       |
  +----------+-----------+-----------+
  | TIMER1   |  PA8      |  PA9      |
  +----------+-----------+-----------+
  | TIMER2   |  PA0      |  PA1      |
  |          |  PA5      |  PB3      |
  |          |  PA15     |           |
  +----------+-----------+-----------+
  | TIMER3   |  PA6      |  PA7_ALT1 |
  |          |  PB4      |  PB5      |
  +----------+-----------+-----------+
  | TIMER4   |  PB6      |  PB7      |
  +----------+-----------+-----------+
  | TIMER5   |  PA0_ALT1 |  PA1_ALT1 |
  +----------+-----------+-----------+
*/

const int ROTARY4_A = PB6;
const int ROTARY4_B = PB7;

STM32_QuadEncoder QE4;

unsigned long counter = 0;
unsigned long oldCounter = 0;

// this is the maximum encoder value if rotated fully 360 degree
#define PPR 10000

void alertOverflow() {
  Serial.println("Overflow!");
}

void setup() {
  QE4.begin(ROTARY4_A, ROTARY4_B, NO_PULLUP, PPR, DIRECTION_NORMAL);
  
  delay(1000);
  Serial.begin(115200);
  Serial.print("STM32 QuadEncoder Basic, using Timer ");
  Serial.println(QE4.getTimerNumber());

  QE4.attach(alertOverflow);  // this will be called every time overflow occurs, ie: from 9999 to 0
}

void loop() {
  counter = QE4.getCount();
  
  if (oldCounter != counter) {
    oldCounter = counter;
    Serial.print("Encoder (");
    Serial.print(QE4.getDirection() == IS_COUNTING_UP ? "UP" : "DOWN");
    Serial.print(") = ");

    Serial.println(counter);
  }
}
