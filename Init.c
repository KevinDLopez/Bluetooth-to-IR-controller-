#include "tm4c123gh6pm.h"
#include "stdint.h"

//////////////////////////////////////////////////////////
/////////////////////// PLL ///////////////////////////
//////////////////////////////////////////////////////////
//#define SYSCTL_RIS_R            (*((volatile unsigned long *)0x400FE050))
#define SYSCTL_RIS_PLLLRIS      0x00000040  // PLL Lock Raw Interrupt Status
//#define SYSCTL_RCC_R            (*((volatile unsigned long *)0x400FE060))
#define SYSCTL_RCC_XTAL_M       0x000007C0  // Crystal Value
#define SYSCTL_RCC_XTAL_6MHZ    0x000002C0  // 6 MHz Crystal
#define SYSCTL_RCC_XTAL_8MHZ    0x00000380  // 8 MHz Crystal
#define SYSCTL_RCC_XTAL_16MHZ   0x00000540  // 16 MHz Crystal
//#define SYSCTL_RCC2_R           (*((volatile unsigned long *)0x400FE070))
#define SYSCTL_RCC2_USERCC2     0x80000000  // Use RCC2
#define SYSCTL_RCC2_DIV400      0x40000000  // Divide PLL as 400 MHz vs. 200
                                            // MHz
#define SYSCTL_RCC2_SYSDIV2_M   0x1F800000  // System Clock Divisor 2
#define SYSCTL_RCC2_SYSDIV2LSB  0x00400000  // Additional LSB for SYSDIV2
#define SYSCTL_RCC2_PWRDN2      0x00002000  // Power-Down PLL 2
#define SYSCTL_RCC2_BYPASS2     0x00000800  // PLL Bypass 2
#define SYSCTL_RCC2_OSCSRC2_M   0x00000070  // Oscillator Source 2
#define SYSCTL_RCC2_OSCSRC2_MO  0x00000000  // MOSC
#define SYSDIV2 4

// bus frequency is 400MHz/(SYSDIV2+1) = 400MHz/(4+1) = 80 MHz
// see the table at the end of this file
// configure the system to get its clock from the PLL
void PLL_Init(void){
  // 0) configure the system to use RCC2 for advanced features
  //    such as 400 MHz PLL and non-integer System Clock Divisor
  SYSCTL_RCC2_R |= SYSCTL_RCC2_USERCC2;
  // 1) bypass PLL while initializing
  SYSCTL_RCC2_R |= SYSCTL_RCC2_BYPASS2;
  // 2) select the crystal value and oscillator source
  SYSCTL_RCC_R &= ~SYSCTL_RCC_XTAL_M;   // clear XTAL field
  SYSCTL_RCC_R += SYSCTL_RCC_XTAL_16MHZ;// configure for 16 MHz crystal
  SYSCTL_RCC2_R &= ~SYSCTL_RCC2_OSCSRC2_M;// clear oscillator source field
  SYSCTL_RCC2_R += SYSCTL_RCC2_OSCSRC2_MO;// configure for main oscillator source
  // 3) activate PLL by clearing PWRDN
  SYSCTL_RCC2_R &= ~SYSCTL_RCC2_PWRDN2;
  // 4) set the desired system divider and the system divider least significant bit
  SYSCTL_RCC2_R |= SYSCTL_RCC2_DIV400;  // use 400 MHz PLL
  SYSCTL_RCC2_R = (SYSCTL_RCC2_R&~0x1FC00000) // clear system clock divider field
                  + (SYSDIV2<<22);      // configure for 80 MHz clock
  // 5) wait for the PLL to lock by polling PLLLRIS
  while((SYSCTL_RIS_R&SYSCTL_RIS_PLLLRIS)==0){};
  // 6) enable use of PLL by clearing BYPASS
  SYSCTL_RCC2_R &= ~SYSCTL_RCC2_BYPASS2;
}



//////////////////////////////////////////////////////////
/////////////////////// UART7 INTERUPT///////////////////////////
//////////////////////////////////////////////////////////
// 57600, 8 Data bits, 1 stop, odd pari    , interupt
void UART7_Init(void){ //PE0,1  rx,tx
  SYSCTL_RCGCUART_R |=  SYSCTL_RCGCUART_R7;
  SYSCTL_RCGCGPIO_R |=  0x10; // CLOCK FOR PORT E

  GPIO_PORTE_DEN_R |= 3;  
  GPIO_PORTE_AFSEL_R |= 3; // ALTERNATE FUNTION 
  GPIO_PORTE_PCTL_R |= 0x00000011;

  UART7_CTL_R = 0;
  UART7_IBRD_R = 86;   //17;
  UART7_FBRD_R = 51; //731
  UART7_CC_R = 0;
  UART7_LCRH_R |= 0X62;  // 8 bits,  Odd Paraty enable 

  UART7_IM_R |= 0x0010;  // ENABLE RX interrupt   pg. 924 UARTIM      INTERRUPT


  UART7_CTL_R |= 0x300;  // ENABLE RX TX 
  UART7_CTL_R |= 1;      // ENABLE UART

  NVIC_EN1_R |= 0x80000000;     // enable IRQ63 for UART7   pg.104     INTERRUPT   
  NVIC_PRI15_R = 3 << 29;       // GIVE PRIOTY 3 ON INTERRUPT          INTERRUPT


}

void UART7_Tx(uint8_t ByteToSend){
  while((UART7_FR_R & UART_FR_TXFF) != 0);
    UART7_DR_R = ByteToSend;
}

uint8_t UART7_Rx(void){
  while((UART7_FR_R & UART_FR_RXFE) != 0);
  return UART7_DR_R;
}

void UART7_Transmit_String(const uint8_t *MessageString){
  while(*MessageString){ // while there is a character
    UART7_Tx(*MessageString); //send character
    MessageString++;
  }
}




/////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////  PWM ///////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

void PWM1A_Init( ){ ////////// PB4        40Khz
//                                                  pmw2  Gen1A
  SYSCTL_RCGCPWM_R |= 0x01;             // 1) activate PWM0
  SYSCTL_RCGCGPIO_R |= 0x02;            // 2) activate port B

  while((SYSCTL_PRGPIO_R&0x02) == 0){}; //delay

  /////////////////////////port B INIT
  GPIO_PORTB_AFSEL_R |= 0x10;           // enable alt funct on PB4
  GPIO_PORTB_PCTL_R &= ~0x000F0000;     // configure PB4 as PWM0
  GPIO_PORTB_PCTL_R |= 0x00040000;
  GPIO_PORTB_AMSEL_R &= ~0x10;          // disable analog functionality on PB6
  GPIO_PORTB_DEN_R |= 0x10;             // enable digital I/O on PB6
  GPIO_PORTB_DIR_R |= 0x10;             // set  PB4 as OUTPUT
  //////////////////////// PWM CONFIG

  PWM0_1_CTL_R = 0;                     // 4) re-loading down-counting mode
  PWM0_1_GENA_R = 0xC8;                 // low on LOAD, high on CMPA down
  // PB6 goes low on LOAD
  // PB6 goes high on CMPA down
  PWM0_1_LOAD_R = 2000;                 // 5) 80Mhz/40k = 2000    
  PWM0_1_CMPA_R = 0;                 // 6) count value when output rises
  PWM0_1_CTL_R |= 0x00000001;           // 7) start PWM0
  PWM0_ENABLE_R |= 0x00000004;          // enable PB6/M0PWM0

}

//////////////////////////////////////////////////////////
/////////////////////// TIMER ///////////////////////////
//////////////////////////////////////////////////////////


void Timer1A(uint32_t delay){//micro seconds
  uint32_t i = 0;
  // Timer1A configuration
  SYSCTL_RCGCTIMER_R |= 0x02; // enable clock to timer Block 1
  TIMER1_CTL_R = 0;           // disable Timer1 during configuration
  TIMER1_CFG_R = 0x04;        // 16-bit timer
  TIMER1_TAMR_R = 0x02;       // periodic countdown mode
  //TIMER1_TAPR_R = 10;         // 16MHZ / 10 = 160 KHZ
  TIMER1_TAILR_R = 80 - 1;      //  80E6/80 = 1E6    period = 1MicroSecond     
  TIMER1_ICR_R |= 0x1;          // clear Timer1A timeout flag
  //TIMER1_IMR_R |= 0x01;       // enable Timer1A timeout interrupt 
  TIMER1_CTL_R |= 0x01;       // enable Timer1A
  NVIC_EN0_R |= 0x00200000;   // enable IRQ21


  while(i < delay){
    while((TIMER1_RIS_R & 0x01) == 0); // wait for timeout
    TIMER1_ICR_R = 0x01;    // clear timerA timeout flag  
    i++;
  }
}
