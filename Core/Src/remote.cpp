/**
******************************************************************************
 * @file    remote.cpp/h
 * @brief   Remote control. 遥控器
 ******************************************************************************
 * Copyright (c) 2026 Team JiaoLong-SJTU
 * All rights reserved.
 ******************************************************************************
 */

#include "remote.h"

#include <string.h>

constexpr uint16_t REMOTE_CONNECT_TIMEOUT = 500u; 
// Constructor 构造函数
Remote::Remote(UART_HandleTypeDef *huart): huart_(huart), connect_(REMOTE_CONNECT_TIMEOUT){
    switch_.l = RCSwitchState_e::DOWN;
    switch_.r = RCSwitchState_e::DOWN;
}

// Start UART(SBUS) receive. 打开UART接收
void Remote::init() {
    HAL_UARTEx_ReceiveToIdle_DMA(huart_,rx_buf,RC_RX_BUF_SIZE);

    // Your code here 
}

// Reset RC data. 重置遥控器数据
void Remote::reset() {
    rx_len_=0;
    memset(rx_data_, 0, RC_FRAME_LEN);
    channel_.l_col=1024;
    channel_.l_row=1024;
    channel_.r_col=1024;
    channel_.r_row=1024;
    channel_.dial_wheel=1024;

    // Your code here 
}

// Check for uart correspondence. 检查串口是否匹配
void Remote::rxMsgCheck(UART_HandleTypeDef *huart) const {
    if (huart!=huart_)
    {
        return;
    }
    // Your code here 

}

// Update connect status, restart UART(SBUS) receive.
// 更新连接状态，重新打开ART(SBUS)接收
void Remote::rxMsgCallback(uint8_t* rx_data){
    connect_.refresh();
    memcpy(rx_data_, rx_data, RC_FRAME_LEN);
    rx_len_=RC_FRAME_LEN;
    handle();
    HAL_UARTEx_ReceiveToIdle_DMA(huart_,rx_buf,RC_RX_BUF_SIZE);
    // Your code here 

}

// Unpack data. 数据解包
//8+3+5+6+2+8+1+7+4
void Remote::handle() {
    channel_.r_col=(uint16_t(rx_data_[0])|uint16_t(rx_data_[1]<<8))&0x07ff;
    channel_.r_row=(uint16_t(rx_data_[1]>>3)|uint16_t(rx_data_[2]<<5))&0x07ff;
    channel_.l_col=(uint16_t(rx_data_[2]>>6)|uint16_t(rx_data_[3]<<2)|uint16_t(rx_data_[4]<<10))&0x07ff;
    channel_.l_row=(uint16_t(rx_data_[4]>>1)|uint16_t(rx_data_[5]<<7))&0x07ff;
    //channel_.dial_wheel=(uint)
    // Your code here 

}
