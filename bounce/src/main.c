/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f4xx.h"
#include "stm32f4xx_nucleo.h"
#include "ssd1306.h"
#include "mpu_defines.h"

I2C_HandleTypeDef I2cHandle;
SPI_HandleTypeDef SpiHandle;

static void SystemClock_Config(void);

uint8_t txdata;
uint8_t rxdata;

//for i2c
static void Error_Handler(void);

int main(void)
{

	HAL_Init();

	HAL_NVIC_SetPriority(SPI3_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(SPI3_IRQn);

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_SPI3_CLK_ENABLE();

	SystemClock_Config();

	SpiHandle.Instance               = SPI3;
	SpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
	SpiHandle.Init.Direction         = SPI_DIRECTION_2LINES;
	SpiHandle.Init.CLKPhase          = SPI_PHASE_1EDGE;
	SpiHandle.Init.CLKPolarity       = SPI_POLARITY_HIGH;
	SpiHandle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
	SpiHandle.Init.CRCPolynomial     = 7;
	SpiHandle.Init.DataSize          = SPI_DATASIZE_8BIT;
	SpiHandle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
	SpiHandle.Init.NSS               = SPI_NSS_SOFT;
	SpiHandle.Init.TIMode            = SPI_TIMODE_DISABLE;
	SpiHandle.Init.Mode = SPI_MODE_MASTER;

	I2cHandle.Instance             = I2C1;
	I2cHandle.Init.AddressingMode  = I2C_ADDRESSINGMODE_10BIT;
	I2cHandle.Init.ClockSpeed      = 400000;
	I2cHandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	I2cHandle.Init.DutyCycle       = I2C_DUTYCYCLE_16_9;
	I2cHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	I2cHandle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
	I2cHandle.Init.OwnAddress1     = MPU6050_ADDRESS;
	I2cHandle.Init.OwnAddress2     = 0xFE;

	if(HAL_SPI_Init(&SpiHandle) != HAL_OK)
	{
	/* Initialization Error */
	Error_Handler();
	}

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin =  GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin =  GPIO_PIN_9 | GPIO_PIN_8 | GPIO_PIN_10;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin       = GPIO_PIN_10;
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_PULLUP;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
	GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_11;
	GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_12;
	GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	txdata = 0;
	rxdata = 0;

	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,0);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9,1);
    BSP_LED_Off(LED2);

	HAL_Delay(100);

	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,1);

	HAL_Delay(200);

	SSD1306Init();
	clearScreen();

	float x = 12.0;
	float y = 12.0;
	float xprev = x;
	float yprev = y;
    int clearcount = 0;

    float yaccel = 0.4;
    float xdamp =  0.99;
    float xvel =   3.0;
    float yvel =   0.0;
    int led_on = 0;

    BSP_LED_Toggle(LED2);

	while(1){

		//clearScreen();
		circleClear(xprev, yprev, 5);

		yvel += yaccel;
		xvel *= xdamp;
		x += xvel;
		y += yvel;
		clearcount++;

		//collision detection
		if(x >= 105 || x <= 12){
			xvel *= -1.0;
		}
		if(y >= 50 || y <= 12){
			yvel *= -1.0;
			if(y > 50){
				y = 50.0;
			}
		}

		//led trigger stuff
		if(y > 48 && led_on == 0){
			BSP_LED_On(LED2);
			led_on = 1;
		}
		if(y < 48){
			led_on = 0;
		}

		//damping and restarting position and velocity every once in a while
		if(xvel < 0.1 && xvel > -0.1){
			y = 12.0;
			if(xvel > 0){
				xvel = 3.0;
			}else{
				xvel = -3.0;
			}
		    yvel = 0.0;
		}

		circleDraw((int)x, (int)y, 5);

		xprev = x;
		yprev = y;
		BSP_LED_Off(LED2);
	}
}


static void Error_Handler(void)
{
  while(1)
  {
    BSP_LED_Toggle(LED2);
    HAL_Delay(40);
  }
}


void SSD1306SendCommand(uint8_t *data, int i){
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9,1);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8,0);       //data/command low for command.
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9,0);
    while(i){
		if(HAL_SPI_Transmit(&SpiHandle, data, 1, HAL_MAX_DELAY) != HAL_OK){
			Error_Handler();
		}
        data++;
        i--;
    }
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9,1);
}

void SSD1306SendData(uint8_t *data, int i){
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9,1);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8,1);       //data/command high for data.
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9,0);
    while(i){
		if(HAL_SPI_Transmit(&SpiHandle, data, 1, HAL_MAX_DELAY) != HAL_OK){
			Error_Handler();
		}
        data++;
        i--;
    }
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9,1);
}


static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  /* Activate the Over-Drive mode */
  HAL_PWREx_EnableOverDrive();

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}
