#include "toggle_timer_interrupt_TivaWare.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/uart.h"
#include "driverlib/timer.h"

#define 	GREEN_MASK 		0x08
#define 	Blue_MASK 		0x04
#define 	RED_MASK 		0x02
//*****************************************************************************
//
//!
//! Design a counter. The counter is incremented by 1 when SW1 (PF4) or SW2 (PF0) 
//! is pressed.
//
//*****************************************************************************

// global variable visible in Watch window of debugger
// increments at least once per button press
volatile int count = 60;


void Timer0A_Init(unsigned long period)
{   
	//
  // Enable Peripheral Clocks 
  //
  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
  TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC); 		// configure for 32-bit timer mode
  TimerLoadSet(TIMER0_BASE, TIMER_A, period -1);      //reload value
	IntPrioritySet(INT_TIMER0A, 0x00);  	 // configure Timer0A interrupt priority as 0
  IntEnable(INT_TIMER0A);    				// enable interrupt 19 in NVIC (Timer0A)
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);      // arm timeout interrupt
  TimerEnable(TIMER0_BASE, TIMER_A);      // enable timer0A
}

void
PortFunctionInit(void)
{
    //
    // Enable Peripheral Clocks 
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    //
    // Enable pin PF4 for GPIOInput
    //
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);

    //
    // Enable pin PF0 for GPIOInput
    //

    //
    //First open the lock and select the bits we want to modify in the GPIO commit register.
    //
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) = 0x1;

    //
    //Now modify the configuration of the pins that we unlocked.
    //
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);
	
	    //
    // Enable pin PF4 for GPIOInput
    //
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);
		
		//Enable pull-up on PF4 and PF0
		GPIO_PORTF_PUR_R |=  00010101;

}


void
Interrupt_Init(void)
{
  IntEnable(INT_GPIOF);  							// enable interrupt 30 in NVIC (GPIOF)
	IntPrioritySet(INT_GPIOF, 0x01); 		// configure GPIOF interrupt priority as 1
	GPIO_PORTF_IM_R |= 00010101;   		// arm interrupt on PF0 and PF4
	GPIO_PORTF_IS_R &= ~00010101;     // PF0 and PF4 are edge-sensitive
  GPIO_PORTF_IBE_R &= ~00010101;   	// PF0 and PF4 not both edges trigger 
  GPIO_PORTF_IEV_R &= ~00010101;  	// PF0 and PF4 falling edge event
	
}

void
uart_Init(void) {
	
		//SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);


    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	
	    //
    // Enable Peripheral Clocks 
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    //
    // Enable pin PF1 PF2 and PF3 for GPIOOutput
    //
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);
	  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);
	  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3);
	  //
    // Enable pin PF4 for GPIOInput
    //
		GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);
}



//interrupt handler for Timer0A
void Timer0A_Handler(void)
{
	// acknowledge flag for Timer0A timeout
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	//check if sw1 or sw2 has been pressed
	count--;
		
	if(count == 0)
		count = 60;
	
	else{	char str[3];
		UARTCharPut(UART0_BASE, 'C');
    UARTCharPut(UART0_BASE, 'o');
    UARTCharPut(UART0_BASE, 'u');
    UARTCharPut(UART0_BASE, 'n');
    UARTCharPut(UART0_BASE, 't');
    UARTCharPut(UART0_BASE, ':');}

		char str[3];
		sprintf(str, "%d", count);
		UARTCharPut(UART0_BASE, str[0]); 
		UARTCharPut(UART0_BASE, str[1]); 
		UARTCharPut(UART0_BASE, str[2]); 
		UARTCharPut(UART0_BASE, '\n'); 
		UARTCharPut(UART0_BASE, '\r');
			
}


//interrupt handler
void GPIOPortF_Handler(void)
{
	char str[3];
	//SW1 is pressed
	if(GPIO_PORTF_RIS_R&00000001)
	{
		//acknowledge flag for PF4
		//counter decremented by 30
		count = count - 30;
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x00);
		sprintf(str, "%d", count);
		UARTCharPut(UART0_BASE, str[0]); 
		UARTCharPut(UART0_BASE, str[1]); 
		UARTCharPut(UART0_BASE, str[2]); 
		UARTCharPut(UART0_BASE, '\n'); 
		UARTCharPut(UART0_BASE, '\r'); 
	}
	
	//SW2 is pressed
  if(GPIO_PORTF_RIS_R&0x00010000)
	{
		// acknowledge flag for PF0
		GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_0);
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x00);
		sprintf(str, "%d", count);
		UARTCharPut(UART0_BASE, str[0]); 
		UARTCharPut(UART0_BASE, str[1]); 
		UARTCharPut(UART0_BASE, str[2]); 
		UARTCharPut(UART0_BASE, '\n'); 
		UARTCharPut(UART0_BASE, '\r');
	}
}

int main(void)
{
		unsigned long period = 16000000; //reload value to Timer0A to generate half second delay	
		
	//initialize the GPIO ports	
		PortFunctionInit();
	
		//initialize UART0
		uart_Init();
		
	  //initialize Timer0A and configure the interrupt
		Timer0A_Init(period);
	
		//configure the GPIOF interrupt
		Interrupt_Init();
	
		IntMasterEnable();       		// globally enable interrupt
	  UARTCharPut(UART0_BASE, 'C');
    UARTCharPut(UART0_BASE, 'o');
    UARTCharPut(UART0_BASE, 'u');
    UARTCharPut(UART0_BASE, 'n');
    UARTCharPut(UART0_BASE, 't');
    UARTCharPut(UART0_BASE, ':');
    UARTCharPut(UART0_BASE, '\n'); 
		UARTCharPut(UART0_BASE, '\r'); 
	
	
	
	  uint8_t LED_data;
	
		//initialize the GPIO ports	
		PortFunctionInit();
	
    // Turn on the LED.
    LED_data= 0x02;
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, LED_data);
    
    //
    // Loop forever.
    //
    while(1)
    {
        // Delay for a bit.
				SysCtlDelay(2000000);	

        // Toggle the LED.
        LED_data^=RED_MASK;	//toggle RED LED (PF1)
				GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, LED_data);
    }
}

