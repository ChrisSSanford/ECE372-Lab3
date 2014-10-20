/* 
 * File:   lab3.c
 * Author: christopherhouseman
 *
 * Created on October 20, 2014, 11:33 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include "lcd.h"
#include "p24fj64ga002.h"

// PWM - PIC24 Datasheet pgs. 136-139
// Ex. 14-1 pg. 137

// ******************************************************************************************* //
// Configuration bits for CONFIG1 settings.
//
// Make sure "Configuration Bits set in code." option is checked in MPLAB.
//
// These settings are appropriate for debugging the PIC microcontroller. If you need to
// program the PIC for standalone operation, change the COE_ON option to COE_OFF.

_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF &
		 BKBUG_ON & COE_ON & ICS_PGx1 &
		 FWDTEN_OFF & WINDIS_OFF & FWPSA_PR128 & WDTPS_PS32768 )

// ******************************************************************************************* //
// Configuration bits for CONFIG2 settings.
// Make sure "Configuration Bits set in code." option is checked in MPLAB.

_CONFIG2( IESO_OFF & SOSCSEL_SOSC & WUTSEL_LEG & FNOSC_PRIPLL & FCKSM_CSDCMD & OSCIOFNC_OFF &
		 IOL1WAY_OFF & I2C1SEL_PRI & POSCMOD_XT )

// ******************************************************************************************* //

/*
 * 
 */
int main(void) {

    int ADC_value;

    char value[8];

    double AD_value;

    LCDInitialize();

    AD1PCFG &= 0xFFFE;

    // Overheard Ian saying somthing about COM1 = 84


    AD1CON2 = 0;

    AD1CON3 = 0x0101;

    AD1CON1 = 0x20E4;

    AD1CHS = 0;

    AD1CSSL = 0;

    IFS0bits.AD1IF = 0;

    AD1CON1bits.ADON = 1;

    while(1){

        while(IFS0bits.AD1IF == 0){};

        IFS0bits.AD1IF = 0;

        ADC_value = ADC1BUF0;

        sprintf(value, "%6d", ADC_value);

        LCDMoveCursor(0,0);

        LCDPrintString(value);

        AD_value = (ADC_value * 3.3)/1024;

        sprintf(value, "%6.2f", AD_value);

        LCDMoveCursor(1,0);

        LCDPrintString(value);

    }

    return 0;
}

