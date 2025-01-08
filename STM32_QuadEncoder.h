#ifndef STM32_QUADENCODER_H
#define STM32_QUADENCODER_H

#ifndef ARDUINO_ARCH_STM32
#error "This library is only compatible with STM32 boards using the STM32 Arduino Core."
#endif

#include <Arduino.h>
#include "HardwareTimer.h"

typedef enum {
    CHANNEL_NO_PULLUP     =   0x00U,
    CHANNEL_PULLUP_1      =   0x01U,
    CHANNEL_PULLUP_2      =   0x02U,
    CHANNEL_PULLUP_12     =   0x03U
} ChannelPullUpTypeDef;

typedef enum {
    DIRECTION_NORMAL        =   0x00U,
    DIRECTION_REVERSE       =   0x01U
} DirectionTypeDef;

typedef enum {
    IS_COUNTING_UP          =   0x00U,
    IS_COUNTING_DOWN        =   0x01U
} CountingDirectionTypeDef;

static HardwareTimer *Encoder;

class STM32_QuadEncoder {
    public:
        STM32_QuadEncoder(uint32_t pinA, uint32_t pinB, ChannelPullUpTypeDef channel, unsigned long pulsePerRotation, DirectionTypeDef direction);
        unsigned long getCount();
        void resetCount();
        void setCount(unsigned long value);
        CountingDirectionTypeDef direction();
        void attach(void (*func)());
        void detach();
        void setPPR(unsigned long pulsePerRotation);
        unsigned long getPPR();
        bool hasInterrupt();
        void setMode(int direction);

    private:
        int timerNumber = UNKNOWN_TIMER;
        uint32_t globalPinA;
        uint32_t globalPinB;
        TIM_TypeDef *timerInstance;
};

#endif