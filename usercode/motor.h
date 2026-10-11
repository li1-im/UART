//
// Created by zyfdx on 2026/10/3.
//

#ifndef CAN_MOTOR_H
#define CAN_MOTOR_H

#include <sys/_stdint.h>
#include "PID.h"


class Motor
{
    public:
        // explicit Motor(const float ratio): ratio_(ratio){}
    ;//Motor...{
    //...=...区别
        enum Mode{POSITION_SPEED,SPEED,TOQUE};
        Motor(float ratio,PID& ppid,PID& spid,Mode mode):ratio_(ratio),ppid_(ppid),spid_(spid),mode_(mode){};
        void canRxMsgCallback(const uint8_t rx_data[8]);
        float angle() const {return angle_;}
        float speeddps() const;
        float speedRpm() const {return speedRpm_;}
        float currentAmps() const {return currentA_;}
        float temperature() const {return tempC_;}
        float currentOutput() const {return current_output;}
        uint8_t *data() {return tx_data_;};
        void setTxCurrent(float amperes,uint8_t motor_id);
        void handle(float target_angle,float feedforward_speed,float feedforward_intensity,uint8_t motor_id);
        void setGains(float pp_kp,float pp_ki,float pp_kd,float sp_kp,float sp_ki,float sp_kd);
        void setLimits(float pp_imax,float pp_outmax,float pp_dfilt,
                       float sp_imax,float sp_outmax,float sp_dfilt);
        void reset(){ppid_.reset();spid_.reset();}
        void stop();
        bool first_receive=true;
        void setMode(Mode mode){mode_=mode;}
    private:
        const float ratio_;
        float ecd_angle_=0;
        float speedRpm_=0;
        float currentA_=0;
        float tempC_=0;
        float angle_=0;
        float last_ecd_=0;
        uint8_t tx_data_[8]={};
        PID& ppid_;
        PID& spid_;
        Mode mode_;
        float current_output=0;
        static constexpr uint16_t kEncoderRange =8192;
        uint32_t last_receive=0;
};
#endif //CAN_MOTOR_H
