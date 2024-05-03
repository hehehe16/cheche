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
#include "OLED.h"
#include "motor.h"
#include "control.h"
#include "uart_it.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
uint8_t angle=150;   //90  120

uint8_t state = 0;   //串口状�??

uint8_t Date[10] = {0}; //接收数组

uint8_t state1 = 0;   //串口状�??

uint8_t Date1[20] = {0}; //接收数组

uint16_t shuliang = 0;   //数字个数

uint16_t number[4] = {0};

uint8_t tick = 0;      
uint8_t ticks =0;



uint8_t rx_v831[6]={0};
uint8_t jiaoyan;
uint8_t num_A=1,num_B=2;
uint8_t rx_n_num=0;
uint8_t sig_num=0;


uint8_t fangjian_zhong[2]={0};
uint8_t fangjianA[2]={0};
uint8_t fangjianB[2]={0};
uint8_t fangjianA_AB[2]={0};
uint8_t fangjianB_AB[2]={0};
int chakou=0,fchakou=0;
uint8_t ji=1;
uint8_t T_num=0;

uint64_t st=0,ff_s=0,f_st=0;
uint8_t zhuangtai=1;//1��ȡ 0�ǻ�
char zhuanwan[4]={0}; 
int ttttt=0;
int statue = 0; //车的行进阶段 0：前往 1：返回

char direction = 1;  //运动状态  0：正常行进 1：停止 ‘L’：左转90度，然后正常行进 ‘R’：   
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
  MX_USART1_UART_Init();
  MX_I2C2_Init();
  MX_TIM2_Init();
  MX_TIM8_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USART3_UART_Init();
  MX_TIM15_Init();
  /* USER CODE BEGIN 2 */
  OLED_Init();
  OLED_FullyClear();
  motor_init();
	int i=0;
	
	int fork = 0; //第几个岔口
	int ex_fork =0; //期望岔口
	int ex_room =0;  //期望房间，1：左，2：右
	int color =0; //当前胶带颜色
	
	int v = 0;
	int vv = 0;
	int vvv=0;
	int vvvv = HAL_GetTick();
	int vvvvv = HAL_GetTick();
	int vvvvvv = HAL_GetTick();
	for(i=0;i<10;i++)
	{
  uart_it_init(&huart1,1);
	uart_it_init(&huart3,1);
	HAL_Delay(10);
	}
	__HAL_TIM_SetCompare(&htim15,TIM_CHANNEL_1,angle+50);
	HAL_TIM_PWM_Start(&htim15,TIM_CHANNEL_1);
  // dianjiA(1000);
  // dianjiB(1500);


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
			}
			else if(shuliang == 2)
			{
				if(Date1[2]<Date1[4]) //如果1号数字x小于2号数字
				{
					number[0]  = Date1[1];
					number[1]  = Date1[3];
				}
				else
				{
					
					number[0]  = Date1[3];
					number[1]  = Date1[1];
				}
			}  
			else
			{
				number[0] = 0;
			}
      state1 = 0; 
    }										//
		
		
		while(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_11)==0) //按键检测
		{
			T_num=number[0];
		}                                            //
		
		if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_3)==0)			//红外检测
		{
			if(HAL_GetTick()-ff_s>1000&&vv==0)
			{
				vv=1;
					direction = 0;																///判定成功
			}
		}
		else
		{
			ff_s=HAL_GetTick();
		}																					//
		
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		if(T_num ==1) 
		{
			ex_fork = 1;
			ex_room =1;
		}
		if(T_num ==2) 
		{
			ex_fork =1;
			ex_room =2;
		}
		
		if(shuliang!=0&&statue == 0&&direction == 0&&T_num !=1&&T_num !=2&&(vvvvv-HAL_GetTick())>2000)  //如果数字数量不为0  且为前往状态  
		{
			vvvvv = HAL_GetTick();
			if(number[0] == T_num)
			{
					ex_fork=fork +1;  //期望岔口为下一个
  				ex_room=1;
			}
			else if(number[1] == T_num)
			{
					ex_fork=fork +1;  //期望岔口为下一个
					ex_room=2;
			}			
			direction = 0;
//			direction = 1;  //先停下来
//				Tv_A = 0;
//        Tv_B = 0;
//			HAL_Delay(1000);
//			if(number[0] == T_num)  //左边数字
//			{
//				ex_fork++;  //期望岔口为下一个
//				ex_room=1;
//			}
//			else
//			{
//				__HAL_TIM_SetCompare(&htim15,TIM_CHANNEL_1,140); //转到另一边
//				HAL_Delay(1000);
//				if(Date1[1] ==T_num)
//				{
//					ex_fork++;  //期望岔口为下一个
//					ex_room=2;
//				}
//				
//				
//			}
		}
		
		
		
		
		
		if(width >100&&statue == 0&&color ==0)		//如果前往过程到达岔口
		{
			
			direction = 1;
			if((HAL_GetTick()-vvvv)>1000)
			{
				vvvv =HAL_GetTick();
				
				fork++;        ///当前岔口加一
			}
			
			

			if(fork == ex_fork)  ///如果是期望岔口
			{
				if(ex_room ==1)
				{
					direction = 'L';  
				}
				if(ex_room == 2)
				{
					direction = 'R';
				}
			}
		}									//
		
		if(width >100&&statue == 1&&color ==0&&(HAL_GetTick() - vvvvvv)>2000)   //如果返回过程到达岔口
		{
			vvvvvv = HAL_GetTick();
			if(ex_fork==3&&ex_fork==4||fork==ex_fork)
			{
				if(ex_room ==1)
				{
					direction = 'L';  
				}
				if(ex_room == 2)
				{
					direction = 'R';
				}				
			}
			
			fork--;

		}
		
		if(color == 1)  //如果到达终点
		{
			direction =1;
			
			if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_3)!=0) //如果拿开药
			{
				statue = 1;  //返回状态
				direction = 2;  //倒退
			}
		}               //
		
		
		
		
		



		

		
		
		
		


		OLED_ShowStr1(0, 3, fork, 4, 16);
		
    OLED_ShowStr1(0, 1, number[0], 4, 16);
	
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
	
  
	else if(huart == &huart3)
  {
		uart_it_receive1(&huart3,Date1,&state1);
  }
}



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

  if (htim == &htim2)
  {
    ceh_A++;   //tim1计数溢出
  }

  if (htim == &htim8)
  {
    ceh_B++;  //tim8计数溢出
  }

	
  if (htim == &htim4)
  {
		
    motor_handle();   //速度pid控制

    
    if (tick == 5)
    {
      if (direction == 0)
      {
				
					line_pid(100, &Tv_A, &Tv_B);   		
      }
      
			else if(direction ==1)
			{
				Tv_A = 0;
        Tv_B = 0;
				
			}
			else if(direction ==2)
			{
				Tv_A = -40;
        Tv_B = -40;
			}
			else
      {
        turn(direction);    //转向
        
        if(ticks >= 70)      //转向时间
        {
          direction = 0;
          ticks = 0;
        }
        else
        {
          ticks++;
        }
      }
    }
    else
    {
      tick++;
    }
  }
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
