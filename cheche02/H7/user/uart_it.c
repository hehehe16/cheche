#include "uart_it.h"
#include "usart.h"

#include "stdio.h"
#include <stdarg.h>

static uint8_t Rx_state = 0;//串口接收阶段
static uint8_t com_date = 0;//串口缓存￿??
static uint8_t Rx_counter = 0;//接收数组计数

static uint8_t Rx_state1 = 0;//串口接收阶段
static uint8_t com_date1 = 0;//串口缓存￿??
static uint8_t Rx_counter1 = 0;//接收数组计数

static uint8_t Rx_state2 = 0;//串口接收阶段
static uint8_t com_date2 = 0;//串口缓存￿??
static uint8_t Rx_counter2 = 0;//接收数组计数

static uint8_t a=1;

void uart_it_init(void)
{
	HAL_UART_Receive_IT(&huart1,&com_date,1);//opmv
	HAL_UART_Receive_IT(&huart6,&com_date1,1);//maix_cam
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
		Rx_state1=1;
	}
	else if (Rx_state1 == 1 && com_date1 == 0x12)
	{
		Rx_state1=2;
	}
	else if (Rx_state1 == 2)
	{
		if (com_date1 == 0x5B)
		{
			Rx_state1 = 0;
			Rx_counter1 = 0;
			*state = 1;
			
		}
		if (*state == 0)
		{
			Data[Rx_counter1++] = com_date1;
			//Data[Rx_counter1++] = com_date1;
		}
	}
}
HAL_UART_Receive_IT(huart, &com_date1, 1);
}

void uart_it_receive3(uart_handle *uart_handle)
{
	UART_HandleTypeDef uart;
	uart = uart_handle->uart;
	uint8_t state  = 0;
if(state == 0) // state=0：处于等待接收或接收过程状�?? state=1:处于接收完毕等待处理状�??
{

	if (Rx_state2 == 0 && com_date2 == 0x2C)
	{
		Rx_state2 = 1;
	}
	else if (Rx_state2 == 1 && com_date2 == 0x12)
	{
		Rx_state2 = 2;
	}
	else if (Rx_state2 == 2)
	{
		if (com_date2 == 0x5B)
		{
			Rx_state2 = 0;
			Rx_counter2 = 0;
			state = 1;
			
		}
		if (state == 0)
		{
			uart_handle->receive_data[Rx_counter2++] = com_date2;
		}
	}
}
HAL_UART_Receive_IT(&uart, &com_date2, 1);
}



void uart_transmit(uart_handle *uart_handle, const uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
	UART_HandleTypeDef uart;
	uart = uart_handle->uart;
	
	
	HAL_UART_Transmit(&uart,pData, Size, Timeout);
}


void usart_printf(uart_handle *uart_handle,char *format,...)
{
	UART_HandleTypeDef uart;
	uart = uart_handle->uart;
	int i =0;
	char String[100];		 //定义输出字符串
	va_list arg;			 //定义一个参数列表变量va_list是一个类型名，arg是变量名
	va_start(arg,format);	 //从format位置开始接收参数表放在arg里面
	
	//sprintf打印位置是String，格式化字符串是format，参数表是arg，对于封装格式sprintf要改成vsprintf
	vsprintf(String,format,arg);
	va_end(arg);			 //释放参数表
	while(1)
	{
		if(String[i] == 0)
		{
			return;
		}
		HAL_UART_Transmit(&uart,(uint8_t *)&String[i], 1, 0xffff);
		i++;
	}	
}







