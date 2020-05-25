#include <stdint.h>
#include <stdbool.h>
#include "toggle_timer_interrupt_TivaWare.h"
#include "switch_counter_interrupt_TivaWare.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "inc/tm4c123gh6pm.h"

#define		RED	0x02
#define 	BLUE 0x04
#define 	GREEN 0x08

//*****************************************************************************
//
//!
//! A very simple example that uses a general purpose timer generated periodic 
//! interrupt to toggle the on-board LED.
//
//*****************************************************************************
//int count = 0;
//bool start = true;

void
PortFunctionInit(void)
{
    //
    // Enable Peripheral Clocks 
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	


    //
    // Enable pin PF2 for GPIOOutput
    //
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2); //Enable Blue LED
		GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);//Enable Red LED
		GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3); //Enable Green LED
	GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_6); // Motor 1 In1
	GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_7); //Motor 1 In2
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_2); //Motor 2 In1
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_3); //Motor 2 In2
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_4); //Motor 3 In1
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_5); //Motor 3 In2
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_6); //Enable Motor 1
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_7); //Enable Motor 2
	GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4); //Enable Motor 3
	GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_5); //Enable Button
 
	
		
		
		GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0); //SW1
		GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4); //SW2
	
		HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
		HWREG(GPIO_PORTF_BASE + GPIO_O_CR) = 0x01;
		HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
	HWREG(GPIO_PORTA_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
		HWREG(GPIO_PORTA_BASE + GPIO_O_CR) = 0x01;
		HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
		
		//Enable pull-up on PF4 and PF0
		GPIO_PORTF_PUR_R |= 0x11; 
}

////Globally enable interrupts 
//void IntGlobalEnable(void)
//{
//    __asm("    cpsie   i\n");
//}



//void Interrupt_Init(void)
//{
//  IntEnable(INT_GPIOF);  							// enable interrupt 30 in NVIC (GPIOF)
//	IntPrioritySet(INT_GPIOF, 0x02); 		// configure GPIOF interrupt priority as 0
//	GPIO_PORTF_IM_R |= 0x11;   		// arm interrupt on PF0 and PF4
//	GPIO_PORTF_IS_R &= ~0x11;     // PF0 and PF4 are edge-sensitive
//  GPIO_PORTF_IBE_R &= ~0x11;   	// PF0 and PF4 not both edges trigger 
//  GPIO_PORTF_IEV_R &= ~0x11;  	// PF0 and PF4 falling edge event
//	IntMasterEnable();       		// globally enable interrupt
//}
void pwmInit(Void)
{
//// PWM settings
//PWM_FREQUENCY = 400;
//ui8Adjust = 440;

//SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
//SysCtlPWMClockSet(SYSCTL_PWMDIV_64);

//SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
//SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
//SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE); // connector J1, pin PE4 and PE5
//SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); // connector J3, pin PD0 and PD1

//// PWM PD0, motor A
//GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_0);
//GPIOPinConfigure(GPIO_PD0_M1PWM0);

//// PWM PD1, motor B
//GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_1);
//GPIOPinConfigure(GPIO_PD1_M1PWM1);

//// PWM PE4, motor C
//GPIOPinTypePWM(GPIO_PORTE_BASE, GPIO_PIN_4);
//GPIOPinConfigure(GPIO_PE4_M1PWM2);

//// PWM PE5, motor D
//GPIOPinTypePWM(GPIO_PORTE_BASE, GPIO_PIN_5);
//GPIOPinConfigure(GPIO_PE5_M1PWM3);

//// For pushbuttons
//HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
//HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
//HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
//GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_1, GPIO_DIR_MODE_IN);
//GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

//}
int main(void)
{	
	
		//initialize the GPIO ports	
		PortFunctionInit();
//		Interrupt_Init();
	while(1)
	{ 
		if((GPIO_PORTF_DATA_R&0x01)==0x00)
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, 0x10); //Start Motor 1
		GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, 0x10);
		GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, 0x00);
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, BLUE); //Blue light test
		
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_7, 0x10); //Pulse Motor 2
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, 0x10);
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0x00); 
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GREEN); //Green light test		
		SysCtlDelay(53333);
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_7, 0x00);
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x00); //Green light test	
		
		GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 0x10); //Start Motor 3 Cycle
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0x10); //Move front to back
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, 0x00); 
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, RED); //Red light test	
		SysCtlDelay(2000000);
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0x00); //Move back to front
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, 0x10);
		SysCtlDelay(2000000);
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0x10); //Move front to back
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, 0x00);
		SysCtlDelay(2000000);
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0x00); //Move back to front
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, 0x10);
		SysCtlDelay(2000000);
		GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 0x00); //Start Motor 3 Cycle
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x00); //Red light test
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, 0x00); //Stop Motor 1
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0x00); //Blue light test
		
	}
}
