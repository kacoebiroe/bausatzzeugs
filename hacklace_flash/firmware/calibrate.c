/*
 * Hacklace.c
 *
 */ 

/**********************************************************************************

Title:				Hacklace - A necklace for hackers

Hardware:			Hacklace-Board with ATtiny4313 running at 4 MHz and a
					5 x 7 dot matrix display.
Author:				Frank Andre
License:			This software is distributed under the creative commons license
					CC-BY-NC-SA.
Disclaimer:			This software is provided by the copyright holder "as is" and any 
					express or implied warranties, including, but not limited to, the 
					implied warranties of merchantability and fitness for a particular 
					purpose are disclaimed. In no event shall the copyright owner or 
					contributors be liable for any direct, indirect, incidental, 
					special, exemplary, or consequential damages (including, but not 
					limited to, procurement of substitute goods or services; loss of 
					use, data, or profits; or business interruption) however caused 
					and on any theory of liability, whether in contract, strict 
					liability, or tort (including negligence or otherwise) arising 
					in any way out of the use of this software, even if advised of 
					the possibility of such damage.
					
**********************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>

#include "calibration-data.h"

/*********
* fuses *
*********/

FUSES =
{
	.low = 0xE2,
	.high = 0xDF,
	.extended = 0xFF,
};

ISR(TIMER1_OVF_vect) {
    PORTD ^= (1<<PD0);
}


int main(void)
{
    DDRD = (1<<PD0);

	sei(); // enable interrupts

    // Use default timer mode (no compare match, no pwm)
    TCCR1A = 0;
    // Clock select 001 for CPU_FREQ
    TCCR1B = (0<<CS12) | (0<<CS11) | (1<<CS10);


    TIMSK = (1<<TOIE1);

    OSCCAL = EXT_OSCCAL;

    while(1) {
    } // of while(1)
}


