#include "STM32_QuadEncoder.h"

STM32_QuadEncoder::STM32_QuadEncoder() {
    Encoder = nullptr;
    timerInstance = nullptr;
}


STM32_QuadEncoder::STM32_QuadEncoder(uint32_t pinA, uint32_t pinB, ChannelPullUpTypeDef channelPullUp, unsigned long pulsePerRotation, DirectionTypeDef direction) {
    begin(pinA, pinB, channelPullUp, pulsePerRotation, direction);
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

void STM32_QuadEncoder::begin(uint32_t pinA, uint32_t pinB, ChannelPullUpTypeDef channelPullUp, unsigned long pulsePerRotation, DirectionTypeDef direction) {
    // get timer instance to be referenced upon based on encoder input pin
    timerInstance  = (TIM_TypeDef *)pinmap_peripheral(digitalPinToPinName(pinA), PinMap_PWM);
    
    // store pin into global variables, will be used for setDirection function
    globalPinA = pinA;
    globalPinB = pinB;

    // declare timer number, useful for attaching interrupt if needed
    #if defined(TIM1_BASE)
        if(timerInstance == TIM1) {
            timerNumber = 1;
        }
    #endif
    #if defined(TIM2_BASE)
        if(timerInstance == TIM2) {
            timerNumber = 2;
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
    #if defined(TIM5_BASE)
        if(timerInstance == TIM5) {
            timerNumber = 5;
        }
    #endif
    if (timerNumber == UNKNOWN_TIMER) {
        Error_Handler();
    }

    // init Encoder instance
    Encoder = new HardwareTimer(timerInstance);

    // configure encoder input pin with pull up if needed
    if(channelPullUp == NO_PULLUP) {
        pinMode(pinA, INPUT);  // set channel A to INPUT
        pinMode(pinB, INPUT);  // set channel B to INPUT
    } else {
        pinMode(pinA, INPUT_PULLUP);  // pull up on channel A
        pinMode(pinB, INPUT_PULLUP);  // pull up on channel B
    }

    // pause the timer
    Encoder->pause();

    // set timer into input capture, depending on the required direction it can be either falling or rising
    Encoder->setMode(STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(pinA), PinMap_PWM)), direction == DIRECTION_NORMAL ? TIMER_INPUT_CAPTURE_FALLING : TIMER_INPUT_CAPTURE_RISING, pinA, FILTER_DTS32_N8);
    Encoder->setMode(STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(pinB), PinMap_PWM)), TIMER_INPUT_CAPTURE_RISING, pinB, FILTER_DTS32_N8);

    // set timer SMCR register to encoder mode (SMS = 011)
    timerInstance->SMCR |= TIM_ENCODERMODE_TI12;

    // set prescaler divide by 4, so that quadrature encoder incremented by one if there are total of 4 clocks from either pinA or pinB
    Encoder->setPrescaleFactor(4);

    // set overflow enable
	Encoder->setPreloadEnable(true);

    // set encoder overflow value
	Encoder->setOverflow(pulsePerRotation, TICK_FORMAT);

    // reset encoder value to zero
	Encoder->setCount(0);

    // start encoder
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
    Encoder->setMode(STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(globalPinA), PinMap_PWM)), direction == DIRECTION_NORMAL ? TIMER_INPUT_CAPTURE_FALLING : TIMER_INPUT_CAPTURE_RISING, globalPinA, FILTER_DTS32_N8);
    Encoder->setMode(STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(globalPinB), PinMap_PWM)), TIMER_INPUT_CAPTURE_RISING, globalPinB, FILTER_DTS32_N8);
    Encoder->resume();
    Encoder->refresh();
}

int STM32_QuadEncoder::getTimerNumber() {
    return timerNumber;
}