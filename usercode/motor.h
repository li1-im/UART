//
// Created by zyfdx on 2026/10/3.
//

#ifndef CAN_MOTOR_H
#define CAN_MOTOR_H

#include <sys/_stdint.h>

class Motor
{
    public:
        explicit Motor(const float ratio): ratio_(ratio){}
    ;//Motor...{
    //...=...区别
        void canRxMsgCallback(const uint8_t rx_data[8]);
        float angle() const;
        float speedRpm() const;
        float currentAmps() const;
        float temperature() const;
        uint8_t *data() {return tx_data_;};
        void setTxCurrent(float amperes,uint8_t motor_id);
        uint8_t* getTxData();
    private:
        const float ratio_;
        float ecd_angle_=0;
        float speedRpm_=0;
        float currentA_=0;
        float tempC_=0;
        float angle_=0;
        float last_ecd_=0;
        uint8_t tx_data_[8]={};

        static constexpr uint16_t kEncoderRange =8192;

};
#endif //CAN_MOTOR_H
