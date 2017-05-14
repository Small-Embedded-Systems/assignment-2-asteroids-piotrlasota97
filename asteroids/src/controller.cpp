/* Controller */

/* C libraries */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* hardware platform libraries */
#include <mbed.h>

/* asteroids */
#include "model.h"
#include "asteroids.h"

// Joystick 5-way switch
typedef enum {left, down, right, up, centre} btnId_t;
DigitalIn joystick[] = {P5_0, P5_1, P5_4, P5_2, P5_3};
bool jsPrsdAndHld(btnId_t b);

/*This starts the process which makes the missiles to fire */
void fire() {
	missile_system();
}

/* The velocity and the heading is changed when joystick input is found */
void controls(void)
{
	if ((jsPrsdAndHld(up) && jsPrsdAndHld(right))) { // This will take the ship in the North-West direction
		player.v.x = -1; player.v.y = -1;
		player.heading = 1;
	}
	else if ((jsPrsdAndHld(right) && jsPrsdAndHld(down))) { //This will take the ship in the south-west direction
		player.v.x = -1; player.v.y = 1;
		player.heading = 3;
	}
	else if ((jsPrsdAndHld(down) && jsPrsdAndHld(left))) { // This will take the ship in the south-east direction
		player.v.x = 1; player.v.y = 1;
		player.heading = 5;
	}
	else if ((jsPrsdAndHld(left) && jsPrsdAndHld(up))) { // This will take the ship in the north-east direction
		player.v.x = 1; player.v.y = -1;
		player.heading = 7;
	}
	else if (jsPrsdAndHld(up)) { // This will take the ship north
		player.v.y = -1;
		player.heading = 0;
	}
	else if (jsPrsdAndHld(right)) { // This will take the ship west
		player.v.x = -1;
		player.heading = 2;
	}
	else if (jsPrsdAndHld(down)) { // This will take the ship south
		player.v.y = 1;
		player.heading = 4;
	}
	else if (jsPrsdAndHld(left)) { // This will take the ship east
		player.v.x = 1;
		player.heading = 6;
	}
	
	/* Joystick centre unpauses the game if it's paused
	   If the user is in play it just shoots */
	else if (jsPrsdAndHld(centre)) {
		if (paused) {
			paused = false;
		}
		else if (!paused) {
			fire();
		}
	}
	
	// No input from the user
	else {	
		player.v.x = 0; player.v.y = 0;
	}
}





/* Definition of Joystick press capture function
 * b is one of JLEFT, JRIGHT, JUP, JDOWN - from enum, 0, 1, 2, 3
 * It returns as true if the Joystick is pressed, otherwise it is false
 *
 * If the button's pin value is 0 then this means it is being pressed,
 * remember this in the savedState
 * If the button's pin value is 1 then this means it is being released,
 * if the savedState of the button is 0, then it is true, but
 * if it's 1 it is false. */
bool jsPrsdAndHld(btnId_t b) { // changed from pressed and released to held
	bool result = false;
	uint32_t state;
	static uint32_t savedState[5] = {1,1,1,1,1};
        
	state = joystick[b].read();
  if ((savedState[b] == 0) && (state == 0)) {
		result = true;
	}
	savedState[b] = state;
	return result;
}