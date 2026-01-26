#include "ActionLolinMotorOutput.h"
#include "Logger.h"
#include "Consts.h"
#include "Utils.h"

ActionLolinMotorOutput::ActionLolinMotorOutput(uint8_t i2caddr, uint8_t ch)
    : motor(i2caddr), channel(ch)
{
    Logger::getInstance()->printf(LogLevel::INFO,
        "Starting LOLIN I2C Motor ... I2C: %d / ch %d", i2caddr, ch);

    if (channel > MOTOR_CH_BOTH) {
        Logger::getInstance()->printf(LogLevel::ERROR,
            "LOLIN motor channel invalid: %d", ch);
        channel = MOTOR_CH_A;
    }

    motor.getInfo();

    // VERSION-Feld auslesen ohne das VERSION-Macro zu triggern
    uint8_t ver = *((uint8_t*)&motor + 1);

    Logger::getInstance()->printf(LogLevel::INFO,
        "LOLIN Motor PRODUCT_ID=%d VERSION=%d",
        motor.PRODUCT_ID,
        ver);
}

ActionLolinMotorOutput::~ActionLolinMotorOutput()
{
}

void ActionLolinMotorOutput::setFreq(uint32_t freq)
{
    Logger::log(LogLevel::TRACE, "LOLIN Motor Freq: " + String(freq));
    motor.changeFreq(channel, freq);
}

void ActionLolinMotorOutput::setSpeed(int l)
{
    currentSpeed = l;

    uint8_t status = MOTOR_STATUS_STOP;
    float duty = 0.0f;

    if (l > 0) {
        status = MOTOR_STATUS_CW;
        duty = (float)l;
    } else if (l < 0) {
        status = MOTOR_STATUS_CCW;
        duty = (float)(-l);
    } else {
        status = MOTOR_STATUS_STOP;
        duty = 0.0f;
    }

    if (duty > 100.0f) duty = 100.0f;

    Logger::log(LogLevel::TRACE,
        "LOLIN Motor DirStatus: " + String(status) +
        " Duty: " + String(duty, 2));

    motor.changeStatus(channel, status);
    motor.changeDuty(channel, duty);
}

void ActionLolinMotorOutput::setSettings(String key, String value)
{
    Logger::log(LogLevel::TRACE,
        "LOLIN Motor setSettings " + key + "/" + value);

    if (key.equals("sd")) {
        setSpeed(value.toInt());
    } else if (key.equals("freq")) {
        setFreq(value.toInt());
    }
}
