#ifndef class_PulseListener
#define class_PulseListener

#include "Arduino.h"
#include <stdlib.h>

class PulseListener{
    public:
        PulseListener(uint8_t Pin, uint32_t Throttle = 1000, uint32_t DeadTime = 10);
        void        update();
        bool        getCurrentState();
        bool        isReady();
        bool        isNewPulseReady(uint16_t lastCount);
        void        resetTimer();
        void        resetCount();
        void        reset();
        uint16_t    getPulseCount();
        uint32_t    getIntervalValue();

    private:
        bool    is_valid;

        uint8_t pin;
        bool    pinLastState = LOW;
        bool    pinCurrState = LOW;

        uint16_t pulseCount;
        uint32_t pulseCurrent;
        uint32_t pulseLast;
        uint32_t pulseFirst;

        uint32_t deadTime;
        uint32_t throttle;
        uint32_t lastResetCall;
};

#endif
