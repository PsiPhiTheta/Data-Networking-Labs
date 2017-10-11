//0. INFORMATION & NOTES_________________________________________________________________________________________________________________

	/////////////////////////
	// Lab: Data Networking	/
	// Name: Thomas Hollis	/
	// Student ID: 9563426  /
	/////////////////////////

	//Port A: Analogue inputs from the rotary potentiometer (RA0), the LM335Z temperature sensor (RA1) and the Light Dependent Resistor (RA2). Digital output to control the LED transistor Q3 (RA4). 5V closed, 0V open. 
	//Port B: Push-button PB2 digital input (RB0).
	//Port C: Toggle switch digital inputs (RC2, RC3, RC4 and RC5). Least significant.
	//Port F: Digital outputs to control 8 LEDs and 7seg displays (RF0 ? RF7). Write 0V to illuminate. 
	//Port H: Digital outputs to control 7-segment transistors Q1 and Q2 (RH0 and RH1). 0V closed, 5V open. Toggle switch digital inputs (RH4, RH5, RH6 and RH7). Most significant.
	//Port J: Push-button PB1 digital input (RJ5). Digital output for controlling the sounder (RJ6).
	//7seg - 0:132, 1:245, 2:76, 3:100, 4:53, 5:38, 6:6, 7:244, 8:4, 9:36, A:20, B:7, C:142, D:132, E:14, F:30

//1. PREPROCESSOR DIRECTIVES____________________________________________________________________________________________________________
    #include "xc_config_settings2.h"
    #include "adc.h"                                                            
    #include "delays.h"
    #include "timers.h"
    #include "int_to_SSD.h"

//2. DEFINITIONS________________________________________________________________________________________________________________________
    #define PB1 PORTJbits.RJ5
    #define PB2 PORTBbits.RB0
    #define SWITCHES (PORTH>>4 & 0x0F)
    //#define PI 3.1415926
	//#define E 2.71828182846
	//#define E0 0.00000000000885
	
//3. FUNCTION PROTOTYPES_______________________________________________________________________________________________________________
    void test_routine();

//4. MAIN______________________________________________________________________________________________________________________________
    void main(void) 
    {
        test_routine();
        
        // setup IO
        ADCON1 = 0x0F; // enable digital I/O
        TRISA = 0b11101111; //turn LED transistor off
        LATA = 0b11101111; //turn LED transistor off
        TRISH = 0b11111110; //turn on 7seg trans
        LATH = 0b11111110; //turn on 7seg trans
        TRISF = 0b00000000; //enable SSDs as outputs
                
        // run
        while(1)
        {
            if(PB1 == 0)
            {
                TRISH = 0b11111110; //turn off 7seg trans
                LATH = 0b11111111; //turn off 7seg trans
            }
            if(PB2 == 0)
            {
                TRISH = 0b11111110; //turn on 7seg trans
                LATH = 0b11111110; //turn on 7seg trans
            }
            LATF = int_to_SSD(SWITCHES);
            Delay10KTCYx(100);
        }
    }

//5. FUNCTIONS_________________________________________________________________________________________________________________________
    void test_routine()
    {
        TRISA = 0b11101111; //turn LED transistor on
        LATA = 0b11111111; //turn LED transistor on
        TRISF = 0b00000000; //enable LEDs as outputs
        LATF = 0x00; // switch all leds off
        Delay10KTCYx(250); 
        LATF = 0xFF; // switch all leds on
        Delay10KTCYx(250); 
        LATF = 0x00; // switch all leds off
        LATA = 0b00000000; //disable LEDs
    }
    
    
    
//6. DEBUG STATUS_________________________________________________________________________________________________________________
    //debugged	

