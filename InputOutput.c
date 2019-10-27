
#include "HisenseIR.c"




int32_t main(void){
	PLL_Init();          // 	bus clock at 80 MHz
	UART7_Init();	//57600, 8 Data bits, 1 stop, odd pari
	PWM1A_Init();		//	PWM 	40Khz	 50% duty Cycle
	Timer1A(10);  	// MicroSecond Timer

	while(1){ //Hybridization mode 



	}


}


void UART7_Handler(void){
	volatile int32_t readback;
	int8_t uartDataIn;
			//MIS = Mask Interrupt Status
	if(UART7_MIS_R & 0x0010){	// UARTMIS ds.p930   
		uartDataIn = UART7_DR_R; //DATA REGISTER  [7:0]


		switch(uartDataIn){
			case 'o': OnOff(); 		break;//on
			case 'u': VolumeUp();	break; //volume up
			case 'd': VolumeDown();	break; //volume down
			case 'n': Netflix();	break; //netflix
			case 'y': YouTube();	break; //youtube
			case 'a': Amazon();		break; //amazon
			case '1': UP();			break; //move Up
			case '2': DOWN();		break; //move Down
			case '3': LEFT();		break; //move left 
			case '4': RIGHT();		break; // move Right
			case 'c': CLICK();		break; //click
			case 'b': BACK();		break; //back
		}




			//ICR = Interrupt Clear Register
		UART7_ICR_R = 0x0010;  //4rth bit RX Clear
		readback = UART0_ICR_R;
	}
	else{
		UART7_ICR_R = UART7_MIS_R;	// clear all interrutp flags
		readback = UART7_ICR_R;			// force flags clear
	}	
}
