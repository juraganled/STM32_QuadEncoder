#ifndef STM32_QUADENCODER_H
#define STM32_QUADENCODER_H

#ifndef ARDUINO_ARCH_STM32
#error "This library is only compatible with STM32 boards using the STM32 Arduino Core."
#endif

#include <Arduino.h>
#include "HardwareTimer.h"

#define CHANNEL_12 0
#define CHANNEL_1 1
#define CHANNEL_2 2
#define NO_PULLUP 3

#define DIR_NORMAL 0
#define DIR_REVERSE 1

static HardwareTimer *Encoder;

class STM32_QuadEncoder {
    public:
        STM32_QuadEncoder(uint32_t pinA, uint32_t pinB, int channel, unsigned long pulsePerRotation, int direction);
        unsigned long getCount();
        void resetCount();
        void setCount(unsigned long value);
        // int direction();
        void attach(void (*func)());
        void detach();
        void setPPR(unsigned long pulsePerRotation);
        unsigned long getPPR();
        bool hasInterrupt();
        void setMode(int direction);


    private:
        int timerNumber = UNKNOWN_TIMER;
};

#endif