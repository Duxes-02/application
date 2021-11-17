/*
 * application.c
 *
 *  Created on: Oct 18, 2021
 *      Author: vinay hatti
 */


#include "main.h"
#include "application.h"
#include "stm32f4xx_hal.h"
#include "stdio.h"
#include "string.h"
#include "lcd_16x2_1.h"

ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;
UART_HandleTypeDef huart5;
uint32_t Tim_Count ;

//uint32_t Config_Data[11] = {0} ;
char Actuator[13] = "Actuators_ON\n" ;
char Solenoid[12] = "Solenoid_ON\n" ;
char Throw_Good1[11] = "Throw_Good\n" ;
char Throw_Bad1[10] = "Throw_Bad\n" ;
char Throw_Retest[13] = "Throw_Retest\n" ;
char str[15] ;
static uint32_t PR_C = 0 ;
uint32_t  ADC_VAL[4] = {0};
uint32_t  Pressure_read_P1[4] = {0};
uint32_t  Pressure_read_P2[4] = {0};
uint32_t  Pressure_read_P3[4] = {0};
uint32_t  Pressure_read_P4[4] = {0};
uint32_t  Result_of_Sensor[4];
uint32_t  Difference;
uint32_t  Difference1;
uint32_t  Difference2;
uint32_t  Acuator_1_result;
uint32_t  Acuator_2_result;
uint32_t  Acuator_3_result;
uint32_t  Acuator_4_result;
char Send_Data[4] ;

//  Turn On 4 different LED's
// (indicating that 4 actuator are now ready to hold the gloves)
void Actuator_ON()
{


		HAL_GPIO_WritePin(GPIOC,Actuator_1_Pin,GPIO_PIN_SET);
	     HAL_UART_Transmit(&huart5, (uint8_t *)Actuator, 13, 18) ;
		//HAL_GPIO_WritePin(GPIOC,Actuator_2_Pin,GPIO_PIN_SET);
		HAL_UART_Transmit(&huart5, (uint8_t *)Actuator, 13, 18) ;
		//HAL_GPIO_WritePin(GPIOC,Actuator_3_Pin,GPIO_PIN_SET);
		HAL_UART_Transmit(&huart5, (uint8_t *)Actuator, 13, 18) ;
		//HAL_GPIO_WritePin(GPIOC,Actuator_4_Pin,GPIO_PIN_SET);
		HAL_UART_Transmit(&huart5, (uint8_t *)Actuator, 13, 18) ;


}

// Blow air from Solenoid into 4 different gloves(AIR 1)
void Solenoid_ON_1()
{

	//HAL_Delay(3000); // 3 sec time to fill air
	HAL_UART_Transmit(&huart5, (uint8_t *)Solenoid, 12, 18) ;

}

// Blow air from Solenoid into 4 different gloves(AIR 2)
void Solenoid_ON_2()
{

	//HAL_Delay(3000); // 3 sec time to fill air
	HAL_UART_Transmit(&huart5, (uint8_t *)Solenoid, 12, 18) ;

}

// Read Pressure values into four different variables
// This function gets called four times(for Pressure_1,Pressure_2,Pressure_3,Pressure_4)


void ADC_READ()
{
	HAL_ADC_Start_DMA(&hadc1, ADC_VAL, 4) ; // ADC Start
	HAL_Delay(50) ;                         // Delay
	HAL_ADC_Stop_DMA(&hadc1) ;
}

void LCD_READ(uint32_t  Pressure_read_P[])
{

	HAL_Delay(90) ;
	lcd_clear() ;
	lcd_put_cur(0, 0) ;
	lcd_send_string("MODULE 1 ") ;
	HAL_Delay(1000) ;
	lcd_put_cur(1, 0) ;
	for(uint8_t i = 0 ; i < 4 ; i++)
	{
		sprintf(str , "SENSOR - %d %ld", (i+1) , Pressure_read_P[i]) ;
		lcd_send_string(str) ;
		HAL_Delay(70) ;
		lcd_put_cur(1, 0) ;
	}


}




void Pressure_sensor_read()
{
	//Read values from ADC
	ADC_READ() ;

	//Delay to Preserve Values
	HAL_Delay(300) ;

	//Counter to check
	PR_C++ ;

	if(PR_C == 1)
	{
		// Copies contents of ADC_VAL to Pressure_read_P1
		memcpy(Pressure_read_P1, ADC_VAL, sizeof(ADC_VAL)) ;

		//Delay to Preserve memory
		//HAL_Delay(200);

		//LCD Reading ADC Values.
		LCD_READ(Pressure_read_P1) ;
	}
	else
	{
		if (PR_C == 2)
		{
			// Copies contents of ADC_VAL to Pressure_read_P1
			memcpy(Pressure_read_P2, ADC_VAL, sizeof(ADC_VAL)) ;

			//Delay to Preserve memory
			HAL_Delay(200);

			//LCD Reading ADC Values.
			LCD_READ(Pressure_read_P2) ;
		}
		else
		{
			if(PR_C == 3)
			{
				// Copies contents of ADC_VAL to Pressure_read_P1
				memcpy(Pressure_read_P3, ADC_VAL, sizeof(ADC_VAL)) ;

				//Delay to Preserve memory
				HAL_Delay(200);

				//LCD Reading ADC Values.
				LCD_READ(Pressure_read_P3) ;
			}
			else
			{
				if(PR_C == 4)
				{
					// Copies contents of ADC_VAL to Pressure_read_P1
					memcpy(Pressure_read_P4, ADC_VAL, sizeof(ADC_VAL)) ;

					//Delay to Preserve memory
					HAL_Delay(200);

					//LCD Reading ADC Values.
					LCD_READ(Pressure_read_P4) ;

					//Initializing the Counter to 0
					PR_C = 0 ;
				}
			}
		}
	}


	//Wait till it is done by MCU
	HAL_Delay(300) ;
}



//static uint8_t Pressure_Stage = 1;
// Throw good glove from actuator into bin
void Throw_Good()
{

	 if(Result_of_Sensor[0] == 1)
	{
		 //HAL_GPIO_WritePin(GPIOD,Actuator_1_Good_Pin,GPIO_PIN_SET);

		 HAL_UART_Transmit(&huart5, (uint8_t *)Throw_Good1, 11, 18) ;
	}



	 if(Result_of_Sensor[1] == 1)
	 	{
		 //HAL_GPIO_WritePin(GPIOD,Actuator_2_Good_Pin,GPIO_PIN_SET);
		 HAL_UART_Transmit(&huart5, (uint8_t *)Throw_Good1, 11, 18) ;
	 	}


	 if(Result_of_Sensor[2] == 1)
	 	{
		 //HAL_GPIO_WritePin(GPIOD,Actuator_3_Good_Pin,GPIO_PIN_SET);

		 HAL_UART_Transmit(&huart5, (uint8_t *)Throw_Good1, 11, 18) ;
	 	}


	 if(Result_of_Sensor[3] == 1)
	 	{
		 //HAL_GPIO_WritePin(GPIOD,Actuator_4_Good_Pin,GPIO_PIN_SET);
		 HAL_UART_Transmit(&huart5, (uint8_t *)Throw_Good1, 11, 18) ;
	 	}



}

// Throw bad glove from actuator into bin
void Throw_Bad()
{

	 if(Result_of_Sensor[0] == 2)
		{
		 //HAL_GPIO_WritePin(GPIOD, Actuator_1_Bad_Pin,GPIO_PIN_SET);
		 HAL_UART_Transmit(&huart5, (uint8_t *)Throw_Bad1, 10, 18) ;
		}



		 if(Result_of_Sensor[1] == 2)
		 	{
			 //HAL_GPIO_WritePin(GPIOD, Actuator_2_Bad_Pin,GPIO_PIN_SET);
			 HAL_UART_Transmit(&huart5, (uint8_t *)Throw_Bad1, 10, 18) ;
		 	}


		 if(Result_of_Sensor[2] == 2)
		 	{
			 //HAL_GPIO_WritePin(GPIOD, Actuator_3_Bad_Pin,GPIO_PIN_SET);
			 HAL_UART_Transmit(&huart5, (uint8_t *)Throw_Bad1, 10, 18) ;
		 	}


		 if(Result_of_Sensor[3] == 2)
		 	{
			 //HAL_GPIO_WritePin(GPIOD, Actuator_4_Bad_Pin,GPIO_PIN_SET);
			 HAL_UART_Transmit(&huart5, (uint8_t *)Throw_Bad1, 10, 18) ;
		 	}
}

// Throw Re-test glove from actuator into bin
void Throw_Re_test()
{

	 if(Result_of_Sensor[0] == 3)
		{
		 //HAL_GPIO_WritePin(GPIOD,Actuator_1_Re_Test_Pin,GPIO_PIN_SET);
		 HAL_UART_Transmit(&huart5, (uint8_t *)Throw_Retest, 13, 18) ;
		}



		 if(Result_of_Sensor[1] == 3)
		 	{
			 //HAL_GPIO_WritePin(GPIOD,Actuator_2_Re_Test_Pin,GPIO_PIN_SET);
			 HAL_UART_Transmit(&huart5, (uint8_t *)Throw_Retest, 13, 18) ;
		 	}


		 if(Result_of_Sensor[2] == 3)
		 	{
			 //HAL_GPIO_WritePin(GPIOD,Actuator_3_Re_Test_Pin,GPIO_PIN_SET);
			 HAL_UART_Transmit(&huart5, (uint8_t *)Throw_Retest, 13, 18) ;
		 	}


		 if(Result_of_Sensor[3] == 3)
		 	{
			 //HAL_GPIO_WritePin(GPIOD,Actuator_4_Re_Test_Pin,GPIO_PIN_SET);
			 HAL_UART_Transmit(&huart5, (uint8_t *)Throw_Retest, 13, 18) ;
		 	}
}


    // G_Counter is a global variable which is defined in timer call back function
	//	(i.e we are incrementing G_Counter for for every call-back event)
	// for every 1 sec there is a call-back.
	// Comparing  Config_Data[11] with G_Counter
	// if Config_Data == G_Counter,return character

uint32_t Update_State_Transit(uint32_t Config_Data[])
{
	// Config_Data[0]  // Time to Turn On Actuators
	// Config_Data[1]  // Time to blow air from Solenoid(AIR 1)
	// Config_Data[2]  // Time to read pressure from the gloves(P1)
	// Config_Data[3]  // Time to read pressure from the gloves(P2)
	// Config_Data[4]  // Time to blow air from Solenoid(AIR 2)
	// Config_Data[5]  // Time to read pressure from the gloves(P3)
	// Config_Data[6]  // Time to read pressure from the gloves(P4)
	// Config_Data[7]  // Time to throw good glove
	// Config_Data[8]  // Time to throw bad glove
	// Config_Data[9]  // Time to throw re-test glove

	//uint32_t Config_Data[10] = {3,4,7,20,25,30,45,55,57,60} ;


/*	Config_Data[0] = 2
	Config_Data[1] = 4

	 solenoid delay 3sec

	Config_Data[2] = 7
	Config_Data[3] = 8
	Config_Data[4] = 10
	Config_Data[5] = 12
	Config_Data[6] = 14
	Config_Data[7] = 20
	Config_Data[8] = 22
	Config_Data[9] = 20
*/



	if(Config_Data[0] == Tim_Count)  // 3
	{
		return 1;

	}
	else if(Config_Data[1] == Tim_Count) // 4
	{

		return 2;
	}
	else if(Config_Data[2] == Tim_Count) // 7
	{

		return 3;
	}
	else if(Config_Data[3] == Tim_Count) // 20
	{

		return 4;
	}
	else if(Config_Data[4] == Tim_Count) // 25
	{

		return 5;
	}
	else if(Config_Data[5] == Tim_Count) // 30
	{

		return 6;
	}
	else if(Config_Data[6] == Tim_Count) //45
	{

		return 7;
	}
	else if(Config_Data[7] == Tim_Count) //55
	{

		return 8;
	}
	else if(Config_Data[8] == Tim_Count) //57
	{

		return 9;
	}
	else if(Config_Data[9] == Tim_Count) //60
	{
		return 10 ;

	}
	return 0 ;
}



// Based on the State Transition this function actually performs task
// (like 1.turning ON the LED's to indicate that actuators are ready to hold gloves
//       2.Blow air
//		 3.Take Pressure readings
//		 4.Seperate Good,Bad and Re-test Gloves)

void Rotor_State_Transit(uint32_t State_Transit_1)
{

	switch(State_Transit_1)

	{
	  		case 1 : // turn on 4 LED's to indicate that actuators are ready to hold gloves.

	  					Actuator_ON();
	  					break;

	  		case 2 : // AIR 1 start(blow air)
	  					Solenoid_ON_1();
	  					break;

	  		case 3 : // P1 read(read pressure from the gloves)
	  					Pressure_sensor_read();
	  					break;

	  		case 4 : // P2 read(read pressure from the gloves)
	  					Pressure_sensor_read();
	  					break;

	  		case 5 : // AIR 2 start(blow air)
	  					Solenoid_ON_2();
	  					break;

	  		case 6 : // P3 read(read pressure from the gloves)
	  					Pressure_sensor_read();
	  					break;

	  		case 7 : // P4 read(read pressure from the gloves)
	  					Pressure_sensor_read();
	  					 // Process all the Pressure Data and update the result array.
	  					Sensor_Result( );
	  					break;

	  		case 8 : //Result
	  					Throw_Good();
	  					break;

	  		case 9 : //Result
	  			  		Throw_Bad();
	  			  		break;

	  		case 10 : //Result
	  			  		Throw_Re_test();
	  			  		break;


	}


}

// Gives result
// (whether glove is good,bad or re-test)
void Sensor_Result( )
{
	/* uint16_t Difference = P1 - P2;
	if(Difference  <=  20 )
		return 1;  // good glove
	else if(Difference  >=  50 )
		return 2; // bad glove
	else ((Difference  >=  20)  && (Difference  <=  40 ) )
		return 3; // Re-test glove
*/


	 	 	Difference  = Pressure_read_P1[0] - Pressure_read_P2[0];

			if(Difference  <=  20 )
				Acuator_1_result = 1;  // good glove
			else if(Difference  >=  50 )
				Acuator_1_result = 2; // bad glove
			else if((Difference  >=  20)  && (Difference  <=  40 ) )

					Acuator_1_result = 3; // Re-test glove


			Difference  = Pressure_read_P1[1] - Pressure_read_P2[1];
						if(Difference  <=  20 )
							Acuator_2_result = 1;  // good glove
						else if(Difference  >=  50 )
							Acuator_2_result = 2; // bad glove

						else if((Difference  >=  20)  && (Difference  <=  40 ) )
							Acuator_2_result = 3; // Re-test glove


			 Difference  = Pressure_read_P1[2] - Pressure_read_P2[2];
							if(Difference  <=  20 )
								Acuator_3_result = 1;  // good glove
							else if(Difference  >=  50 )
								Acuator_3_result = 2; // bad glove
							else if((Difference  >=  20)  && (Difference  <=  40 ) )
								Acuator_3_result = 3; // Re-test glove


			 Difference  = Pressure_read_P1[3] - Pressure_read_P2[3];
							if(Difference  <=  20 )
								Acuator_4_result = 1;  // good glove
							else if(Difference  >=  50 )
								Acuator_4_result = 2; // bad glove
							else if ((Difference  >=  20)  && (Difference  <=  40 ) )
								Acuator_4_result = 3; // Re-test glove

							Result_of_Sensor[0]=Acuator_1_result;
							Result_of_Sensor[1]=Acuator_2_result;
							Result_of_Sensor[2]=Acuator_3_result;
							Result_of_Sensor[3]=Acuator_4_result;

							memcpy(Send_Data , Result_of_Sensor , sizeof(Result_of_Sensor)) ;


/*	Difference1  = (Pressure_read_P1[0] - Pressure_read_P2[0]);
	Difference2  = (Pressure_read_P3[0] - Pressure_read_P4[0]);

				if((Difference1 - Difference2) <= 20)
				{
					Acuator_1_result = 1; // good glove
				}
				else if( ((Difference1 <= 20) && (Difference2 >= 20)) || ((Difference1 >= 20) && (Difference2 <= 20)) )
				{
					Acuator_1_result = 3; // re-test glove
				}
				else
				{
					Acuator_1_result = 2; // bad glove
				}

*/
	// uint16_t Result_of_Sensor[4] = {1,1,1,1};
}





/*
 * //Bug not needed at a single time
	switch( Pressure_Stage )
		  	{
		  		 case 1 : // Pressur_1 reading from 4 sensors

		  			  for(uint8_t i =0;i<4;i++)

		  			  {
		  				  Pressure_read_P1[i]= ADC_VAL[i];

		  			  }
		  			  Pressure_Stage++;
		  			  break;


		  		  case 2 : // Pressur_2 reading from 4 sensors

		  			  for(uint8_t i =0;i<4;i++)

		  		 	  {
		  				  Pressure_read_P2[i]= ADC_VAL[i];

		  		 	  }
		  			  Pressure_Stage++;
		  			  break;

		  		  case 3 :  // Pressur_3 reading from 4 sensors

		  		 	 for(uint8_t i =0;i<4;i++)

		  		 	 {
		  		 		Pressure_read_P3[i]= ADC_VAL[i];

		  		 	 }
		  		 	 Pressure_Stage++;
		  		 	 break;

		  		   case 4 :  // Pressur_4 reading from 4 sensors

		  		 	for(uint8_t i =0;i<4;i++)

		  		 	{
		  		 		Pressure_read_P4[i]= ADC_VAL[i];
		  		 	}
		  		 	break ;

		  	}
 *
 *
 */


