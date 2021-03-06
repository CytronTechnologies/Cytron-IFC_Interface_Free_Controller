//=====================================================================================
//
//	Author				:Cytron Technologies		
//	Project				:Interface Free Controller-Control Panel (IFC-CP04)
//	Project description	:Sample for IFC-CP04
//  Date				: 21 April 2008
//
//=====================================================================================

//=====================================================================================
//	include h file for cards
//	User has to include h file for all the cards used 
//=====================================================================================
#include<p18f2685.h>
#include "iic.h"
#include "iic_cp.h"
#include "iic_ai.h"

//=====================================================================================
//	configuration bit, User is advised not to change the setting here
//=====================================================================================
#pragma config OSC = HSPLL
#pragma config FCMEN = OFF 
#pragma config IESO = OFF 
#pragma config PWRT = OFF 
#pragma config BOREN = OFF 
#pragma config WDT = OFF 
#pragma config MCLRE = ON
#pragma config PBADEN = OFF 
#pragma config DEBUG = OFF
#pragma config XINST = OFF
#pragma config LVP = OFF 

//=====================================================================================
//	define input or ouput of board here
//	For example: 
//				1.led1 was defined for first LED on MB00
//				2.buzzer was defined for buzzer on MB00
//	The address for input/output devide on board is fixed, hence user is 
//	advised not to change the defined address for input/output 
//=====================================================================================

#define	led1		LATBbits.LATB0 	// led1 defined as output for first LED on MB00	
#define	led2		LATBbits.LATB1	// led2 defined as output for second LED on MB00
#define	led3		LATBbits.LATB2	// led3 defined as output for third LED on MB00
#define	led4		LATBbits.LATB3	// led4 defined as output for forth LED on MB00
#define	led5		LATBbits.LATB4	// led5 defined as output for fifth LED on MB00
#define	led6		LATBbits.LATB5	// led6 defined as output for sixth LED on MB00
#define	led7		LATBbits.LATB6	// led7 defined as output for seventh LED on MB00
#define	led8		LATBbits.LATB7	// led8 defined as output for eighth LED on MB00
#define	led			LATB			// led defined as output for ALL LED on MB00

#define	sw1			PORTAbits.RA2	// sw1 defined as input for SW1 on MB00
#define	sw2			PORTAbits.RA1	// sw2 defined as input for SW2 on MB00
#define	sw3			PORTAbits.RA0	// sw3 defined as input for SW3 on MB00
#define	buzzer		LATCbits.LATC5	// buzzer defined as output for buzzer on MB00
#define busy_led	LATCbits.LATC1	// busy_led defined as output for Busy LED on MB00
#define error_led	LATCbits.LATC2	// error_led defined as output for Error LED on MB00
#define slave_reset	LATCbits.LATC0	// slave_reset define as output to reset slave cards

//=====================================================================================
//	card address
//	User can define the address of slave card here
//=====================================================================================
#define add_ai1 		0b000011	// Please make sure the communication address on 
									// IFC-AI08 is compatible with the address 

//=====================================================================================
//	function prototype
//	User needs to include all the function prototype here
//	The first 4 functions prototype are fixed, user is also advised not to
//	change it.
//=====================================================================================
void ISRHigh(void);
void ISRLow(void);
void delay(unsigned long data);
void init(void);
	
//=====================================================================================
//	Main Function
//	This is the main function where program start to execute 
//=====================================================================================
void main(void)
{
	//--------------------------------------------------------------------------------
	//	Program start
	//	User can start to write/modify the program here
	//--------------------------------------------------------------------------------

	//Local variable declaration
	unsigned char test[8];
	unsigned short l_test;
	unsigned char cmp;


	//	User is advised not to change or remove the initialization function called 
	init();						// call initialization function of the setting in program

	ai_bit_conf(add_ai1, 0b00010000);	//Port5 is set to 10 bits digital conversion while the rest are 8 bits digital conversion 

	ai_sampling_conf(add_ai1,1,100);		//Refresh rate for analog port 1 and 2 are set to 100x10ms 
	ai_sampling_conf(add_ai1,2,200);		//Refresh rate for analog port 2 and 2 are set to 200x10ms 
											//the rest analog ports (3-8) which have not configure the refresh rate are set to default 1x10ms 

	while(1)
	{
			test[0]=ai_read(add_ai1,1);	//Read the value from analog port 1 and display the value at CP04 control panel card
			cp1_goto(0,0);				//If there is no device plug in the analog port, the value returned are random (not zero!)
			cp1_dec(test[0],3);
			test[1]=ai_read(add_ai1,2);	//Read the value from analog port 2 and display the value at CP04 control panel card
			cp1_goto(0,4);				//If there is no device plug in the analog port, the value returned are random (not zero!)
			cp1_dec(test[1],3);
			test[2]=ai_read(add_ai1,3);	//Read the value from analog port 3 and display the value at CP04 control panel card
			cp1_goto(0,8);				//If there is no device plug in the analog port, the value returned are random (not zero!)
			cp1_dec(test[2],3);
			test[3]=ai_read(add_ai1,4);	//Read the value from analog port 4 and display the value at CP04 control panel card
			cp1_goto(0,12);				//If there is no device plug in the analog port, the value returned are random (not zero!)
			cp1_dec(test[3],3);
			l_test=ai_read(add_ai1,5);	//Read the value from analog port 5 and display the value at CP04 control panel card
			cp1_goto(1,0);				//If there is no device plug in the analog port, the value returned are random (not zero!)
			cp1_dec(l_test,4);
		
			
			cmp=ai_cmphl(add_ai1,0b00001111,1);	//Compare analog port 1 to 4 and figure out the highest value 
			cp1_goto(1,6);						//Display the byte returned at CP04 control panel card 
			cp1_bin(cmp,8);
	}	
}

//=====================================================================================
//	functions
//  User can write all the necessary function here
//=====================================================================================
void delay(unsigned long data)
{
	// this is a delay function for user to use when the program needs a delay
	// this function can be call by typing : delay(xxxx),
	// user can replace the 'xxxx' with a value to determine how long the program
	// should delay for, the bigger the value, the longer the time of the delay
	for( ;data>0;data-=1);
}


void init(void)
{
	// this is a initialization to set the input and output of PIC18F2685
	// User is advised NOT to change the setting here
	// tris
	TRISB=0x00;				// led as output
	TRISA=0xff;				// push button as input
	TRISC=0x00;				// set port c as output
	
	// the setting for analog to digital converter, ADC for PIC18F2685
	// User is advised NOT to change the setting here
	// adc
	ADCON1=0x0F;
	
	//call the initialization function of I2C
	iic_init();		
	
	//	set the initial condition of output device on board
	//	User is advised not to change the initial condition
	led=0x00;				// turn OFF all LED on board by sending value '0x00'
	buzzer=0;				// turn OFF buzzer on board by sending value '0'
	error_led=0;			// turn OFF Error LED on board by sending value '0'
	busy_led=1;				// turn ON Busy LED on board by sending value '1'
	slave_reset=1;			// reset slave cards
	delay(10000);			
	slave_reset=0;			
	delay(50000);			
	busy_led=0;				// turn OFF Busy LED on board by sending value '0'
}


//=====================================================================================
//	Interrupt vector
//=====================================================================================
#pragma	code InterruptVectorHigh = 0x08
void InterruptVectorHigh(void)
{
 	_asm
		goto ISRHigh		// jump to interrupt routine
	_endasm
}
#pragma code
#pragma	code InterruptVectorLow = 0x18
void InterruptVectorLow(void)
{
 	_asm
		goto ISRLow			// jump to interrupt routine
	_endasm
}
#pragma code

//=====================================================================================
//	Interupt Service Routine
//	this a function reserved for interrupt service routine
//	User may need it in advance development of the program
//=====================================================================================
#pragma interrupt ISRHigh
void ISRHigh(void)
{

}

#pragma interrupt ISRLow
void ISRLow(void)
{

}

