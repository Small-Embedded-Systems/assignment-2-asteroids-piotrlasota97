/* Asteroids view
*/

/* C libraries */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

/* hardware platform libraries */
#include <display.h>
#include <mbed.h>

#include "asteroids.h"
#include "model.h"
#include "utils.h"

Display *graphics = Display::theDisplay();

const colour_t background = rgb(0,0,0);
const colour_t sidebar = rgb(107,115,221);


const coordinate_t shape[] = {
    {10,0}, {-5,5}, {-5,-5}
};

/* double buffering functions */
void init_DBuffer(void)
{   /* initialise the LCD driver to use second frame in buffer */
    uint16_t *bufferbase = graphics->getFb();
    uint16_t *nextbuffer = bufferbase+ (480*272);
    LPC_LCD->UPBASE = (uint32_t)nextbuffer;
}

void swap_DBuffer(void)
{   /* swaps frames used by the LCD driver and the graphics object */
    uint16_t *buffer = graphics->getFb();
    graphics->setFb( (uint16_t*) LPC_LCD->UPBASE);
    LPC_LCD->UPBASE = (uint32_t)buffer;
}

/* This draws the sidebar, which includes, the score, elapsed time, lives and the previous score */
void draw_sidebar(int e_t, int sc, int li) {
	graphics->fillRect(0, 0, 80, 275, sidebar);
	  graphics->setTextColor(WHITE, sidebar);
		graphics->setCursor(5, 10);
			graphics->printf("Assignment:");
		graphics->setCursor(5, 30);
			graphics->printf("Asteroids");
		graphics->setCursor(27, 80);
			graphics->printf("time");
		graphics->setCursor(35, 90);
	    graphics->printf("%d", e_t);
		graphics->setCursor(25, 120);
	    graphics->printf("score:");
		graphics->setCursor(35, 130);
	    graphics->printf("%d", sc);
		graphics->setCursor(25, 160);
		  graphics->printf("lives:");
		graphics->setCursor(35, 170);
	    graphics->printf("%d", li);
		graphics->setCursor(17, 200);
		  graphics->printf("previous");
		graphics->setCursor(25, 210);
			graphics->printf("score:");
		graphics->setCursor(35, 220);
	    graphics->printf("%d", prevscore);
			
			if (paused) {
				graphics->setCursor(180, 90);
					graphics->setTextColor(WHITE, background);
					graphics->printf("To begin press the centre of the joystick");
			}
}

/* Draw ship based on heading,
	 the ship will change colour to red if he has lost a life */
void draw_ship(ship player) {
	if (player.heading == 0) { 
		graphics->fillTriangle(player.p.x, player.p.y-4,
		player.p.x-2, player.p.y+2,
		player.p.x+2, player.p.y+2,
		WHITE);
				if (player.lostlife == 1) {
			graphics->fillTriangle(player.p.x, player.p.y-4,
			player.p.x-2, player.p.y+2,
			player.p.x+2, player.p.y+2,
			RED);
			}
	}
	
	if (player.heading == 1) { 
		graphics->fillTriangle(player.p.x-3, player.p.y-3,
		player.p.x, player.p.y+3,
		player.p.x+3, player.p.y,
		WHITE);
			if (player.lostlife == 1) {
				graphics->fillTriangle(player.p.x-3, player.p.y-3,
				player.p.x, player.p.y+3,
				player.p.x+3, player.p.y,
				RED);
			}
	}

	if (player.heading == 2) { 
		graphics->fillTriangle(player.p.x-4, player.p.y,
		player.p.x+2, player.p.y+2,
		player.p.x+2, player.p.y-2,
		WHITE);			  
			if (player.lostlife == 1) {
				graphics->fillTriangle(player.p.x-4, player.p.y,
				player.p.x+2, player.p.y+2,
				player.p.x+2, player.p.y-2,
				RED);
			}
	}

	if (player.heading == 3) { 
		graphics->fillTriangle(player.p.x-3, player.p.y+3,
		player.p.x+3, player.p.y,
		player.p.x, player.p.y-3,
		WHITE);
			if (player.lostlife == 1) {
				graphics->fillTriangle(player.p.x-3, player.p.y+3,
				player.p.x+3, player.p.y,
				player.p.x, player.p.y-3,
				RED);
			}
	}

	if (player.heading == 4) { 
		graphics->fillTriangle(player.p.x, player.p.y+4,
		player.p.x+2, player.p.y-2,
		player.p.x-2, player.p.y-2,
		WHITE);
			if (player.lostlife == 1) {
				graphics->fillTriangle(player.p.x, player.p.y+4,
				player.p.x+2, player.p.y-2,
				player.p.x-2, player.p.y-2,
				RED);
			}
	}

	if (player.heading == 5) { 
		graphics->fillTriangle(player.p.x+3, player.p.y+3,
		player.p.x, player.p.y-3,
		player.p.x-3, player.p.y,
		WHITE);
			if (player.lostlife == 1) {
				graphics->fillTriangle(player.p.x+3, player.p.y+3,
				player.p.x, player.p.y-3,
				player.p.x-3, player.p.y,
				RED);
			}
	}

	if (player.heading == 6) { 
		graphics->fillTriangle(player.p.x+4, player.p.y,
		player.p.x-2, player.p.y-2,
		player.p.x-2, player.p.y+2,
		WHITE);
			if (player.lostlife == 1) {
				graphics->fillTriangle(player.p.x+4, player.p.y,
				player.p.x-2, player.p.y-2,
				player.p.x-2, player.p.y+2,
				RED);
			}
	}

	if (player.heading == 7) { 
		graphics->fillTriangle(player.p.x+3, player.p.y-3,
		player.p.x-3, player.p.y,
		player.p.x, player.p.y+3,
		WHITE);
			if (player.lostlife == 1) {
				graphics->fillTriangle(player.p.x+3, player.p.y-3,
				player.p.x-3, player.p.y,
				player.p.x, player.p.y+3,
				RED);
			}
	}
}

void drawmissiles(struct missile *lst)
{
    // while there are missiles to draw
    while(lst) {
        graphics->fillCircle(lst->p.x,lst->p.y, 1, GREEN);
        lst = lst->next; // iterate along the list
    }
}

void drawasteroids(struct asteroid *lst)
{
    // while there are asteroids to draw
    while(lst) {
      graphics->fillCircle(lst->p.x,lst->p.y, 20, BLUE);
			lst = lst->next; // iterate along the list
    }
}

void draw(void)
{
    graphics->fillScreen(background);
	
		draw_ship(player);
		drawmissiles(shots);
		drawasteroids(asteroids);
		draw_sidebar(elapsed_time, score, lives);
	
    swap_DBuffer();
}
