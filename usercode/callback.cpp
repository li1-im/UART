//
// Created by zyfdx on 2026/10/3.
//
#include "main.h"
#include "can_user.h"
#include "motor.h"
#include "can.h"
#include <cmath>
// Motor motor1(19.2f);
PID ppid(7.0f,0.0f,0.1f,0.0f,200.0f,0.05f);
PID spid(0.0f, 0.0f, 0.00f, 0.0f, 10.0f, 0.05f);
Motor motor1(36.1f,ppid,spid,Motor::POSITION_SPEED);
volatile uint8_t stop_flag=0;
volatile float g_kp_pp=37.0f;      volatile float g_ki_pp=0.0f;      volatile float g_kd_pp=0.45f;
volatile float g_kp_sp=0.08f;      volatile float g_ki_sp=0.0f;      volatile float g_kd_sp=0.04f;
volatile float g_imax_pp=0.0f;    volatile float g_outmax_pp=500.0f; volatile float g_dfilt_pp=0.05f;
volatile float g_imax_sp=0.0f;    volatile float g_outmax_sp=10.0f;  volatile float g_dfilt_sp=0.5f;
volatile  float target_angle=0.0f;
float feedforward_speed=0.0f;
volatile float feedforward_intensity=0.0f;
volatile float g_ff_scale=1.4f;
volatile float g_fdb_angle=0;
volatile float g_fdb_speed=0;
volatile float g_out=0;
volatile uint32_t g_can_rx_cnt=0;
volatile uint32_t g_tx_ok=0;
volatile uint8_t g_mode=2;
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    uint8_t rx_data[8];
    if (hcan->Instance != CAN1) return;
    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0,&rx_header,rx_data)!=HAL_OK) return;
    if (rx_header.StdId==0x201)
    {
        motor1.canRxMsgCallback(rx_data);
    }
    //调用函数copy数据，放进队列里，保持函数简洁
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance != TIM6) return;
    if (stop_flag)
    {
        motor1.stop();
    }
    else
    {
        motor1.setGains(g_kp_pp,g_ki_pp,g_kd_pp,g_kp_sp,g_ki_sp,g_kd_sp);
        motor1.setLimits(g_imax_pp,g_outmax_pp,g_dfilt_pp,g_imax_sp,g_outmax_sp,g_dfilt_sp);
        float rad=motor1.angle()*3.1415926f/180.0f;
        feedforward_intensity=g_ff_scale*(0.1f*9.8f*0.065f/0.18f)*sinf(rad);
        if (g_mode==2)      motor1.setMode(Motor::SPEED);
        else if (g_mode==3) motor1.setMode(Motor::TOQUE);
        else  motor1.setMode(Motor::POSITION_SPEED);
        motor1.handle(target_angle,feedforward_speed,feedforward_intensity,1);
    }

    g_fdb_angle=motor1.angle();
    g_fdb_speed=motor1.speeddps();
    g_out=motor1.currentOutput();
    if (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1)>0)
    {
        HAL_CAN_AddTxMessage(&hcan1,&tx_header,motor1.data(),&can_tx_mailbox);
    }
}