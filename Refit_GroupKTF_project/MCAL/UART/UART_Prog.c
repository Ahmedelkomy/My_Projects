/*
 * UART_Prog.c
 *
 *  Created on: ???/???/????
 *      Author: Ahmed
 */

/*--------------------------------------------------------------------------------------------*/
/*   INCLUDES   */
/*--------------------------------------------------------------------------------------------*/
#include "UART_Config.h"
#include "UART_Interface.h"
#include "UART_Private.h"

/*--------------------------------------------------------------------------------------------*/
/*   FUNCTION BODY   */
/*--------------------------------------------------------------------------------------------*/
void M_UART_Void_UARTInit(void)
{
	M_DIO_Void_SetPinDirection(PD0_PIN,INPUT);
	M_DIO_Void_SetPinDirection(PD1_PIN,OUTPUT);
	/* TO SELECT BAUD RATE -> 9600 */
	UBRRL_REG = ((F_OSC * 1000000) / ((u32)16 * UART_BAUD_RATE)) - 1 ;
#if   UART_PARITY_MODE   ==   PARITY_DISABLE
	CLR_BIT(UCSRC_REG,UPM0_BIT);
	CLR_BIT(UCSRC_REG,UPM1_BIT);
#elif UART_PARITY_MODE   ==   PARITY_EVEN
	CLR_BIT(UCSRC_REG,UPM0_BIT);
	SET_BIT(UCSRC_REG,UPM1_BIT);
#elif UART_PARITY_MODE   ==   PARITY_ODD
	SET_BIT(UCSRC_REG,UPM0_BIT);
	SET_BIT(UCSRC_REG,UPM1_BIT);
#endif
#if   UART_STOP_BIT   ==   1
	CLR_BIT(UCSRC_REG,USBS_BIT);
#elif UART_STOP_BIT   ==   2
	SET_BIT(UCSRC_REG,USBS_BIT);
#endif
	/* TO SELECT CHARACTER SIZE -> 8 BITS */
	CLR_BIT(UCSRB_REG,UCSZ2_BIT);
	SET_BIT(UCSRC_REG,UCSZ1_BIT);
	SET_BIT(UCSRC_REG,UCSZ0_BIT);

	/* TO ENABLE RX CIRCUIT */
	SET_BIT(UCSRB_REG,RXEN_BIT);
	/* TO ENABLE TX CIRCUIT */
	SET_BIT(UCSRB_REG,TXEN_BIT);
}

/*--------------------------------------------------------------------------------------------*/
/*   FUNCTION BODY   */
/*--------------------------------------------------------------------------------------------*/
void M_UART_Void_UARTWrite(u8 Copy_U8_Data)
{
	u32 Local_U32_Counter = 0;
	UDR_REG = Copy_U8_Data;
	while(Local_U32_Counter < UART_POLLING_TIME)
	{
		Local_U32_Counter++;
		if(GET_BIT(UCSRA_REG,TXC_BIT) == UART_FINISHED_TRANSMITTING)
			break;
	}
}

/*--------------------------------------------------------------------------------------------*/
/*   FUNCTION BODY   */
/*--------------------------------------------------------------------------------------------*/
u8   M_UART_U8_UARTRead(void)
{
	while(GET_BIT(UCSRA_REG,RXC_BIT) == 0);
	return UDR_REG;
}
