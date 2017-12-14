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
#include "p18f8722.h"
#include "adc.h"
#include "i2c.h"
#include "delays.h"
#include "stdio.h"
#include "stdlib.h"
#include "int_to_SSD.h"

//2. DEFINITIONS________________________________________________________________________________________________________________________
//#define PI 3.1415926
//#define E 2.71828182846
//#define E0 0.00000000000885
#define PB1 PORTJbits.RJ5
#define PB2 PORTBbits.RB0
#define SWITCHES (PORTH>>4 & 0x03)
#define EDIT PORTCbits.RC2

//3. GLOBAL VARIABLES______________________________________________________________________________________________________________
unsigned char buffer[4] = {0, 0, 0, 0};
char EEPROMaddress = 0x0D4;
int valuePotentiometer = 0;
int i = 0;

//4. FUNCTIONS__________________________________________________________________________________________________________________________
void config_ADC(void) //configures ADC
{
    OpenADC(ADC_FOSC_16 & ADC_RIGHT_JUST & ADC_0_TAD, ADC_CH0 & ADC_INT_OFF & ADC_VREFPLUS_VDD & ADC_VREFMINUS_VSS, 0x0E);
    ADCON1 = 0x0F;
    TRISF = 0x00;
}

void config_I2C(void) //configures I2C
{
    OpenI2C1(MASTER, SLEW_OFF);
    SSP1ADD = 0x18;
}

int read_potentiometer(void) //reads value from potentiometer
{
    ConvertADC();
    while(BusyADC());
    return ReadADC()>> 2;
}

void display_SSD(int value) //displays value on seven segment display
{
    LATF = int_to_SSD((value & 0xF0) >> 4);
    LATH = 0xFE;
    TRISH = 0xFE;
    Delay10TCYx(200);
}

void read_EEPROM(char address) //reads from EEPROM memory chip
{
    //Step 1 - Move address pointer of EEPROM to right position
    StartI2C1(); //begin I2C connection
    WriteI2C1(0xA0); //control
    WriteI2C1(0x00); //MSB of address
    WriteI2C1(address); //LSB of address
    StopI2C1(); //stop I2C connection

    //Step 2 - Read data sequentially
    StartI2C1(); //begin I2C connection
    WriteI2C1(0xA1); //control
    buffer[0] = ReadI2C1(); //read from EEPROM memory and increment pointer
    AckI2C1();
    buffer[1] = ReadI2C1(); //read from EEPROM memory and increment pointer
    AckI2C1();
    buffer[2] = ReadI2C1(); //read from EEPROM memory and increment pointer
    AckI2C1();
    buffer[3] = ReadI2C1(); //read from EEPROM memory and increment pointer
    StopI2C1(); //stop I2C connection
}

void write_EEPROM(char address) //writes to EEPROM memory chip
{
    //Step 1 - Write data sequentially
    StartI2C1(); //begin I2C connection
    WriteI2C1(0xA0); //control
    WriteI2C1(0x00); //MSB of address
    WriteI2C1(address); //LSB of address
    WriteI2C1(buffer[0]); //write buffer value to EEPROM memory and increment pointer
    WriteI2C1(buffer[1]); //write buffer value to EEPROM memory and increment pointer
    WriteI2C1(buffer[2]); //write buffer value to EEPROM memory and increment pointer
    WriteI2C1(buffer[3]); //write buffer value to EEPROM memory and increment pointer
    StopI2C1(); //stop I2C connection
}

void clear_buffer(void) //clears the contents of the buffer
{
    buffer[0] = 0;
    buffer[1] = 0;
    buffer[2] = 0;
    buffer[3] = 0;
}

//5. MAIN__________________________________________________________________________________________________________________________
void main(void)
{
    config_ADC(); //configures ADC
    config_I2C(); //configures I2C
    while(1) //execute for ever
    {
        if(EDIT) //if edit switch is 1
        {
            valuePotentiometer = read_potentiometer(); //updates valuePotentiometer variable
            if(!PB1)
            {
                buffer[i] = valuePotentiometer; //adds chosen value to buffer
                i++; //moves on to next value
                if(i == 4)
                {
                    i = 0; //loops in case PB1 is pressed more than 4 times
                }
                Delay10KTCYx(175); //debounces PB1
                Delay10KTCYx(175); //debounces PB1
            }
            display_SSD(valuePotentiometer); //displays current potentiometer value on seven segment display
        }
        else
        {
            if(!PB1) //if PB1 is pressed
            {
                write_EEPROM(EEPROMaddress); //writes to EEPROM memory chip
                clear_buffer(); //clears the contents of the buffer
                Delay10KTCYx(175); //debounces PB1
                Delay10KTCYx(175); //debounces PB1
            }
            if(!PB2) //if PB2 is pressed
            {
                read_EEPROM(EEPROMaddress); //reads from EEPROM memory chip
                Delay10KTCYx(175); //debounces PB2
                Delay10KTCYx(175); //debounces PB2
            }
            display_SSD(buffer[SWITCHES]); //displays value of buffer (switch chooses address) on seven segment display
        }
    }
}

//5. DEBUG STATUS_________________________________________________________________________________________________________________
    //debugged
