/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "oled.h"
#include "stdio.h"
#include <stdarg.h>
#include "motor.h"
#include "control.h"
#include "uart_it.h"

//********************************************************
uint8_t angle=150;   //90  120
uint8_t state = 0;   //串口状???
uint8_t Date[10] = {0}; //接收数组
uint8_t state1 = 0;   //串口状???
uint8_t Date1[20] = {0}; //接收数组
uint16_t shuliang = 0;   //数字个数
int number[4] = {0};
uint8_t tick = 0;      
int color =0; //当前胶带颜色 0red 1black 2wuse
uint8_t T_num=0;
//*********************************************************

//||||||||||||||||||||||||||||||||||||||||||||||||||||||||||//

uint8_t 
hhh,hhhh,hhhhh,									//一次性变量用与使特定程序只执行一次
zhongbingfangnum[2]={0},				//中间病房
for_ru_num[4]={0},							//4数字位置
moweibingfangnum[2]={0},					//终点病房数字位置
zhuanwan[4]={0,0,0,0},
back=0,
BL_RX[3]={0},
f1_zhong[1]={0},
bibibi=1//避障状态
;



int 
lukou=0,				//路口变量lukou为当前第几次路口，zhuanwan[]为第几次路口时的运动方向，用于返回
zhuangtai01 = -2,//用于确定车的普通状态，-2未收到开始命令，-1初始位置停止并且收到开始命令，0去时巡线执行，1避让，2病房处转一圈，3避让巡线，4病房处停止，5转一圈准备返回，6返回巡线拐弯,7速度返回，8停止
zhuangtai02 = -1,//用于确定车的发挥1状态，-1,停止，0正常寻线，1找到岔路口判断是否转弯并发送自己转弯状态，3低速巡线走，4黑线停止并发送命令（启动）并等待接收
timu=0
;
uint64_t set_jvli_A,set_jvli_B;
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
void sort(int b[],int a[],int len)    //排序算法 b:被排序对象  a：排序依据
{
	int temp[2];
	for(int i=0;i<len-1;i++)
	{
		for(int n=i+1;n<len;n++)
		{
			temp[0] = a[i];
			temp[1] = b[i];
			if(a[n]<a[i])
			{
				b[i] = b[n];
				b[n]= temp[1];
				a[i] = a[n];
				a[n] = temp[0];
			}
		}
	}
}


void led(char ch)
{
	if(ch=='F')
	{
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_9,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_8,GPIO_PIN_RESET);
	}
	else if(ch=='G')
	{	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_9,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_8,GPIO_PIN_RESET);}
	else if(ch=='Y')
	{	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_9,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_8,GPIO_PIN_SET);}
	else if(ch=='R')
	{	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_9,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_8,GPIO_PIN_RESET);}
}

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C2_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USART2_UART_Init();
  MX_I2C3_Init();
  MX_TIM5_Init();
  MX_USART1_UART_Init();
  MX_USART6_UART_Init();
  /* USER CODE BEGIN 2 */
	OLED_Init();
	OLED_FullyClear();
	HAL_TIM_Encoder_Start(&htim3,TIM_CHANNEL_1|TIM_CHANNEL_2);
	HAL_TIM_Encoder_Start(&htim4,TIM_CHANNEL_1|TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
	int i=0;
  motor_init();
	char dir[8];
	int num_shu[4] = {0};
	for(i=0;i<10;i++)
	{
  uart_it_init();
	
	HAL_Delay(10);
	}
	HAL_UART_Receive_IT(&huart2,BL_RX,3);
	
	HAL_TIM_Base_Start_IT(&htim3);
	HAL_TIM_Base_Start_IT(&htim4);
	HAL_TIM_Base_Start_IT(&htim5);
	int cnt=0;
	hhh=0;
	while	(1)
	{	Tv_A=0;Tv_B=0;
		OLED_ShowStr1(0, 3,BL_RX[0],2, 16);
		OLED_ShowStr1(40, 3,BL_RX[1],2, 16);
		OLED_ShowStr1(80, 3,BL_RX[2],2, 16);
		if(timu==0)
		{
			OLED_ShowStr(0,0,(uint8_t*)"wait",16);
		}
		if(timu==1)
		{
			OLED_ShowStr(0,0,(uint8_t*)"f111",16);
			HAL_Delay(1000);
			OLED_FullyClear();
			break;
		}
		if(timu==2)
		{
			OLED_ShowStr(0,0,(uint8_t*)"f222",16);
			HAL_Delay(1000);
			OLED_FullyClear();
			break;
		}
	}
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	
  while (1)
  {
		
		if (state == 1)    //串口1接收完毕,等待处理
    {
      width =Date[0];
      now = Date[1]; 
			color = Date[2];
      state = 0; 
    }										//
		if(state1 == 1)    //串口3接收完毕,等待处理
    {
			
			shuliang = Date1[0]; //数字个数
			if(shuliang == 1)
			{
				number[0]  = Date1[1];
				number[1] = 0;
				number[2] = 0;
				number[3] = 0;
						if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)==0) //按键检测
		{
			HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_10);
			T_num=number[0];
		} 
			}
			else if(shuliang >1)  //数字数量大于1
			{
				for(int i =0;i<shuliang;i++)                //依次储存各数字与各数字坐标
				{
					num_shu[i] = (Date1[2+i*3]<<8) + Date1[3+i*3];
					number[i] = Date1[1+i*3];
				}
				sort(number,num_shu,shuliang);               //根据数字坐标排序数字
				for(int i = 0;i<4-shuliang;i++)
				{
					number[3-i] = 0;                        //清空未赋值数字           
				}
				for(int i = 0;i<4;i++)
				{
					if(number[i] == 2)  
					{
					number[i]=7;
					}						
				}
			}	
			
			//开始病房
			if(back==0)
			{
				if (shuliang==1&&lukou==0)
				{
					if(T_num==1)
					{
						zhuanwan[0]='A';
					}
					else if(T_num==2)
					{
						zhuanwan[0]='B';
					}
//					else 
//						zhuanwan[0]=0;
				}
				
				//中端病房插口
				else if(shuliang==2&&lukou==1)
				{
					zhongbingfangnum[0]=number[0];
					zhongbingfangnum[1]=number[1];
					if(zhongbingfangnum[0]==T_num)
					{
						zhuanwan[1]='A';
					}
					else if(zhongbingfangnum[1]==T_num)
					{
						zhuanwan[1]='B';
					}
//					else
//						zhuanwan[1]=0;
				}
				
				
				//4病房岔口
				else if(shuliang==4&&lukou==2)
				{
					for_ru_num[0]=number[0];
					for_ru_num[1]=number[1];
					for_ru_num[2]=number[2];
					for_ru_num[3]=number[3];
					if(for_ru_num[0]==T_num||for_ru_num[1]==T_num)
					{
						zhuanwan[2]='A';
					}
					else if(for_ru_num[2]==T_num||for_ru_num[3]==T_num)
					{
						zhuanwan[2]='B';
					}
//					else
//						zhuanwan[2]=0;
				}
				
				//最后病房岔口
				else if(shuliang==2&&lukou==3)
				{
					moweibingfangnum[0]=number[0];
					moweibingfangnum[1]=number[1];
					if(moweibingfangnum[0]==T_num)
					{
						zhuanwan[3]='A';
					}
					else if(moweibingfangnum[1]==T_num)
					{
						zhuanwan[3]='B';
					}
//					else
//						zhuanwan[3]=0;
				}
				}				
				number[0] = 0;
				number[1] = 0;
				number[2] = 0;
				number[3] = 0;
      state1 = 0; 
			
    }										//
		
		
 
		
		if(timu==1)
		{
		while(zhuangtai01==-1&&HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_3)==0)
		{
		cnt++;
		HAL_Delay(10);
			if(cnt>150)
			{
			cnt=0;
			zhuangtai01=0;
			}
		}	
		while(zhuangtai01==2&&HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_3)==1)
		{
		cnt++;
		HAL_Delay(10);
			if(cnt>150)
			{
			cnt=0;
			zhuangtai01=3;
				led('F');
			}
		}
		if(color==0&&width>=65&&hhh==0&&(zhuangtai01==0||zhuangtai01==7))//岔路口检测函数
		{
			if(bibibi!=2)
			{
			set_jvli_A=z_A;
			set_jvli_B=z_B;
			if(back==0)
			lukou++;
			else
			lukou--;
			zhuangtai01=1;
			}
			else
				bibibi=0;
			hhh=1;//运行一次标志位
		}
		else if(color==0&&width<65&&(zhuangtai01==0||zhuangtai01==7))//标志位置0
		{	
			led('F');
		hhh=0;
		}
		
		if(zhuangtai01!=-1&&zhuangtai01!=-2&&zhuangtai01!=1&&color==1&&hhh==0)
		{
			set_jvli_A=z_A;
			set_jvli_B=z_B;
			if(back==0)
				zhuangtai01=2;
			else
				zhuangtai01=6;
			hhh=1;
		}
		
		
		}
		
		
		
		
		
		
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

		{
		OLED_ShowChar(0,3,zhongbingfangnum[0]+'0',16);
		OLED_ShowChar(16,3,zhongbingfangnum[1]+'0',16);
		OLED_ShowChar(32,3,0,16);
		OLED_ShowChar(48,3,for_ru_num[0]+'0',16);
		OLED_ShowChar(64,3,for_ru_num[1]+'0',16);
		OLED_ShowChar(80,3,for_ru_num[2]+'0',16);
		OLED_ShowChar(96,3,for_ru_num[3]+'0',16);
		OLED_ShowChar(0,5,zhuanwan[0],16);
		OLED_ShowChar(17,5,zhuanwan[1],16);
		OLED_ShowChar(34,5,zhuanwan[2],16);
		OLED_ShowChar(51,5,zhuanwan[3],16);
		OLED_ShowStr1(78, 5,T_num, 1, 16);
		OLED_ShowStr1(0, 0, width, 3, 16);
		OLED_ShowStr1(32, 0,HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_3),3, 16);
		OLED_ShowStr1(100, 0,lukou,2, 16);
		OLED_ShowStr1(64, 0,zhuangtai01,1, 16);
//OLED_ShowStr1(0, 0,(int)v_A,3, 16);	
//OLED_ShowStr1(64, 0,(int)v_B,3, 16);		
		}
  }
	
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */



void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	
  if(huart == &huart1)
  {
		uart_it_receive(&huart1,Date,&state);
  }
	
  
	else if(huart == &huart6)
  {
		uart_it_receive1(&huart6,Date1,&state1);
		
  }
	else if(huart == &huart2)
  {
		if(BL_RX[0]==0XF0&&BL_RX[2]==0X0F)
		{
			timu=BL_RX[1];
		}
		else if(BL_RX[0]==0XFA&&BL_RX[2]==0XAF)
		{
			if(BL_RX[1]==0XAA)
				{zhuanwan[1]='A';}
			else if(BL_RX[1]==0XBB)
				{zhuanwan[1]='B';}
			else if(BL_RX[1]==0X11)
			{zhuangtai01=-1;}
			else if(BL_RX[1]==0X22)
			{led('F');}
			else if(BL_RX[1]==0X33)
			{zhuangtai01=3;}
		}
		
		BL_RX[0]=0;
		BL_RX[1]=0;
		BL_RX[2]=0;
		HAL_UART_Receive_IT(&huart2,BL_RX,3);	
  }
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

  if (htim == &htim3)
  {
		if(v_A>=0)
    ceh_A++;   //tim1计数溢出
		else
			ceh_A--;
  }

  if (htim == &htim4)
  {
		if(v_B>=0)
    ceh_B++;  //tim8计数溢出
		else
			ceh_B--;
  }

	
  if (htim == &htim5)
  {
		
    motor_handle();   //速度pid控制  
		if(tick==1)
		{
			if(timu==1){
			if (zhuangtai01 == -1||zhuangtai01==-2)//初始停止状态包括未收到开始指令状态
      {
					Tv_A=0;
					Tv_B=0;
      }			
			else if (zhuangtai01 == 0||zhuangtai01==7)//巡线状态
      {
					line_pid(80,zhuangtai01);   		
      }		
			else if(zhuangtai01==1)//避让和回
			{
				if(bibibi==1)
				{
					if(zhuanwan[lukou-1]=='B')
					{
						if(z_A<set_jvli_A+700 && z_B<set_jvli_B+700 ){line_pid(80,zhuangtai01);}
					else 
					{
					get_T_distence(set_jvli_A+700,set_jvli_B+700+1700);
					distence_A_pid();
					distence_B_pid();
					if(distence_A_er*distence_A_er<10&&distence_B_er*distence_B_er<10)	
					{
					zhuangtai01=7;
					
					}						
					}
					
					}
					else if(zhuanwan[lukou-1]=='A')
					{
					if(z_A<set_jvli_A+700 && z_B<set_jvli_B+700 ){line_pid(80,zhuangtai01);}
					else 
					{
					get_T_distence(set_jvli_A+700+1700,set_jvli_B+700);
					distence_A_pid();
					distence_B_pid();
					if(distence_A_er*distence_A_er<10&&distence_B_er*distence_B_er<10)	
					{
					zhuangtai01=7;
					
					}						
					}
					}
					else 
						zhuangtai01 = 0;
				}
				else
				{
					if(zhuanwan[lukou]=='A')
					{
						if(hhhh==0&&z_A<set_jvli_A+700 && z_B<set_jvli_B+700 )
					{
					get_T_distence(set_jvli_A+700+1700,set_jvli_B+700);
					line_pid(80,zhuangtai01);
					}
					else
						hhhh=1;
					if(hhhh==1)
					{
						distence_A_pid();
						distence_B_pid();
						if(distence_A_er*distence_A_er<10&&distence_B_er*distence_B_er<10)	
						{
							hhhh=0;
						zhuangtai01 = 0;
						}
					}			
					}
					else if(zhuanwan[lukou]=='B')
					{
					if(hhhh==0&&z_A<set_jvli_A+700 && z_B<set_jvli_B+700 )
					{
					get_T_distence(set_jvli_A+700,set_jvli_B+700+1700);
					line_pid(80,zhuangtai01);
					}
					else
						hhhh=1;
					if(hhhh==1)
					{
						distence_A_pid();
						distence_B_pid();
						if(distence_A_er*distence_A_er<10&&distence_B_er*distence_B_er<10)	
						{
							hhhh=0;
						zhuangtai01 = 0;
						}
					}
						
					}
					else 
						zhuangtai01 = 0;
				}
			}
			
			
			else if(zhuangtai01==2||zhuangtai01==6)//任务点停止状态|任务结束状态
			{
			if(bibibi==1)
			{
			led('Y');
			}
			else
			{
			if(zhuangtai01==2)
			{
			led('R');
			}
			if(zhuangtai01==6)
			{
			led('G');
			}
			}
			get_T_distence(set_jvli_A,set_jvli_B);
			distence_A_pid();
			distence_B_pid();
			}
			else if(zhuangtai01==3)//避让结束转弯状态
			{
			if(hhhh==0)
				{
				set_jvli_A=z_A;
				set_jvli_B=z_B;
				hhhh=1;
				}
				else if(hhhh==1)
				{
				get_T_distence(set_jvli_A+1500,set_jvli_B+1500);
				distence_B_pid();
				distence_A_pid();
				if(distence_A_er*distence_A_er<10&&distence_B_er*distence_B_er<10)
					hhhh=2;
				}
				
				else if(hhhh==2)
				{
				get_T_distence(set_jvli_A+1500,set_jvli_B+1500-1700);
				distence_B_pid();
				distence_A_pid();
				if(distence_A_er*distence_A_er<10&&distence_B_er*distence_B_er<10)
					hhhh=3;
				}
				else if (hhhh==3)
				{
				distence_B_pid();	
				Tv_A=20;
				if(color==0)
				{	
				back=1;
				hhhh=0;
				zhuangtai01=0;
					bibibi=2;//
				}
				}
				
			}
			else if(zhuangtai01==4)//送药结束转弯状态
			{
			if(hhhh==0)
				{
				set_jvli_A=z_A;
				set_jvli_B=z_B;
				hhhh=1;
				}
				else if(hhhh==1)
				{
				get_T_distence(set_jvli_A+1500,set_jvli_B+1500);
				distence_B_pid();
				distence_A_pid();
				if(distence_A_er*distence_A_er<10&&distence_B_er*distence_B_er<10)
					hhhh=2;
				}
				
				else if(hhhh==2)
				{
				get_T_distence(set_jvli_A+1500,set_jvli_B+1500-1700);
				distence_B_pid();
				distence_A_pid();
				if(distence_A_er*distence_A_er<10&&distence_B_er*distence_B_er<10)
					hhhh=3;
				}
				else if (hhhh==3)
				{
				distence_B_pid();	
				Tv_A=20;
				if(color==0)
				{	
				back=1;
				hhhh=0;
				zhuangtai01=7;
					bibibi=0;//
				}
				}
				
			}
		}
			tick=0;
		}
		else
		{tick++;}
		}
}



int fputc(int ch, FILE *f)
{
	HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xffff);
	return ch;
}
/* USER CODE END 4 */

 /* MPU Configuration */

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x0;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.SubRegionDisable = 0x87;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
