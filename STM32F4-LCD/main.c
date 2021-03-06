/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "gfx.h"
#include "stdlib.h"
#include "string.h"
#include "test.h"
#include "math.h"

int uitoa(unsigned int value, char * buf, int max) {
	int n = 0;
	int i = 0;
	unsigned int tmp = 0;

	if (NULL == buf) {
		return -3;
	}

	if (2 > max) {
		return -4;
	}

	i=1;
	tmp = value;
	for (;;) {
		tmp /= 10;
		if (0 >= tmp) {
			break;
		}
		i++;
	}
	if (i >= max) {
		buf[0] = '?';
		buf[1] = 0x0;
		return 2;
	}

	n = i;
	tmp = value;
	buf[i--] = 0x0;
	for (;;) {
		buf[i--] = (tmp % 10) + '0';
		tmp /= 10;
		if (0 >= tmp) {
			break;
		}
	}
	if (-1 != i) {
		buf[i--] = '-';
	}

	return n;
}

static WORKING_AREA(waThread2, 2048);
__attribute__ ((__noreturn__))
static msg_t Thread2(void *arg)  {

  (void)arg;
  chRegSetThreadName("lcd");

	/* Reset */
  palClearPad(GPIOC, GPIOC_PIN6);
  chThdSleepMilliseconds(10);
  palSetPad(GPIOC, GPIOC_PIN6);
  chThdSleepMilliseconds(10);
  GHandle GW1;
	gdispInit();
	gdispClear(Black);

	uint16_t width = gdispGetWidth();
	uint16_t height = gdispGetHeight();

	uint32_t pixels;
	uint32_t i;
	color_t random_color;
	uint16_t rx, ry, rcx, rcy;
	char pps_str[25];
	srand(halGetCounterValue());

	const font_t font1 = gdispOpenFont("UI2 Double");

  while (TRUE) {

	  GW1 = gwinCreateConsole(NULL, 0, 0, gdispGetWidth(), gdispGetHeight()/2, font1);
		gwinSetColor(GW1, Green);
		gwinSetBgColor(GW1, Black);

	  gdispFillArea(10, 10, width-20, height-20, Grey);
	  gdispFillArea(30, 30, 300, 150, Red);
	  gdispFillArea(50, 50, 200, 100, Blue);
	  gdispFillArea(80, 80, 150, 50, Yellow);
	  gdispFillCircle(width/2, height/2, 50, White);

	  const char *msg = "ChibiOS/GFX on SSD1963";
	  gdispDrawString(width-gdispGetStringWidth(msg, font1)-3, height-24, msg, font1, White);
/*
	  chThdSleepMilliseconds(2000);

	  gdispClear(Black);
	    Graph G1 = {
	    	width/2,
	    	height/2,
	        -225,
	        225,
	        -130,
	        130,
	        20,
	        5,
	        TRUE,
	        TRUE,
	        White,
	        Grey,
	    };

	    graphDrawSystem(&G1);

	    uint16_t i;
	    for(i = 0; i < 2500; i++)
	        graphDrawDot(&G1, i-(width/2), 80*sin(2*0.2*M_PI*i/180), 1, Blue);

	    for(i = 0; i < 2500; i++)
	        graphDrawDot(&G1, i/5-(width/2), 95*sin(2*0.2*M_PI*i/180), 1, Green);

	    chThdSleepMilliseconds(1500);

		  gdispClear(Black);
		    Graph G2 = {
		    	100,
		    	100,
		        -100,
		        100,
		        -100,
		        100,
		        20,
		        5,
		        FALSE,
		        TRUE,
		        White,
		        Grey,
		    };

		    Graph G3 = {
		    	width-100,
		    	height-100,
		        -100,
		        100,
		        -100,
		        100,
		        20,
		        5,
		        TRUE,
		        FALSE,
		        White,
		        Grey,
		    };

		    graphDrawSystem(&G2);
		    graphDrawSystem(&G3);

		    for(i = 0; i < 2500; i++) {
		        graphDrawDot(&G2, i-(width/5), 80*sin(2*0.2*M_PI*i/180), 1, Blue);
		        graphDrawDot(&G3, i-(width/5), 80*sin(2*0.2*M_PI*i/180), 1, Blue);
		    }
		    for(i = 0; i < 2500; i++) {
		        graphDrawDot(&G2, i/5-(width/5), 95*sin(2*0.2*M_PI*i/180), 1, Green);
		        graphDrawDot(&G3, i/5-(width/5), 95*sin(2*0.2*M_PI*i/180), 1, Green);
		    }
		    chThdSleepMilliseconds(2000);
*/
	  pixels = 0;
	  gdispClear(Black);
	  gdispDrawString(60, height/2, "Doing 5000 random rectangles", font1, White);
	  chThdSleepMilliseconds(2000);
	  uint32_t start = halGetCounterValue();
	  for (i = 0; i < 5000; i++) {
		  random_color = (rand() % 65535);
		  rx = (rand() % (width-10));
		  ry = (rand() % (height-10));
		  rcx = (rand() % ((width-rx)-10))+10;
		  rcy = (rand() % ((height-ry)-10))+10;

		  gdispFillArea(rx, ry, rcx, rcy, random_color);
		  pixels += (rcx+1)*(rcy+1);
	  }
	  uint32_t ms = (halGetCounterValue()-start) / 168000;
	  uint32_t pps = (float)pixels/((float)ms/1000.0f);

	  memset (pps_str, 0, sizeof(pps_str));
	  uitoa(pps, pps_str, sizeof(pps_str));
	  strcat(pps_str, " Pixels/s");

	  gdispClear(Black);
	  gdispDrawString(100, height/2, pps_str, font1, White);
	  chThdSleepMilliseconds(3000);
  }
}

/*
 * Application entry point.
 */
__attribute__ ((__noreturn__))
int main(void) {

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  /*
   * Creates the example thread.
   */
//  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);
  chThdCreateStatic(waThread2, sizeof(waThread2), NORMALPRIO, Thread2, NULL);
  while (TRUE) {
    chThdSleepMilliseconds(500);
  }
}
