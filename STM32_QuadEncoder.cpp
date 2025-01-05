#include "STM32_QuadEncoder.h"

STM32_QuadEncoder::STM32_QuadEncoder(uint32_t pinA, uint32_t pinB, int channelPullUp, unsigned long pulsePerRotation, int direction) {
    TIM_TypeDef *Instance  = (TIM_TypeDef *)pinmap_peripheral(digitalPinToPinName(pinA), PinMap_PWM);
    uint32_t channel_1 = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(pinA), PinMap_PWM));
    uint32_t channel_2 = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(pinB), PinMap_PWM));

    Encoder = new HardwareTimer(Instance);

    #if defined(TIM1_BASE)
        if(Instance == TIM1) {
            timerNumber = 1;
        }
    #endif
    #if defined(TIM3_BASE)
        if(Instance == TIM3) {
            timerNumber = 3;
        }
    #endif
    #if defined(TIM4_BASE)
        if(Instance == TIM4) {
            timerNumber = 4;
        }
    #endif
    if (timerNumber == UNKNOWN_TIMER) {
        Error_Handler();
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

unsigned long STM32_QuadEncoder::getCount() {
    return Encoder->getCount(TICK_FORMAT);
}

void STM32_QuadEncoder::resetCount() {
    Encoder->setCount(0);
}

void STM32_QuadEncoder::setCount(unsigned long value) {
    Encoder->setCount(value);
}

// int direction() {
//     Encoder->getDirection();
// }

void STM32_QuadEncoder::attach(void (*func)()) {
    Encoder->attachInterrupt(timerNumber, (*func));
}

void STM32_QuadEncoder::detach() {
    Encoder->detachInterrupt(timerNumber);
}

void STM32_QuadEncoder::setPPR(unsigned long pulsePerRotation) {
    Encoder->setOverflow(pulsePerRotation, TICK_FORMAT);
    Encoder->resume();
    Encoder->refresh();
}

unsigned long STM32_QuadEncoder::getPPR() {
    return Encoder->getOverflow();
}

bool STM32_QuadEncoder::hasInterrupt() {
    return Encoder->hasInterrupt();
}

void STM32_QuadEncoder::setMode(int direction) {
    // Encoder->setMode(1, direction == DIR_NORMAL ?? TIMER_ENCODER : TIMER_ENCODER_REVERSE)
    Encoder->resume();
    Encoder->refresh();
}
