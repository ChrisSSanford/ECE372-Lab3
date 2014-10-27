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




/*
 * 
 */
int main(void) {

    
    

/**********************************************/
    T3CONbits.TCS = 0; // sets up to use internal clock 
    T3CONbits.TGATE = 0; 
    T3CONbits.TON = 0;  // Turn timer 3 off
    IFS0bits.T3IF = 0;  // reset timer 3 interrupt flag
    TMR3 = 0;           // resets timer 3 to 0

    T3CONbits.TCKPS = 11; // set a prescaler of 8 for timer 2
    PR3 = 287;
    IEC0bits.T3IE = 1;

/*****************************************************/
    

    OC1CONbits.OCM0 = 1; // Initialize OCx pin low, compare event forces OCx pin high,
    OC1CONbits.OCTSEL = 1; // using timer 3

    OC1R = OC1RS = PR3/2;

    OC2CONbits.OCM0 = 1; // Initialize OCx pin low, compare event forces OCx pin high,
    OC2CONbits.OCTSEL = 1; // using timer 3

    OC2R = OC2RS = PR3/2;





    return (EXIT_SUCCESS);
}

void __attribute__((interrupt,auto_psv)) _T3Interrupt(void){
    IFS0bits.T3IF = 0;


}

