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
#include "mpu.h"

I2C_HandleTypeDef I2cHandle;
SPI_HandleTypeDef SpiHandle;

static void SystemClock_Config(void);

static void Error_Handler(void);

int main(void)
{

	HAL_Init();

	//HAL_NVIC_EnableIRQ(I2C3_EV_IRQn);
	//HAL_NVIC_EnableIRQ(SPI3_IRQn);
	//HAL_NVIC_SetPriority(I2C3_EV_IRQn, 0, 1);
	//HAL_NVIC_SetPriority(SPI3_IRQn, 1, 1);

	  //HAL_NVIC_SetPriority(I2C1_ER_IRQn, 1, 0);
	  //HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);
	  //HAL_NVIC_SetPriority(I2C1_EV_IRQn, 2, 0);
	  //HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_SPI3_CLK_ENABLE();
	__HAL_RCC_I2C1_CLK_ENABLE();
	__HAL_RCC_I2C1_RELEASE_RESET();

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

	if(HAL_SPI_Init(&SpiHandle) != HAL_OK)
	{
	Error_Handler();
	}

	I2cHandle.Instance             = I2C1;
	//I2cHandle.Mode                 = HAL_I2C_MODE_MASTER;
	I2cHandle.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
	I2cHandle.Init.ClockSpeed      = 800000;
	//I2cHandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	I2cHandle.Init.DutyCycle       = I2C_DUTYCYCLE_16_9;
	//I2cHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	//I2cHandle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
	I2cHandle.Init.OwnAddress1     = 0x30F;				//0xd2 (if AD0 is set)
	I2cHandle.Init.OwnAddress2     = 0xFE;

	if(HAL_I2C_Init(&I2cHandle) != HAL_OK)
	{
	Error_Handler();
	}

	//gpio pins
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin =  GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_3;		//led, d/c, cs
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	//spi pins
	GPIO_InitStruct.Pin       = GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_PULLUP;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
	GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	//i2c pins
	GPIO_InitStruct.Pin       = GPIO_PIN_8 | GPIO_PIN_9;		//scl, sda
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
	//GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_PULLUP;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
	GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin =  GPIO_PIN_10 | GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    //cs
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7,1);
    //reset on display
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,1);
    //error led
	BSP_LED_Off(LED2);
	//ad0 on mpu
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,1);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,0);

	HAL_Delay(200);

	SSD1306Init();
	clearScreen();

	uint8_t string[] = "gx is: ";
	uint8_t numberstring[] = "000";
	uint8_t string2[] = "gy is: ";
	uint8_t numberstring2[] = "000";
	uint8_t string3[] = "mpu good is: ";
	uint8_t numberstring3[] = "000";

	int count = 0, count2 = 0, count3 = 0;

	int x = 64;
	int y = 32;
	int xprev, yprev;

	//stringDraw(1, 3, string, 1);
	//stringDraw(3, 3, string2, 1);
	//stringDraw(5, 3, string3, 1);

	uint8_t addr = 12;
	uint8_t data = 13;

	write_i2c(addr,data);
	count3 = verify_i2c();
	initialize_mpu();
	count3 = check_sleep();

	HAL_Delay(100);

	float gyrorates[3] = {0.0, 0.0, 0.0};
	int accels[3] = {0.0, 0.0, 0.0};

	while(1){

    	count =  (int)accels[0];
    	count2 = (int)accels[1];

    	//process the gyroscope output a bit
    	/*
    	if(count < 0){
    		count *= -1;
    	}
    	if(count < 1000){
    		numberstring[2] =  int2ascii(count%10);
			numberstring[1] =  int2ascii((count/10)%10);
			numberstring[0] =  int2ascii((count/100)%10);
    	}

    	if(count2 < 0){
    		count2 *= -1;
    	}
    	if(count2 < 1000){
			numberstring2[2] = int2ascii(count2%10);
			numberstring2[1] = int2ascii((count2/10)%10);
			numberstring2[0] = int2ascii((count2/100)%10);
    	}

		numberstring3[2] = int2ascii(count3%10);
		numberstring3[1] = int2ascii((count3/10)%10);
		numberstring3[0] = int2ascii((count3/100)%10);
		*/

		xprev = x;
		yprev = y;

		x += count/1000;
		y += count2/1000;

		if(x > 119){
			x = 119;
		}
		if(x < 5){
			x = 5;
		}
		if(y > 58){
			y = 58;
		}
		if(y < 5){
			y = 5;
		}


		circleDraw(x,y,5,1);

		//update the screen
		//stringDraw(1, 14, numberstring, 1);
		//stringDraw(3, 14, numberstring2, 1);
		//stringDraw(5, 16, numberstring3, 1);

		HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_3);
	    BSP_LED_Toggle(LED2);

		get_gyro_rates(gyrorates);
		get_accel_values(accels);

		HAL_Delay(5);

		//clear the screen
		//stringDraw(1, 14, numberstring,  0);
		//stringDraw(3, 14, numberstring2, 0);
		//stringDraw(5, 16, numberstring3, 0);
		circleDraw(xprev,yprev,5,0);

	}
}


static void Error_Handler(void)
{
  while(1)
  {
    BSP_LED_Toggle(LED2);
    HAL_Delay(80);
  }
}


void SSD1306SendCommand(uint8_t *data, int i){
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7,1);		  //cs
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6,0);       //data/command low for command.
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7,0);
    while(i){
		if(HAL_SPI_Transmit(&SpiHandle, data, 1, HAL_MAX_DELAY) != HAL_OK){
			Error_Handler();
		}
        data++;
        i--;
    }
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7,1);
}

void SSD1306SendData(uint8_t *data, int i){
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7,1);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6,1);       //data/command high for data.
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7,0);
    while(i){
		if(HAL_SPI_Transmit(&SpiHandle, data, 1, HAL_MAX_DELAY) != HAL_OK){
			Error_Handler();
		}
        data++;
        i--;
    }
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7,1);
}


int read_i2c(uint8_t address){

	uint8_t* addrptr = &address;

	uint8_t data = 0x00;
	uint8_t* dataptr = &data;

	while (HAL_I2C_GetState(&I2cHandle) != HAL_I2C_STATE_READY){};

	//send the address you want to read from
	if(HAL_I2C_Master_Transmit(&I2cHandle, (uint8_t)MPU6050_ADDRESS, (uint8_t*)addrptr, 1, 500) != HAL_OK){
	  Error_Handler();
	}

	while (HAL_I2C_GetState(&I2cHandle) != HAL_I2C_STATE_READY){};

	HAL_I2C_Master_Receive(&I2cHandle, (uint8_t)MPU6050_ADDRESS, (uint8_t*)dataptr, 1, 500);

	return (int)*dataptr;

}


void write_i2c(uint8_t address, uint8_t data){

	uint8_t addranddata[2] = {address,data};

	while (HAL_I2C_GetState(&I2cHandle) != HAL_I2C_STATE_READY){};

	if(HAL_I2C_Master_Transmit(&I2cHandle, (uint8_t)MPU6050_ADDRESS, (uint8_t*)addranddata, 2, 500) != HAL_OK){
	  Error_Handler();
	}

	while (HAL_I2C_GetState(&I2cHandle) != HAL_I2C_STATE_READY){};

}

void I2C1_EV_IRQHandler(void)
{
  HAL_I2C_EV_IRQHandler(& I2cHandle);
}

/*
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);


  HAL_PWREx_EnableOverDrive();

  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}
  */


void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSI Oscillator and activate PLL with HSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 0x10;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  RCC_OscInitStruct.PLL.PLLR = 6;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

   /* Activate the OverDrive to reach the 180 MHz Frequency */
  ret = HAL_PWREx_EnableOverDrive();
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}
