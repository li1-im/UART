//
// Created by zyfdx on 2026/10/2.
//

#include <string.h>

#include"main.h"
#include "remote.h"
#include "stm32f4xx_hal_gpio.h"
#include"usart.h"
// extern uint8_t rx_msg[10];
extern Remote remote;
// void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
// {

    // if (huart==&huart1)
    // {
    //     uint8_t tx_msg[10];
    //     memcpy(tx_msg,rx_msg,10);
    //     HAL_UART_Transmit_IT(&huart1,tx_msg,10);
    //     HAL_UART_Receive_IT(&huart1,rx_msg,10);
    //


        // if (rx_msg[0]=='R')
        // {
        //     HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_RESET);
        //     HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_RESET);
        //     HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_RESET);
        // }
        // else if (rx_msg[0]=='M')
        // {
        //     HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_SET);
        //     HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_SET);
        //     HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_SET);
        //      HAL_UART_Receive_DMA(&huart1, rx_msg, 1);
        // }

//     }
// }
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart,uint16_t Size)
{
    // if (huart==&huart1)
    // {
    //     // if (HAL_UARTEx_GetRxEventType(huart)!=HAL_UART_RXEVENT_HT){
    //     //     uint8_t tx_msg[10];
    //     //     memcpy(tx_msg,rx_msg,Size);
    //     //     HAL_UART_Transmit_IT(&huart1,tx_msg,Size);
    //     //     HAL_UARTEx_ReceiveToIdle_DMA(&huart1,rx_msg,10);
    //     // }
    // }
    if (huart==&huart3)
    {
        if (Size == RC_FRAME_LEN && HAL_UARTEx_GetRxEventType(huart)==HAL_UART_RXEVENT_IDLE)
        {
            remote.rxMsgCallback(remote.getRxBuf());
        }
    }
}