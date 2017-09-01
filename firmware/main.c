/** @file
  * @brief 	VESNA Driver Development and Demonstration
  * @author Marko Mihelin
  *
  * - CMSIS/stm32...c clock is set to internal HSI by default
  */

#include <stdio.h>

#include "vsn.h"
#include "vsnsetup.h"
#include "vsntime.h"
#include "vsnpm.h"
#include "vsnledind.h"
#include "vsnusart.h"
#include "vsndriversconf.h"
#include "vsnsht21.h"


/* Global variables ----------------------------------------------------- */
/* Main function prototypes --------------------------------------------- */

/* Start of main -------------------------------------------------------- */
int main(void)
{
	/* Main local variables --------------------------------------------- */
	USART_InitTypeDef USART_InitStructure;
	float sht21_temp,sht21_rh;
	int status;
	/* End of main local variables -------------------------------------- */

	/* Start of Main code ----------------------------------------------- */
	/* Reset Clock configuration */
	SystemInit();
	/* Turn on power manager */
	vsnPM_init();
	/* Set SysClock frequency */
	vsnSetup_intClk(SNC_CLOCK_8MHZ);
    /* Initialize SNC */
	vsnSetup_initSnc();
	/* Calibrate the HSI clock source */
	vsnSetup_calibHsi();
	/* Configure debug port at USART1, 115200 kbaud, 8 data bit, no parity, one stop bit, no flow control */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	vsnUSART_init(USART1, &USART_InitStructure);

	printf("SNC Initialized\n");
	printf("Debug port Open\n");

	/* Get ADC one bit value in volts, call as last init function */
	vsnPM_mesureAdcBitVolt();

	/* Init functions for sensors, actuators,... ------------------------ */
	/* End of init functions -------------------------------------------- */
	vsnI2C_init(VSN_I2C1_REMAPPED,VSN_I2C_MASTER,0x01, 10000);
	printf("ERROR numbers:\n"
			"I2C_SUCCESS = 0,\n"
			"I2C_BUS_BUSY = 1,\n"
			"I2C_NO_DEVICE = 2,\n"
			"I2C_COM_IN_PROGRESS = 3,\n"
			"I2C_IDLE = 4,\n"
			"I2C_INCORRECT_SETUP_DATA = 5,\n"
			"I2C_TIMEOUT_RX = 6,\n"
			"I2C_TIMEOUT_STOP = 7,\n"
			"I2C_TIMEOUT_DMA = 8,\n"
			"I2C_NOT_IN_COPATIBLE_MODE = 9,\n"
			"I2C_BUS_ERROR = 10\n");
	/* Start of main while ---------------------------------------------- */
	while(1)
	{
		// TMP75 have 8 possible device address
		status = vsnSHT21_MeasureTandRH(VSN_I2C1_REMAPPED, &sht21_temp,&sht21_rh);
		if(status != 0){
			printf("read error = %d \n", status);
		}
		printf("temperature = %f C\r\n RH = %f %%\n", sht21_temp,sht21_rh);
		vsnTime_delayMS(600);
	} /* End of main while */
} /* End of main */

