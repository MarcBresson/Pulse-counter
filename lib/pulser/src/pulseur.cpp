#include "pulseur.h"
#include "Arduino.h"

PulseListener::PulseListener(const uint8_t Pin, const uint32_t Throttle, const uint32_t DeadTime){
    pin = Pin;
    throttle = Throttle;
    deadTime = DeadTime;
}

void PulseListener::update(){
    pinLastState = pinCurrState;
    pinCurrState = getCurrentState();

    if (pinLastState == LOW && pinCurrState == HIGH && pulseCurrent + deadTime < millis()){
        if (pulseCount == 0){
            pulseFirst = pulseCurrent;
        }

        pulseLast = pulseCurrent;

        pulseCurrent = millis();
        pulseCount ++;
    }
}

bool PulseListener::getCurrentState(){
    return digitalRead(pin) ? LOW : HIGH;
}

bool PulseListener::isReady(){
    return (millis() - lastResetCall > throttle) && (pulseCount > 0);
}

bool PulseListener::isNewPulseReady(uint16_t lastCount){
    return isReady() && pulseCount > lastCount;
}

uint16_t PulseListener::getPulseCount(){
    return pulseCount;
}

uint32_t PulseListener::getIntervalValue(){
    return pulseCurrent - pulseFirst;
}

void PulseListener::resetTimer(){
    lastResetCall = pulseCurrent;
}

void PulseListener::resetCount(){
    pulseCount = 0;
}

void PulseListener::reset(){
    resetTimer();
    resetCount();
}
