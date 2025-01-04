#include "STM32_QuadEncoder.h"

STM32_QuadEncoder::STM32_QuadEncoder(uint32_t pinA, uint32_t pinB, int channelPullUp, unsigned long pulsePerRotation, int direction) {
    TIM_TypeDef *Instance  = (TIM_TypeDe~f *)pinmap_peripheral(digitalPinToPinName(pinA), PinMap_PWM);
    uint32_t channel_1 = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(pinA), PinMap_PWM));
    uint32_t channel_2 = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(pinB), PinMap_PWM));

    Encoder = new HardwareTimer(Instance);

    if(Instance == TIM1) {
        timerNumber = 1;
    }
    if(Instance == TIM2) {
        timerNumber = 2;
    }
    if(Instance == TIM3) {
        timerNumber = 3;
    }
    if(Instance == TIM4) {
        timerNumber = 4;
    }
    if(Instance == TIM5) {
        timerNumber = 5;
    }
    

    switch(channelPullUp) {
        case CHANNEL_12:
            pinMode(pinA, INPUT_PULLUP);  //channel A
            pinMode(pinB, INPUT_PULLUP);  //channel B
            break;
        case CHANNEL_1:
            pinMode(pinA, INPUT_PULLUP);  //channel A
            break;
        case CHANNEL_2:
            pinMode(pinB, INPUT_PULLUP);  //channel B
            break;
        case NO_PULLUP:
        default:
        break;
    }

    Encoder->pause();
    // Encoder->setMode(1, direction == DIR_NORMAL ?? TIMER_ENCODER : TIMER_ENCODER_REVERSE)
    Encoder->setPrescaleFactor(4);
	Encoder->setPreloadEnable(true);
	Encoder->setOverflow(pulsePerRotation, TICK_FORMAT);
	Encoder->setCount(0);    
    Encoder->resume();
    Encoder->refresh();
}

unsigned int getCount() {
    return Encoder->getCount();
}

void resetCount() {
    Encoder->setCount(0);
}

void setCount(unsigned long value) {
    Encoder->setCount(value);
}

int direction() {
    Encoder->getDirection();
}

void attach(void (*func)()) {
    Encoder->attachInterrupt(timerNumber, (*func));
}

void detach() {
    Encoder->detachInterrupt(timerNumber);
}

void setPPR(unsigned long pulsePerRotation) (
    Encoder->setOverflow(pulsePerRotation, TICK_FORMAT);
    Encoder->resume();
    Encoder->refresh();
)

unsigned long getPPR() (
    return Encoder->getOverflow();
)

bool hasInterrupt() {
    return Encoder->hasInterrupt();
}

void setMode(int direction) {
    // Encoder->setMode(1, direction == DIR_NORMAL ?? TIMER_ENCODER : TIMER_ENCODER_REVERSE)
    Encoder->resume();
    Encoder->refresh();
}
