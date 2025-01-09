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

/* Class --------------------------------------------------------*/
class STM32_QuadEncoder {
    public:
        STM32_QuadEncoder();    // constructor
        STM32_QuadEncoder(uint32_t pinA, uint32_t pinB, ChannelPullUpTypeDef channel, unsigned long pulsePerRotation, DirectionTypeDef direction);
        ~STM32_QuadEncoder();   // destructor
        void begin(uint32_t pinA, uint32_t pinB, ChannelPullUpTypeDef channel, unsigned long pulsePerRotation, DirectionTypeDef direction); // Setup, only needed if no instance was passed to the constructor
        unsigned long getCount();   // get encoder value
        void resetCount();          // set encoder value to zero
        void setCount(unsigned long value); // set encoder value
        CountingDirectionTypeDef getDirection();   // get encoder direction
        void attach(void (*func)());    // attach a function to execute whenever there is overflow on encoder
        void detach();  // remove any attached function
        void setPPR(unsigned long pulsePerRotation);    // set encoder maximum counting
        unsigned long getPPR(); // get maximum counting of the encoder
        bool hasInterrupt();    // findout if there are any function attached
        void setDirection(DirectionTypeDef direction);    // set encoder direction, useful for accomodating reversed encoder wiring

    private:
        int timerNumber = UNKNOWN_TIMER;
        uint32_t globalPinA;
        uint32_t globalPinB;
        TIM_TypeDef *timerInstance;
        TIM_Encoder_InitTypeDef channelEncoder;
};

#endif