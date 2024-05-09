#ifndef _uart_it__H
#define _uart_it__H

#include "main.h"

void uart_it_receive(UART_HandleTypeDef* huart,uint8_t Data[],uint8_t* state);
void uart_it_init(void);
void uart_it_receive1(UART_HandleTypeDef* huart,uint8_t* Data,uint8_t* state);
#endif