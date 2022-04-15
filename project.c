/*
 * Snake Game
 *
 * CSE 20311
 * By: Jose Rodriguez
 */

#include <stdio.h>
#include <stdlib.h>
#include "gfx.h"

typedef struct { // snake position + direction
	int x;
	int y;
	int x_direction;
	int y_direction;
} Snake;

// function prototypes
void drawBorder(int, int, int);
void drawPixel(float,float,float);
int doesTouch(int,int, int, int, int);

int main(){
	int width = 500, height = 500, margin = 100; // gfx window sizes
	int xc = width/2, yc = height/2, x_rand, y_rand; // snake head + food
	char c, str[(width-2*margin)*(height-2*margin)];
	int state = 0, score = 0, r = 2;
	int i, j, sizeSnake = 0;
	int isEat = 0, isCanibal = 0; 
	Snake player[width*height]; // snake is a structured list

	// open gfx window
	gfx_open(width,height,"The Snake Game");
	drawBorder(width, height, margin);
	gfx_color(255,255,255);


	while (c != 'q') { // loop until user quits
		srand(time(0));
		gfx_clear();
		gfx_color(255,255,255);

		// end game if snake touches border
		if (player[0].x >= width-margin || player[0].x <= margin) state = 0;
		if (player[0].y >= height-margin || player[0].y <= margin) state = 0;

		// do not start game while state = 0
		while (state == 0) {
			gfx_text(xc-25,yc,"Press (p) to Play");
			if (gfx_wait() == 'p') state = 1; // begin if user presses p
			if (gfx_wait() == 'q') return 0; // leave if user chooses q
			sizeSnake = 0; // snake is intialized to just the head
			// snake begins moving to the right
			player[0].x_direction = 1;
			player[0].y_direction = 0;
			// snake begins in center
			player[0].x = xc;
			player[0].y = yc;
			// random position of food
			x_rand = (margin+2*r) + rand() % (width-(2*margin+2*r));
			y_rand = (margin+2*r) + rand() % (height-(2*margin+2*r));
			// initialize score
			score = 0;
			// snake has not eaten food or itself
			isEat = 0;
			isCanibal = 0;
		}

		// display score
		sprintf(str,"Score: %d", score);
		gfx_text(margin,margin-1,str);

		// draw border of snake game
		drawBorder(width, height, margin);

		// draw pixels in snake
		for (i = 0; i <= sizeSnake; i++) {
			drawPixel(player[i].x,player[i].y,r);
		}

		// draw green food
		gfx_color(0,155,0);
		drawPixel(x_rand,y_rand,r);
		gfx_color(255,255,255);

		gfx_flush();
		usleep(10000);

		// use wasd characters to change direction of snake
		if (c == 'd') {
			gfx_clear();
			if (player[0].x_direction != -1){
				player[0].x_direction = 1;
				player[0].y_direction = 0;
			}
		} else if (c == 'w') {
			if (player[0].y_direction != 1) {
				player[0].x_direction=0;
				player[0].y_direction=-1;
			}
		} else if (c == 'a'){
			if (player[0].x_direction != 1) {
				player[0].x_direction=-1;
				player[0].y_direction=0;
			}
		} else if (c == 's') {
			if (player[0].y_direction != -1) {
				player[0].x_direction=0;
				player[0].y_direction=1;
			}
		}

		// snake element gets pos and direction for previous element
		for (i = sizeSnake; i > 0; i--) {
			player[i].x = player[i-1].x;
			player[i].y = player[i-1].y;
			player[i].x_direction = player[i-1].x_direction;
			player[i].y_direction = player[i-1].y_direction;
		}

		// check to see if snake touches any pixels
		for (i = 10; i <= sizeSnake; i++) { // anything before is impossible to touch
			// if head touches elment, set isCanibal to 1
			isCanibal = doesTouch(player[0].x,player[0].y,player[i].x,player[i].y,r);
			// if isCanibal is 1, state is 0 and current game ends
			if (isCanibal == 1) state = 0;
		}

		// increase position of snake head
		player[0].x+=player[0].x_direction;
		player[0].y+=player[0].y_direction;

		// see if snake has eaten food
		isEat = doesTouch(player[0].x,player[0].y, x_rand, y_rand, r);

		if (isEat == 1){ // if food has been eaten
			// random position for new food
			x_rand = (margin+2*r) + rand() % (width-(2*margin+2*r));
			y_rand = (margin+2*r) + rand() % (height-(2*margin+2*r));
			score++; // increment score

			for (i = 0; i <= 2*r; i++) { // increase snake size
				sizeSnake++;
				player[sizeSnake].x_direction=player[sizeSnake-1].x_direction;
				player[sizeSnake].y_direction=player[sizeSnake-1].y_direction;
			}
			isEat = 0;
		}

	if (gfx_event_waiting()){
		c = gfx_wait();}	
	}

	return 0;
}

void drawBorder(int x, int y, int margin){ // draws rectangular border
	gfx_line(margin,margin,x-margin,margin);
	gfx_line(margin,margin,margin,y-margin);
	gfx_line(x-margin,margin,x-margin,y-margin);
	gfx_line(margin,y-margin,x-margin,y-margin);
}

// recursive function draws filled in square
void drawPixel(float xc, float yc, float r){
	if (r < 0.5) return;
	gfx_line(xc-r,yc-r,xc+r,yc-r);
	gfx_line(xc+r,yc-r,xc+r,yc+r);
	gfx_line(xc+r,yc+r,xc-r,yc+r);
	gfx_line(xc-r,yc+r,xc-r,yc-r);
	drawPixel(xc,yc,r-0.5);
}

int doesTouch(int x1, int y1, int x2, int y2, int r) { // checks if two squares touch, returns 1 id they do.
	int isEat = 0;
	if ((y1 + r == y2-r) && ((x1+r>=x2-r) && (x1-r <= x2+r))){
		isEat = 1;
	}
	if ((x1+r == x2-r) && ((y1+r>=y2-r) && (y1-r<=y2+r))){
		isEat = 1;
	}
	if ((x1-r == x2+r) && ((y1+r>=y2-r) && (y1-r<=y2+r))){
		isEat = 1;
	}
	if ((y1 - r == y2+r) && ((x1+r>=x2-r) && (x1-r <= x2+r))){
		isEat = 1;
	}

	return isEat;
}

