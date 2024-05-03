#include "uart_it.h"


static uint8_t Rx_state = 0;//串口接收阶段
static uint8_t com_date = 0;//串口缓存￿??
static uint8_t Rx_counter = 0;//接收数组计数

static uint8_t Rx_state1 = 0;//串口接收阶段
static uint8_t com_date1 = 0;//串口缓存￿??
static uint8_t Rx_counter1 = 0;//接收数组计数

static uint8_t a=1;

void uart_it_init(UART_HandleTypeDef* huart,uint8_t size)
{
	HAL_UART_Receive_IT(huart, &com_date, size);
}

void uart_it_receive(UART_HandleTypeDef* huart,uint8_t* Data,uint8_t* state)
{
if(*state == 0) // state=0：处于等待接收或接收过程状�?? state=1:处于接收完毕等待处理状�??
{

	if (Rx_state == 0 && com_date == 0x2C)
	{
		Rx_state = 1;
	}
	else if (Rx_state == 1 && com_date == 0x12)
	{
		Rx_state = 2;
	}
	else if (Rx_state == 2)
	{
		if (com_date == 0x5B)
		{
			Rx_state = 0;
			Rx_counter = 0;
			*state = 1;
		}
		if (*state == 0)
		{
			Data[Rx_counter++] = com_date;
		}
	}
}
HAL_UART_Receive_IT(huart, &com_date, 1);
}


void uart_it_receive1(UART_HandleTypeDef* huart,uint8_t* Data,uint8_t* state)
{
if (*state == 0) // state=0：处于等待接收或接收过程状�?? state=1:处于接收完毕等待处理状�??
{

	if (Rx_state1 == 0 && com_date1 == 0x2C)
	{
		Rx_state1++;
	}
	else if (Rx_state1 == 2 && com_date1 == 0x12)
	{
		Rx_state1++;
	}
	else if (Rx_state1 == 3)
	{
		if (com_date1 == 0x5B)
		{
			Rx_state1 = 0;
			Rx_counter1 = 0;
			*state = 1;
			a=1;
		}
		if (*state == 0)
		{
			if(a == 1)
			{
				Data[Rx_counter1] = com_date1;
				a=2;
			}
			else if(a == 2)
			{
				Data[Rx_counter1] = (Data[Rx_counter1]<<8)+com_date1;
				Rx_counter1++;
				a = 1;
			}
			//Data[Rx_counter1++] = com_date1;
		}
	}
}
HAL_UART_Receive_IT(huart, &com_date1, 1);
}