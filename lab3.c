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
// ******************************************************************************************* //
volatile int done = 0;
// ******************************************************************************************* //
int main(void)
{
    unsigned long int temp;
    int ADC_value;
    char value[8];
    double AD_value;

    unsigned int adcBuff[16], i=0;
    unsigned int * adcPtr;
    LCDInitialize();
    AD1PCFG &= 0xFFDF;
    AD1CON2 = 0x003C;
    AD1CON3 = 0x0D09;
    AD1CON1 = 0x20E4;
    AD1CHS = 0;
    AD1CSSL = 0;
    AD1CON1bits.ADON = 1;
    IEC0bits.AD1IE = 1;
    IFS0bits.AD1IF = 0;

    while(1)
    {
        while(!done);
        done = 0;
        adcPtr = (unsigned int*)(&ADC1BUF0);
        temp = 0;
        for(i=0; i<16; i++)
        {
            adcBuff[i] = *adcPtr++;
            temp = temp +adcBuff[i];
        }

        ADC_value = temp/16;
        sprintf(value, "%6d", ADC_value);
        LCDMoveCursor(0,0);
        LCDPrintString(value);
        AD_value = (ADC_value * 3.3)/1024;
        sprintf(value, "%6.2f", AD_value);
        LCDMoveCursor(1,0);
        LCDPrintString(value);
        AD1CON1bits.SAMP = 1;
    }
return 0;
}

void _ISR _ADC1Interrupt(void)
{
    IFS0bits.AD1IF = 0;
    done=1;
    AD1CON1bits.SAMP = 0;
    
}

