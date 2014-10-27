/**************************************************************************************************/

/*
 * File: lab3.c
 * Team: Lambda^3
 * Members: Chris Houseman
 *          Randy Martinez
 *          Rachel Powers
 *          Chris Sanford
 *
 * Date: October 31, 2014
 *
 * Description: Code that used a potentiometer to control two motors
 *
 */

// ******************************************************************************************* //
// ******************************************************************************************* //

#include "p24fj64ga002.h"
#include <stdio.h>
#include "lcd.h"

_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF &
          BKBUG_ON & COE_ON & ICS_PGx1 &
          FWDTEN_OFF & WINDIS_OFF & FWPSA_PR128 & WDTPS_PS32768 )

_CONFIG2( IESO_OFF & SOSCSEL_SOSC & WUTSEL_LEG & FNOSC_PRIPLL & FCKSM_CSDCMD & OSCIOFNC_OFF &
          IOL1WAY_OFF & I2C1SEL_PRI & POSCMOD_XT )

int main(void)
{

    int ADC_value;      // variable to store the binary value in the ADC buffer
    char value[8];      //  character array to store the values to be printed to the LCD
    double AD_value;    // variable to store the calculated value of the voltage

    LCDInitialize();  // initialize the LCD display

    AD1PCFG &= 0xFFDF; // Pin 7, AN5, where the POT is connected, IO6, is set to analog mode, AD module samples pin voltage
    AD1CON2 = 0x0;       // Always uses MUX A input multiplexer settings, configured as one 16-word buffer, interrupts at the completion of conversion for each sample/convert sequence, use the channel selected by the CH0SA bits as the MUX A input
    AD1CON3 = 0x0101;      //set the A/D conversion clock period to be 2*Tcy, set the Auto-Sample Time bits to be 1 T_AD, A/D conversion clock derived from system clock
    AD1CON1 = 0x20E4;   // A/D sample auto-start mode set for sampling begins immediately after last conversion completes, SAMP bit is automatically set, Conversion trigger source set to internal counter (auto-convert), data output format is integer, stop in idle mode set to discontinue module operation when device enters idle mode
    AD1CHS = 5;         // positive input is AN5
    AD1CSSL = 0;        // low reference set to 0

    AD1CON1bits.ADON = 1; // A/D operating mode set to A/D converter module is operating
    IFS0bits.AD1IF = 0;   // clear the A/D 1 interrupt flag

    while(1)
    {
        while(!IFS0bits.AD1IF);  // wait while the A/D 1 interrupt flag is low
        IFS0bits.AD1IF = 0;     // clear the A/D 1 interrupt flag
        ADC_value = ADC1BUF0;   // stores the current value in the A/D 1 buffer in the ADC_value variable
        sprintf(value, "%6d", ADC_value); // formats value in ADC_value as a 6 character string and stores in in the value character array
        LCDMoveCursor(0,0);                 // moves the cursor on the LCD to the home position
        LCDPrintString(value);              // sends value to the LCD print function to display it on the LCD screen
        AD_value = (ADC_value * 3.3)/1024;  // converts the binary value of the voltage to the analog value by multiplying by the maximum voltage and dividing by 2^n = 2^10, then stores it in AD_value
        sprintf(value, "%6.2f", AD_value);  // formats the value in AD_value to 2 decimals places and stores it in the value variable
        LCDMoveCursor(1,0);                 // moves the cursor on the LCD to the second line
        LCDPrintString(value);              // sends value to the LCD print function to display it on the LCD screen
    }
return 0;
}



