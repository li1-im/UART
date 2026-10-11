//
// Created by zyfdx on 2026/10/3.
//
#include "motor.h"

#include "stm32f4xx_hal.h"

float Motor::speeddps() const
{
    return speedRpm_*360.0f/60.0f/ratio_;
}
void Motor::canRxMsgCallback(const uint8_t rx_data[8])
{
    last_receive=HAL_GetTick();
    uint16_t raw_ecd= static_cast<uint16_t>(rx_data[0] << 8 | rx_data[1]);
    int16_t raw_speed= static_cast<int16_t>(rx_data[2] << 8 | rx_data[3]);
    int16_t raw_current= static_cast<int16_t>(rx_data[4] << 8 | rx_data[5]);
    ecd_angle_=static_cast<float>(raw_ecd);
    speedRpm_=static_cast<float>(raw_speed);
    currentA_=static_cast<float>(raw_current) * 20.0f / 16384.0f;
    tempC_=static_cast<float>(rx_data[6]);
    if (first_receive)
    {
        last_ecd_=ecd_angle_;
        first_receive=false;
    }
    float delta = ecd_angle_ - last_ecd_;
    if (delta>kEncoderRange / 2.0f)
    {
        delta-=kEncoderRange;
    }
    else if (delta<-kEncoderRange / 2.0f)
    {
        delta+=kEncoderRange;
    }
    angle_+= delta * 360.0f/kEncoderRange/ratio_;
    last_ecd_=ecd_angle_;
}

void Motor::setTxCurrent(float amperes,uint8_t motor_id)
{
    if (amperes>20.0f)
    {
        amperes=20.0f;
    }
    if (amperes<-20.0f)
    {
        amperes=-20.0f;
    }
    uint16_t raw=static_cast<int16_t>(amperes*16384.0f/20.0f);
    uint8_t index=static_cast<uint8_t>((motor_id - 1) * 2);
    tx_data_[index]=static_cast<uint8_t>((raw >> 8) & 0xFF);
    tx_data_[index+1]=static_cast<uint8_t>(raw & 0xFF);
}


uint8_t *Motor::getTxData() {
    return tx_data_;
}

void Motor::handle(float target_angle,float feedforward_speed,float feedforward_intensity,uint8_t motor_id)
{
    if (first_receive||HAL_GetTick()-last_receive>20)
    {
        current_output=0.0f;
        reset();
    }
    else
    {
        switch (mode_)
        {
        case POSITION_SPEED:
            current_output=spid_.calc(ppid_.calc(target_angle,angle())+feedforward_speed,
                speeddps())+feedforward_intensity;
            break;
        case SPEED:
            current_output=spid_.calc(target_angle,speeddps())+feedforward_intensity;
            break;
        case TOQUE:
            current_output=target_angle;
            break;//注意target_angle复用
        }
        setTxCurrent(current_output,motor_id);
    }
}