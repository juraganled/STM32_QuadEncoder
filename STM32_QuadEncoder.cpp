#include "STM32_QuadEncoder.h"

STM32_QuadEncoder::STM32_QuadEncoder() {
    Encoder = nullptr;
    timerInstance = nullptr;
}


STM32_QuadEncoder::STM32_QuadEncoder(uint32_t pinA, uint32_t pinB, ChannelPullUpTypeDef channelPullUp, unsigned long pulsePerRotation, DirectionTypeDef direction) {
    setup(pinA, pinB, channelPullUp, pulsePerRotation, direction);
}

STM32_QuadEncoder::~STM32_QuadEncoder() {
    Encoder->pause();
    if (Encoder->hasInterrupt()) {
        Encoder->detachInterrupt();
    }
    pinMode(globalPinA, INPUT);  // reset channel A to INPUT
    pinMode(globalPinB, INPUT);  // reset channel B to INPUT
    timerInstance = nullptr;
    Encoder = nullptr;
}

void STM32_QuadEncoder::setup(uint32_t pinA, uint32_t pinB, ChannelPullUpTypeDef channelPullUp, unsigned long pulsePerRotation, DirectionTypeDef direction) {
    timerInstance  = (TIM_TypeDef *)pinmap_peripheral(digitalPinToPinName(pinA), PinMap_PWM);
    globalPinA = pinA;
    globalPinB = pinB;

    #if defined(TIM1_BASE)
        if(timerInstance == TIM1) {
            timerNumber = 1;
        }
    #endif
    #if defined(TIM3_BASE)
        if(timerInstance == TIM3) {
            timerNumber = 3;
        }
    #endif
    #if defined(TIM4_BASE)
        if(timerInstance == TIM4) {
            timerNumber = 4;
        }
    #endif
    if (timerNumber == UNKNOWN_TIMER) {
        Error_Handler();
    }

    Encoder = new HardwareTimer(timerInstance);
    Encoder->pause();
    // Encoder->setMode(1, direction == DIR_NORMAL ?? TIMER_ENCODER : TIMER_ENCODER_REVERSE)
    Encoder->setMode(STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(pinA), PinMap_PWM)), direction == DIRECTION_NORMAL ? TIMER_INPUT_CAPTURE_FALLING : TIMER_INPUT_CAPTURE_RISING, pinA, FILTER_DTS32_N8);
    Encoder->setMode(STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(pinB), PinMap_PWM)), direction == DIRECTION_NORMAL ? TIMER_INPUT_CAPTURE_FALLING : TIMER_INPUT_CAPTURE_RISING, pinB, FILTER_DTS32_N8);
    timerInstance->SMCR |= TIM_ENCODERMODE_TI12;
    Encoder->setPrescaleFactor(4);
	Encoder->setPreloadEnable(true);
	Encoder->setOverflow(pulsePerRotation, TICK_FORMAT);

    pinMode(pinA, INPUT);  // set default channel A to INPUT
    pinMode(pinB, INPUT);  // set default channel B to INPUT
    switch(channelPullUp) {
        case CHANNEL_PULLUP_12:
            pinMode(pinA, INPUT_PULLUP);  //channel A
            pinMode(pinB, INPUT_PULLUP);  //channel B
            break;
        case CHANNEL_PULLUP_1:
            pinMode(pinA, INPUT_PULLUP);  //channel A
            break;
        case CHANNEL_PULLUP_2:
            pinMode(pinB, INPUT_PULLUP);  //channel B
            break;
        case CHANNEL_NO_PULLUP:
        default:
        break;
    }

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

CountingDirectionTypeDef STM32_QuadEncoder::getDirection() {
    return (__HAL_TIM_IS_TIM_COUNTING_DOWN(Encoder->getHandle()) ? IS_COUNTING_DOWN : IS_COUNTING_UP);
}

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

void STM32_QuadEncoder::setDirection(DirectionTypeDef direction) {
    // Encoder->setMode(1, direction == DIR_NORMAL ?? TIMER_ENCODER : TIMER_ENCODER_REVERSE)
    Encoder->setMode(STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(globalPinA), PinMap_PWM)), direction == DIRECTION_NORMAL ? TIMER_INPUT_CAPTURE_FALLING : TIMER_INPUT_CAPTURE_RISING, globalPinA, FILTER_DTS32_N8);
    Encoder->setMode(STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(globalPinB), PinMap_PWM)), direction == DIRECTION_NORMAL ? TIMER_INPUT_CAPTURE_FALLING : TIMER_INPUT_CAPTURE_RISING, globalPinB, FILTER_DTS32_N8);
    Encoder->resume();
    Encoder->refresh();
}
