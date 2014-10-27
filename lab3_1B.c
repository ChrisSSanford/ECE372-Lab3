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



    return (EXIT_SUCCESS);
}

