//
// Created by zyfdx on 2026/10/11.
//
#include "PID.h"
#include <algorithm>

void PID::reset()
{
    lasterror=0.0f;
    intergel=0.0f;
    dout=0.0f;
}
float PID::calc(float ref,float fdb)
{
    float error=ref-fdb;
    intergel+=error;
    intergel=std::clamp(intergel,-i_max_,i_max_);
    float delta=error-lasterror;
    dout=delta*d_filter_k_+(1-d_filter_k_)*dout;
    lasterror=error;
    float out=ki_*intergel+kd_*dout+kp_*error;
    out=std::clamp(out,-out_max_,out_max_);
    return out;
}