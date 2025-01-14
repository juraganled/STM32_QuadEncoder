# STM32_QuadEncoder Library

STM32_QuadEncoder is an Arduino-compatible library for STM32 microcontrollers that simplifies the handling of quadrature encoders using the hardware timers built into STM32 boards. By offloading encoder logic to hardware timers, the library frees up CPU resources, making it ideal for real-time applications.

## Features

- Uses STM32 hardware timers for efficient encoder signal processing.
- Supports configurable pulses per revolution (PPR).
- Detects rotation direction (clockwise or counterclockwise).
- Provides callbacks for overflow events.
- Lightweight and easy to integrate.

## Installation

1. Download the library from the [GitHub repository](https://github.com/juraganled/STM32_QuadEncoder).
2. Place the `STM32_QuadEncoder` folder in the Arduino `libraries` directory.
3. Restart the Arduino IDE.

## Getting Started

### Basic Usage

1. Connect your quadrature encoder to the STM32 pins (A and B signals).
2. Initialize the `STM32_QuadEncoder` object in your sketch.
3. Use the library methods to read counts, detect direction, and handle overflow events.

### Example Code

#### Basic Encoder Reading

```cpp
#include "STM32_QuadEncoder.h"

const int ROTARY_A = PB6;
const int ROTARY_B = PB7;

STM32_QuadEncoder encoder;

#define PPR 4

void setup() {
  Serial.begin(115200);
  encoder.begin(ROTARY_A, ROTARY_B, NO_PULLUP, PPR, DIRECTION_NORMAL);
}

void loop() {
  Serial.print("Count: ");
  Serial.println(encoder.getCount());
  delay(500);
}
```

#### Detecting Rotation Direction with Overflow

```cpp
#include "STM32_QuadEncoder.h"

const int ROTARY_A = PB6;
const int ROTARY_B = PB7;

STM32_QuadEncoder encoder;

#define PPR 2

void findDirection() {
  Serial.println(encoder.getDirection() == IS_COUNTING_UP ? "UP" : "DOWN");
}

void setup() {
  Serial.begin(115200);
  encoder.begin(ROTARY_A, ROTARY_B, NO_PULLUP, PPR, DIRECTION_NORMAL);
  encoder.attach(findDirection);
}

void loop() {
  // Main loop is left empty; logic is handled by the library.
}
```

## API Reference

### Initialization
- `STM32_QuadEncoder()` — Default constructor.
- `begin(pinA, pinB, channel, pulsePerRotation, direction)` — Initialize the encoder with specific settings.

### Reading Encoder Values
- `getCount()` — Returns the current encoder count.
- `resetCount()` — Resets the encoder count to zero.
- `setCount(value)` — Sets the encoder count to a specified value.
- `getDirection()` — Returns the current rotation direction (`IS_COUNTING_UP` or `IS_COUNTING_DOWN`).

### Callbacks
- `attach(void (*func)())` — Attaches a function to handle encoder overflow events.
- `detach()` — Detaches the overflow handler function.

### Configuration
- `setPPR(pulsePerRotation)` — Sets the pulses per revolution (PPR).
- `setDirection(direction)` — Sets the encoder direction (`DIRECTION_NORMAL` or `DIRECTION_REVERSE`).

## Supported Boards

This library is compatible with STM32 boards using the STM32 Arduino Core.

## Author

Based on forum thread https://www.stm32duino.com/viewtopic.php?t=1962 by Bakisha

- **Ricky Suprayudi**  
  [GitHub Repository](https://github.com/juraganled/STM32_QuadEncoder)  
  Contact: juraganled@gmail.com

## License

This library is open-source and distributed under the MIT License.
```

This README provides an overview of the library, installation instructions, example code, and API documentation to help users understand and use the library effectively.