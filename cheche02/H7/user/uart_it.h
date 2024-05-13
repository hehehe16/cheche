#ifndef _uart_it__H
#define _uart_it__H

#include "main.h"

void uart_it_receive(UART_HandleTypeDef* huart,uint8_t Data[],uint8_t* state);
void uart_it_init(void);
void uart_it_receive1(UART_HandleTypeDef* huart,uint8_t* Data,uint8_t* state);

typedef struct uart_handle
{
	UART_HandleTypeDef *uart;
	uint8_t receive_data[20];
	
	void (* transmit)(struct uart_handle *uart_handle, const uint8_t *pData, uint16_t Size, uint32_t Timeout);
	void (* print)(struct uart_handle *uart_handle,char *format,...);
	void (* receive)(struct uart_handle *uart_handle);

	
}uart_handle;

void uart_transmit(uart_handle *uart_handle, const uint8_t *pData, uint16_t Size, uint32_t Timeout);
void usart_printf(uart_handle *uart_handle,char *format,...);
void uart_it_receive3(uart_handle *uart_handle);
uint8_t uart_handle_init(uart_handle (*uart_handle),UART_HandleTypeDef *huart);
#endif