//
// Created by zyfdx on 2026/10/11.
//

#ifndef CAN_PID_H
#define CAN_PID_H

class PID
{
public:
    PID(float kp, float ki, float kd,float i_max,float out_max,float d_filter_k):
    kp_(kp),ki_(ki),kd_(kd),i_max_(i_max),out_max_(out_max),
    d_filter_k_(d_filter_k),intergel(0.0f),lasterror(0.0f),dout(0.0f){}//outmax : 16384-20
    void reset();
    float calc(float ref,float fdb);
private:
    float kp_;
    float ki_;
    float kd_;
    float i_max_;
    float out_max_;
    float d_filter_k_;
    float intergel;
    float lasterror;//改不了，要set
    float dout;
};


#endif //CAN_PID_H
