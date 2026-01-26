/*
 * ActionLolinMotorOutput.h
 * Ansteuerung LOLIN I2C Motor Shield
 */

#ifndef ACTIONLOLINMOTOROUTPUT_H_
#define ACTIONLOLINMOTOROUTPUT_H_

#include <Arduino.h>

// RemoteDebug definiert VERSION → Konflikt mit LOLIN_I2C_MOTOR
#ifdef VERSION
#undef VERSION
#endif

#include "LOLIN_I2C_MOTOR.h"
#include "ActionPWMOutput.h"

class ActionLolinMotorOutput : public ActionPWMOutput {
public:
    ActionLolinMotorOutput(uint8_t i2caddr, uint8_t ch);
    virtual ~ActionLolinMotorOutput();

    void setSpeed(int l);                 // kein override, da Basisklasse keine setSpeed() hat
    void setFreq(uint32_t value) override;

private:
    void setSettings(String key, String value) override;

    LOLIN_I2C_MOTOR motor;
    uint8_t channel;
    int currentSpeed = 0;
};

#endif /* ACTIONLOLINMOTOROUTPUT_H_ */
