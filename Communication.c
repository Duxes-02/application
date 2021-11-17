#include <stdio.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include <string.h>
#include <stdlib.h>
/*
 * Communication.c
 *Initiating Communication between PC and MCU
    Comm_State_Machine() ;

    HAL_Delay(200) ;
 *  Created on: 16-Oct-2021
 *      Author: PARAM
 */

UART_HandleTypeDef huart5;

// Wait for the RESET SWITCH.
	 // Initiate communication with PC.
	 // Get configuration Data from PC.
	 // Send Sensor Data to PC.
 	 // Process configuration Data and Add in buffer.
	 // Get the Sensor/Result Data from MCU .

	  //WAIT_RESET
	  //COMM_INIT_PC
	  //GET_CONFIG_DATA_PC
	  //SEND_SENSOR_DATA_PC
	  //CONFIG_DATA_PROC
	  //SENSOR_RESULT_MCU
	  //IDLE_STATE
typedef enum COMM_STATE
	{
		 WAIT_RESET = 1 ,
		 COMM_INIT_PC ,
		 GET_CONFIG_DATA_PC ,
		 SEND_SENSOR_DATA_PC ,
		 CONFIG_DATA_PROC ,
		 SENSOR_RESULT_MCU ,
		 IDLE_STATE ,
	} COM_STATE;


uint32_t T_Var3 = 1 ;
uint32_t T_Var1 = 1 ;
uint32_t T_Var = 1 ;
uint32_t CHECK , COUNT ;
static uint32_t P_Buf = 0 ;
bool Temp_Var = true ;
uint32_t  UR2 , UT1_STATUS , UR_STATUS , UT_STATUS ;
uint32_t STATE = WAIT_RESET ;
uint32_t Button_Status ;
char Tx_OK[5] = "OK_1\n" ;
char Module_ID[10] = "FE141EF\n" ;
char Send_Data[4] ;
char Ack_C[2] = "C\n";
char Ack_Y[2] = "Y\n" ;
char Ack_D[2] = "D\n" ;
char Ack_Z[2] = "Z\n" ;
char PARAM_CONFIG[30] ;
char D_Tx[3] = "OK\n" ;
char D_Rx_1[55]  ;
uint8_t G_RST_C ;
char CALC[4] ;

uint32_t Str_Len , FLAG ;
uint32_t i , j , c , E ;
uint32_t D_P[10] ;
uint32_t Data_Proc[10] ;
uint32_t Config_Data[10] ;
uint32_t L_Counter ;
int p ;
char DC[2] ;
char DC_1[4] ;
int l , k ;

void STR_SEP(char Con_Data[])  //022040608501254567890123459789
{
	//Calculate String length
	l = strlen(Con_Data) - 1 ;

	//Converting Total 5 bytes to Config_Data[5] Array
	for(i = 0 ; i < 5 ; i++)
	{
		//Segregating the 1 byte values from the Sequence
		while(j < 2)
		{
			DC[j] = Con_Data[c] ;
			j++ ;
			c++ ;
		}
		//Converting string to integer
		k = atoi(DC) ;

		//Putting values to Config_Data[]
		Config_Data[p] = k;

		//Incrementing Array Address
		p++ ;

		//Resetting counter value to Start the count again
		j = 0 ;
	}

	//Converting Total 2 bytes to Config_Data[10] Array.
	for( ; i < 10 ; i++)
	{
		//Segregating the 2 byte values from the Sequence.
		while(j < 4)
		{
			DC_1[j] = Con_Data[c] ;
			j++ ;
			c++ ;
		}
		//Converting string to integer
		k = atoi(DC_1) ;

		////Putting values to Config_Data[]
		Config_Data[p] = k;

		//Incrementing Array Address
		p++ ;

		//Resetting counter value to Start the count again
		j = 0 ;
	}
}

//Searching the DATA in UART buffer
uint32_t SRCH_ELEMT(char D_Rx[])
{
	//Search for 1st Acknowledgement
	while(D_Rx[P_Buf] != '\0')
	{
		HAL_Delay(800) ;
		if (D_Rx[P_Buf] == 'F' && D_Rx[P_Buf + 1] == 'E' && D_Rx[P_Buf + 2] == 'A')
		{
			P_Buf++ ;
			HAL_Delay(800) ;
			UT1_STATUS = HAL_UART_Transmit(&huart5, (uint8_t *)Send_Data, 4 , 10) ;
			HAL_Delay(300) ;

			return 1 ;
		}
		else
		{
			if(D_Rx[P_Buf] == 'F' && D_Rx[P_Buf + 1] == 'E' && D_Rx[P_Buf + 2] == 'W')
			{
			   // Counter if Acknowledgment received is 'W'
	  	  	   G_RST_C++ ;
	  	  	   P_Buf++ ;

	  	  	   return 2 ;
			}
			else
			{
				if(D_Rx[P_Buf] == 'B')
				{
					//Resends Acknowledgment upon receiving 'B'
					HAL_UART_Transmit(&huart5, (uint8_t *)Ack_C, 1 , 25) ;

					P_Buf++ ;



					return 3 ;
				}
				else
				{
					if(D_Rx[P_Buf] == 'X')
					{
						//Resends Data 3 times upon receiving 'X'
						HAL_UART_Transmit(&huart5, (uint8_t *)Send_Data, 4 , 10) ;
						HAL_UART_Transmit(&huart5, (uint8_t *)Send_Data, 4 , 10) ;
						HAL_UART_Transmit(&huart5, (uint8_t *)Send_Data, 4 , 10) ;

						P_Buf++ ;

						return 4 ;

					}
				}

			}

		}
		P_Buf++ ;
	}
  return 0 ;
}


uint32_t SRCH_CONFIG(char D_Rx[])
{
	//Calculate the String size
	while(D_Rx[P_Buf] != '\0')
	{
		//HAL_Delay(300) ;
		PARAM_CONFIG[COUNT] = D_Rx[P_Buf] ;
		COUNT++ ;
		P_Buf++ ;
	}
	//Check if Received String Length is 30
	if(COUNT == 30)
	{
		// Store the fetched Data (Configuration Parameters) to MCU Memory
		//strcpy(PARAM_CONFIG, D_Rx) ;


		//Send the appropriate Acknowledgment upon receiving the Configuration Parameters .
		HAL_UART_Transmit(&huart5, (uint8_t *)Ack_D, 2 , 10) ;

		return 1 ;
	}
	else
	{
		// Send appropriate Acknowledgment to Docklight if Configuration Parameters is not received properly.
		HAL_UART_Transmit(&huart5, (uint8_t *)Ack_Z, 2 , 10) ;

		return 2 ;
	}
}

//Initiating the Communication Between PC and MCU
void Comm_State_Machine(void)
{


	  //Traverse through different State machines as required
	  switch(STATE)
	  {
	  	  	  	  case 1:
	  	  	  		  	  //WAIT_RESET ;
	  	  	  		  	  //Read the Reset state through GPIO(UB_ST).
	  	  	  		  	  // Press the GPIO_BUTTON for RESET STATE
	  	  	  		  	  //RST_SWTICH = HAL_GPIO_ReadPin(GPIOA, GPIO_Pin_0) ;// RESET SWITCH
	  	  	  		  	  Button_Status = HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_7) ;
	  	  	  		  	  if ( Button_Status && Temp_Var )// change to while loop
	  	  	  		  	  {
	  	  	  		  		  // Wait for Debounce Effect
	  	  	  		  		  HAL_Delay(5) ;

	  	  	  		  		  //Changing the STATE to next State.
	  	  	  		  		  STATE = COMM_INIT_PC ;  //HAL_GPIO_WritePin(GPIOD, LED_1_Pin, 1) ;

	  	  	  		  		  Temp_Var = false ;
	  	  	  		  }
	  	  	  		  else
	  	  	  		  {
	  	  	  		    if(Button_Status && Temp_Var )
	  	  	  		  	{
	  	  	  		  	  	//Wait for debouncing effect
	  	  	  		  	   	HAL_Delay(5) ;


	  	  	  		  	   //Change Temporary_Variable Status
	  	  	  		  	  	Temp_Var = true ;

	  	  	  		  	  	//Staying in the same Machine
	  	  	  		  	  	STATE = WAIT_RESET ;
	  	  	  		  	}
	  	  	  		  }


	  	  	  		  	  break;
	  	  	  	  case 2 :
	  	  	  		  	  //Initiate communication with PC.
	  	  	  	          UT_STATUS = HAL_UART_Transmit(&huart5, (uint8_t *)Module_ID, 10 , 20) ;

	  	  	  		      //// Send valid Module ID Data to Docklight
	  	  	  		   	  while(T_Var)    //(!(UT_STATUS == HAL_OK) &&
	  	  	  		   	  {
	  	  	  		   		  UR_STATUS = HAL_UART_Receive_DMA(&huart5, (uint8_t *)D_Rx_1, 55) ;
	  	  	  		   		  if(UR_STATUS == HAL_OK)  //|| UR1 != HAL_TIMEOUT)
	  	  	  		   		  {
	  	  	  		   			  T_Var = 0 ;
	  	  	  		   		  }
	  	  	  		   		  HAL_Delay(500) ;
	  	  	  		   		  CHECK = SRCH_ELEMT(D_Rx_1) ;
	  	  	  		   		  if(CHECK == 1)
	  	  	  		   		  {
	  	  	  		   			  HAL_UART_Transmit(&huart5, (uint8_t *)Tx_OK, 5 , 20) ;

	  	  	  		   			  HAL_Delay(300) ;

	  	  	  		   			  T_Var = 1 ;
	  	  	  		   		  }
	  	  	  		   		  else
	  	  	  		   		  {
                                 if (CHECK == 2)
                                 {
                                	// Counter if Acknowledgment received is 'W'
                                	 G_RST_C++ ;

                                	 T_Var = 1 ;


                                	 if(G_RST_C == 3)
                                	 {
                                		 //Staying in the same Machine
                                		 STATE = WAIT_RESET ;

                                		 //Break the loop
                                		 break ;
                                	 }
                                 }
                                 else
                                 {
                                	 if(CHECK == 0)
                                	 {
                                		 T_Var = 0 ;
                                	 }
                                 }
	  	  	  		   		  }
	  	  	  		   	  }
	  	  	  		   	  while(T_Var1) //(!(UT1_STATUS == HAL_OK) &&
	  	  	  		   	  {
	  	  	  		   		  UR2 = HAL_UART_Receive_DMA(&huart5, (uint8_t *)D_Rx_1, 55) ;
	  	  	  		   		  if(UR2 == HAL_OK)  // (== HAL_OK || UR2 != HAL_TIMEOUT)
	  	  	  		   		  {
	  	  	  		   			  T_Var1 = 0 ;
	  	  	  		   		  }
	  	  	  		   		  CHECK = SRCH_ELEMT(D_Rx_1) ;
	  	  	  		   		  if(CHECK == 3)
	  	  	  		   		  {
	  	  	  		   			  //HAL_UART_Transmit(&huart5, (uint8_t *)Tx_OK, 5 , 10) ;

	  	  	  		   			  HAL_Delay(300) ;

	  	  	  		   			  // Change the STATE to next STATE to receive Configuration Data from PC.
	  	  	  		   			  STATE = GET_CONFIG_DATA_PC ;

	  	  	  		   			  HAL_Delay(1000) ;

	  	  	  		   			  break ;
	  	  	  		   		  }
	  	  	  		   		  else
	  	  	  		   		  {
	  	  	  		   			  if (CHECK == 4)
	  	  	  		   			  {
	  	  	  		   				  T_Var1 = 1 ;
	  	  	  		   			  }
	  	  	  		   		  }
	  	  	  		   	   }



	  	  	  	  case 3 :
	  	  	  		  	  //Send Sensor Data to PC.
	  	  	  		  	 //SEND_SENSOR_DATA_PC
	  	  	  		  	 //Send SENSOR DATA to PC

	  	  	  		  	  while(T_Var3) //(!(UT1_STATUS == HAL_OK) &&
	  	  	  		  	  {
	  	  	  		  		  UR2 = HAL_UART_Receive_DMA(&huart5, (uint8_t *)D_Rx_1, 55) ;
	  	  	  		  		  if(UR2 == HAL_OK)  // (== HAL_OK || UR2 != HAL_TIMEOUT)
	  	  	  		  		  {
	  	  	  		  			  T_Var3 = 0 ;
	  	  	  		  		  }
	  	  	  		  		  FLAG = SRCH_CONFIG(D_Rx_1) ;
	  	  	  		  		  if(FLAG == 1)
	  	  	  		  		  {
	  	  	  		  			  //Change the State Machine
	  	  	  		  			  STATE = SEND_SENSOR_DATA_PC ;
	  	  	  		  			  break;
	  	  	  		  		  }
	  	  	  		  		  else
	  	  	  		  		  {
	  	  	  		  			  if(FLAG == 2)
	  	  	  		  			  {
	  	  	  		  				  T_Var3 = 1 ;

	  	  	  		  				  break;
	  	  	  		  			  }
	  	  	  		  		  }
	  	  	  		  	  }
	  	  	  	  case 4 :
	  	  	  		  	  	 // Process configuration Data and Add in buffer.
	  	  	  		  	   //Convert the received Timing Sequence to proper order
	  	  	  		  	   STR_SEP(PARAM_CONFIG) ;


	  	  	  		  	  //Change the State Machine
	  	  	  		  	  STATE = CONFIG_DATA_PROC  ;
	  	  	  		  	  break;
	  	  	  	  case 5 :
	  	  	  		  	   STATE = WAIT_RESET ;
	  	  	  		  	  break;
	  	  	  	  case 6 :
	  	  	  		  	  //Fetching the Sensor RESULTS from MCU .
	  	  	  		  	  break;
	  	  	  	  default :
	  	  	  		  	  //printf("WRONG INPUT")
	  	  	  		  	  //HAL_Delay(100) ;
	  	  	  		  	  break ;
	  }
}
