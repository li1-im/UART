//
// Created by zyfdx on 2026/10/3.
//
#include "main.h"
#include "can_user.h"
#include "motor.h"
#include "can.h"
// Motor motor1(19.2f);
PID ppid(7.0f,0.0f,0.1f,0.0f,200.0f,0.05f);
PID spid(0.5f, 0.0f, 0.01f, 0.0f, 20.0f, 0.05f);
Motor motor1(19.2f,ppid,spid,Motor::POSITION_SPEED);
float target_angle=0.0f;
float feedforward_speed=0.0f;
float feedforward_intensity=0.0f;
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    uint8_t rx_data[8];
    if (hcan->Instance == CAN1)
    {
        if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0,&rx_header,rx_data)==HAL_OK)
        {
            motor1.canRxMsgCallback(rx_data);
        }
    }
    //调用函数copy数据，放进队列里，保持函数简洁
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance != TIM6) return;
    motor1.handle(target_angle,feedforward_speed,feedforward_intensity,2);
    if (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1)>0)
    {
        HAL_CAN_AddTxMessage(&hcan1,&tx_header,motor1.data(),&can_tx_mailbox);
    }
}