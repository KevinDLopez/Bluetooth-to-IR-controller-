#include "tm4c123gh6pm.h"
#include "Init.c"

// ENOCDED BIT FORMS 
void BitToSend(uint8_t bit ) {
/*  • Logical 1: 0.6ms high, 0.5ms low 
    • Logical 0: 0.6ms high, 1.6ms low    */
	switch(bit){
		case 0: 	PWM0_1_CMPA_R = 1000; 	Timer1A(600);
							PWM0_1_CMPA_R = 0;			Timer1A(1600);		
							break; 
		case 1: 	PWM0_1_CMPA_R = 1000; 	Timer1A(600);
							PWM0_1_CMPA_R = 0; 	  	Timer1A(500);
							break;
		default: 	break;
	}
}

// ENCODED START BIT
void SendStartBit(void){
	PWM0_1_CMPA_R = 1000; 	Timer1A(9000);
	PWM0_1_CMPA_R = 0;			Timer1A(4400);		

}


void  SendTVAddress(void){
	/// using bit to send to send specific bits 
	//ADDRESS = 11 0 11111 00 1 00000

	BitToSend(1); BitToSend(1);
	BitToSend(0);
	BitToSend(1); BitToSend(1); BitToSend(1); BitToSend(1); BitToSend(1);
	BitToSend(0); BitToSend(0);
	BitToSend(1);
	BitToSend(0); BitToSend(0);BitToSend(0); BitToSend(0);BitToSend(0);
}

void STOP_BIT(){
	PWM0_1_CMPA_R = 1000; Timer1A(600); 
	PWM0_1_CMPA_R = 0;
}






///////////////////////////////////////////////////////////////
///////////////////		COMMANDS 	//////////////////////////
///////////////////////////////////////////////////////////////
void OnOff(){// 111 0 1111 000 1 0000
	SendStartBit(); SendTVAddress(); 
	BitToSend(1); BitToSend(1); BitToSend(1); 
	BitToSend(0); 
	BitToSend(1); BitToSend(1); BitToSend(1); BitToSend(1); 
	BitToSend(0); BitToSend(0); BitToSend(0); 
	BitToSend(1); 
	BitToSend(0); BitToSend(0); BitToSend(0); BitToSend(0);
	STOP_BIT();
}



void VolumeUp(){
	SendStartBit(); SendTVAddress(); 
	BitToSend(1); 
	BitToSend(0);  
	BitToSend(1); BitToSend(1); BitToSend(1); BitToSend(1); BitToSend(1); BitToSend(1); 
	BitToSend(0);
	BitToSend(1);
	BitToSend(0); BitToSend(0); BitToSend(0); BitToSend(0); BitToSend(0); BitToSend(0);
	STOP_BIT();
}

void VolumeDown(){
	SendStartBit(); SendTVAddress(); 
	BitToSend(0); BitToSend(0);  
	BitToSend(1); BitToSend(1); BitToSend(1); BitToSend(1); BitToSend(1); BitToSend(1); BitToSend(1); BitToSend(1);
	BitToSend(0); BitToSend(0); BitToSend(0); BitToSend(0); BitToSend(0); BitToSend(0);
	STOP_BIT();
}

void Netflix(){	//11 00 11 0 1 00 11 00 1  0 
	SendStartBit(); SendTVAddress(); 
	BitToSend(1); BitToSend(1);  
	BitToSend(0); BitToSend(0); 
	BitToSend(1); BitToSend(1);
	BitToSend(0); 
	BitToSend(1); 
	BitToSend(0); BitToSend(0);
	BitToSend(1); BitToSend(1); 
	BitToSend(0); BitToSend(0); 
	BitToSend(1); 
	BitToSend(0);
	STOP_BIT();
}

void YouTube(){//0 11 0 11 0 11 00 1 00 1 0 
	SendStartBit(); SendTVAddress(); 
	BitToSend(0); 					//0
	BitToSend(1); BitToSend(1); 	//11
	BitToSend(0); 					//0
	BitToSend(1); BitToSend(1); 	//11
	BitToSend(0); 					//0
	BitToSend(1); BitToSend(1);		//11
	BitToSend(0); BitToSend(0);		//00
	BitToSend(1);					//1
	BitToSend(0); BitToSend(0);		//00
	BitToSend(1);					//1
	BitToSend(0);					//0
	STOP_BIT();
}

void Amazon(){//000 111 000 1 0
	SendStartBit(); SendTVAddress(); 
	BitToSend(0); BitToSend(0); BitToSend(0); 
	BitToSend(1); BitToSend(1); BitToSend(1); 
	BitToSend(0); BitToSend(0); BitToSend(0); 
	BitToSend(1);
	BitToSend(0); 
//	BitToSend(0); BitToSend(0); BitToSend(0); BitToSend(0); BitToSend(0);
	STOP_BIT();
}

void UP(){//1 0 1 0 1 0 11 0 1 0 1 0
	SendStartBit(); SendTVAddress(); 
	BitToSend(1); 
	BitToSend(0);  
	BitToSend(1); 
	BitToSend(0); 
	BitToSend(1); 
	BitToSend(0); 
	BitToSend(1); BitToSend(1); 
	BitToSend(0); 
	BitToSend(1);
	BitToSend(0); 
	BitToSend(1); 
	BitToSend(0); 
	//BitToSend(0); BitToSend(0); BitToSend(0);
	STOP_BIT();
}


void DOWN(){//000 1 0 1 0 1111 0 1 0 1 0
	SendStartBit(); SendTVAddress(); 
	BitToSend(0); BitToSend(0); BitToSend(0); 
	BitToSend(1); 
	BitToSend(0); 
	BitToSend(1); 
	BitToSend(0); 
	BitToSend(1); BitToSend(1); BitToSend(1);BitToSend(1); 
	BitToSend(0); 
	BitToSend(1); 
	BitToSend(0); 
	BitToSend(1); 
	BitToSend(0);
	STOP_BIT();
}


void LEFT(){//111 00 1 0 1 000 11 0 1 0
	SendStartBit(); SendTVAddress(); 
	BitToSend(1); BitToSend(1); BitToSend(1); 
	BitToSend(0); BitToSend(0); 
	BitToSend(1); 
	BitToSend(0); 
	BitToSend(1); 
	BitToSend(0); BitToSend(0);BitToSend(0); 
	BitToSend(1); BitToSend(1); 
	BitToSend(0); 
	BitToSend(1); 
	BitToSend(0);
	STOP_BIT();
}

void RIGHT(){//0 11 00 1 0 11 00 11 0 1 0
	SendStartBit(); SendTVAddress(); 
	BitToSend(0); 
	BitToSend(1); BitToSend(1); 
	BitToSend(0); BitToSend(0); 
	BitToSend(1); 
	BitToSend(0); 
	BitToSend(1); BitToSend(1); 
	BitToSend(0); BitToSend(0); 
	BitToSend(1); BitToSend(1); 
	BitToSend(0); 
	BitToSend(1); 
	BitToSend(0);
	STOP_BIT();
}


void CLICK(){//1 0 1 00 1 0 1 0 1 0 11 0 1 0
	SendStartBit(); SendTVAddress(); 
	BitToSend(1); 
	BitToSend(0); 
	BitToSend(1); 
	BitToSend(0); BitToSend(0); 
	BitToSend(1); 
	BitToSend(0); 
	BitToSend(1); 
	BitToSend(0); 
	BitToSend(1); 
	BitToSend(0); 
	BitToSend(1); BitToSend(1); 
	BitToSend(0); 
	BitToSend(1); 
	BitToSend(0);
	STOP_BIT();
}

void BACK(){//11 0 11111 00 1 00000
	SendStartBit(); SendTVAddress(); 
	BitToSend(1); BitToSend(1); 
	BitToSend(0); 
	BitToSend(1); BitToSend(1); BitToSend(1); BitToSend(1); BitToSend(1); 
	BitToSend(0); BitToSend(0); 
	BitToSend(1); 
	BitToSend(0); BitToSend(0); BitToSend(0); BitToSend(0); BitToSend(0);
	STOP_BIT();
}

/////////////////////////////////////////////////
//				STRINGS TO SEND 
////////////////////////////////////////////////
struct UART_7_SEND_STRING
{
	uint8_t OnOff_S[] 		= "Device is on\n\r";
	uint8_t VolumeUp_S[] 	= "VolumeUp\n\r";
	uint8_t VolumeDown_S[] 	= "VolumeDown\n\r";
	uint8_t Netflix_S[] 	= "Netflix\n\r";
	uint8_t YouTube_S[] 	= "YouTube\n\r";
	uint8_t Amazon_S[] 		= "Amazon\n\r";
	uint8_t up_S[] 			= "up\n\r";
	uint8_t DOWN_S[]	 	= "DOWN\n\r";
	uint8_t LEFT_S[] 		= "LEFT\n\r";
	uint8_t RIGHT_S[]	 	= "RIGHT\n\r";
	uint8_t CLICK_S[] 		= "CLICK\n\r";
	uint8_t BACK_S[] 		= "BACK\n\r";
}DATA_OUT;
