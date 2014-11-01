/**************************************************************************************************/

/*
 * File: lab3_1B.c
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
#include <stdlib.h>

_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF &
          BKBUG_ON & COE_ON & ICS_PGx1 &
          FWDTEN_OFF & WINDIS_OFF & FWPSA_PR128 & WDTPS_PS32768 )

_CONFIG2( IESO_OFF & SOSCSEL_SOSC & WUTSEL_LEG & FNOSC_PRIPLL & FCKSM_CSDCMD & OSCIOFNC_OFF &
          IOL1WAY_OFF & I2C1SEL_PRI & POSCMOD_XT )

// ******************************************************************************************* //

// variables that are accessed in multiple functions

volatile int state = 0;
volatile int prevState=0;

// ******************************************************************************************* //


/*
 * 
 */
int main(void) {

    
    double percent1 = 0;
    double percent2 = 0;
    double oldpercent1=-1;
    double oldpercent2=-1;
    double oldADVal=-1;


/**********************************************/
    T3CONbits.TCS = 0; // sets up to use internal clock 
    T3CONbits.TGATE = 0; 
    T3CONbits.TON = 0;  // Turn timer 3 off
    TMR3 = 0;           // resets timer 3 to 0

    T3CONbits.TCKPS = 3; // set a prescaler of 8 for timer 2
    PR3 = 575;


/*****************************************************/
    

    OC1CONbits.OCM = 6; // Initialize OCx pin low, compare event forces OCx pin high,
    OC1CONbits.OCTSEL = 1; // using timer 3

    OC1R = OC1RS = PR3;

    OC2CONbits.OCM = 6; // Initialize OCx pin low, compare event forces OCx pin high,
    OC2CONbits.OCTSEL = 1; // using timer 3

    OC2R = OC2RS = PR3;

    //Ports used for output to H-bridge
    //PWM outputs
    RPOR0bits.RP0R = 18;    //10010 - OC1 (Output Compare 1)
    RPOR1bits.RP2R = 19;    //10011 - OC2 (Output Compare 2)
/*****************************************************/
    TRISBbits.TRISB11 = 0;
    LATBbits.LATB11 = 1;
    CNPU1bits.CN15PUE = 1;

    TRISBbits.TRISB10 = 0;
    LATBbits.LATB10 = 1;
    CNPU2bits.CN16PUE = 1;
/*****************************************************/
// Configure TRIS register bits for switch 1 input
	TRISBbits.TRISB5 = 1;

// Configure CN register bits to enable change notifications for switch input.
	CNEN2bits.CN27IE = 1;
        IFS1bits.CNIF = 0;
        IEC1bits.CNIE = 1;
/*****************************************************/

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
    T3CONbits.TON = 1;  // Turn timer 3 on - VVVEEERRRYYY important for comparisons

    while(1)
    {
        while(!IFS0bits.AD1IF);  // wait while the A/D 1 interrupt flag is low
        IFS0bits.AD1IF = 0;     // clear the A/D 1 interrupt flag
        ADC_value = ADC1BUF0;   // stores the current value in the A/D 1 buffer in the ADC_value variable
        sprintf(value, "%6d", ADC_value); // formats value in ADC_value as a 6 character string and stores in in the value character array
        if(oldADVal!=ADC_value){
            LCDMoveCursor(0,0);                 // moves the cursor on the LCD to the home position
            LCDPrintString(value);              // sends value to the LCD print function to display it on the LCD screen
            oldADVal=ADC_value;
        }
        AD_value = (ADC_value * 3.3)/1024;  // converts the binary value of the voltage to the analog value by multiplying by the maximum voltage and dividing by 2^n = 2^10, then stores it in AD_value
        if (AD_value<1.65){
            OC1RS = PR3;
            percent1=100;
            percent2=AD_value/1.65;
            OC2RS =PR3*percent2*.83;
            percent2=percent2*100;
        }
        else if (AD_value>1.65) {
            OC2RS = PR3*.83;
            percent2=100;
            percent1=(AD_value-1.65)/1.65;
            percent1=1-percent1;
            OC1RS =PR3*(percent1);
            percent1=percent1*100;
        }
        else {
            OC1RS=PR3;
            OC2RS=PR3*.83;
            percent1=100;
            percent2=100;
        }
        sprintf(value, "%3.0f", percent1); // formats value in ADC_value as a 6 character string and stores in in the value character array
        if (oldpercent1!=percent1){
            LCDMoveCursor(1,0);                 // moves the cursor on the LCD to the second line
            LCDPrintString(value);              // sends value to the LCD print function to display it on the LCD screen
            oldpercent1=percent1;
        }
        sprintf(value, " %3.0f", percent2); // formats value in ADC_value as a 6 character string and stores in in the value character array
        if (oldpercent2!=percent2){
            LCDMoveCursor(1,3);
            LCDPrintString(value);              // sends value to the LCD print function to display it on the LCD screen
            oldpercent2=percent2;
        }
// Motor switching
        switch(state){

            //State 0: Idle Sate-wait for button press
            case 0:
                LATBbits.LATB10=0;
                LATBbits.LATB11=0;
                break;
            //State 1: Drive forward
            case 1:
                LATBbits.LATB10=1;
                LATBbits.LATB11=0;
                break;
            //State 2: Drive backwards
            case 2:
                LATBbits.LATB10=0;
                LATBbits.LATB11=1;
                break;
        }

    }
return 0;
}


void __attribute__((interrupt,auto_psv)) _CNInterrupt(void)
{
    // Clear CN interrupt flag to allow another CN interrupt to occur.

    while (PORTBbits.RB5==0);
    IFS1bits.CNIF = 0;
    if(state == 0 && prevState !=1){
        state = 1;
        prevState=1;

    }

    else if(state == 0 && prevState==1){
        state = 2;
        prevState=2;
    }
    else {
        state = 0;
    }
}

