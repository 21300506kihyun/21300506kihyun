#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>

#include <unistd.h>
#include <pthread.h>

#define TRUE 1
#define FALSE 0

#include "Console.h"

// use this structure to send parameter to the child threads
typedef struct {
	int width, height;		// screen size
	int x, y;				// current coordinate of the ball
	int dx, dy;				// velocity of the ball
} ThreadParam;

// initialize the parameter for the thread function
void InitParam(ThreadParam *param, int width, int height);

// thread function to move a ball
void* MoveBall(void *vparam);

int repeat = TRUE;

int main(int argc, char *argv[])
{
	int result;
	// get <# of balls> from command line parameter
	int no_thread = 0;
	if(argc > 1)
		no_thread = atoi(argv[1]);
	if(no_thread == 0)				// for safety
		no_thread = 5;				// default value of <# of threads>

	srand(time(NULL));

	int width = getWindowWidth();
	int height = getWindowHeight() - 1;
	int t = 0;						// thread index

	EnableCursor(0);				// hide cursor

	clrscr();
	printf("screen size = %d x %d\n", width, height);
	printf("Press ESC to quit!\n");



	ThreadParam param[no_thread];

	for(int i = 0; i < no_thread; i++){
		InitParam(&param[i], width, height);
	}

	pthread_t p_thread[no_thread];
// TO DO: modify the above code to represent multiple balls
//	1. Declare an array of ThreadParam whose length is no_thread.
//		ex) ThreadParam param[no_thread];
//  2. Initialize each of param[t] by calling InitParam()


	// animate the bouncing ball 
		// if the user presses ESC, terminate
		// DO NOT copy the next 4 lines to MoveBall()
		
		int thd_id = 0;

		for(int j = 0; j <no_thread; j++){  // create thread
			int thd_id = pthread_create(&p_thread[j],NULL, MoveBall,&param[j]);
		}

		if(thd_id < 0){
			perror("thread create error");
			exit(0);
		}
//	
		while(1){
			//	MySleep(1000);
				if(kbhit()){			// check if the a key was pressed
						if(getch() == 27)	// 27: ASCII code of ESC
						repeat = 0;
						break;
				}
		}
	//	}
//	 	int x =TRUE;
//		while(x){
//			MySleep(5000);
//			repeat = 0;
//			x = FALSE;
//		}

		for(int k = 0; k < no_thread; k++){
			pthread_join(p_thread[k],(void *)&result);
		}
		
// TO DO: extend the above animation code to animate multiple balls using threads 

//	1. Move the above while-loop to the 'void* MoveBall(void *vparam);' below.

//	2. Launch threads using MoveBall() function passing &param[t]

//	3. Wait for ESC key
//		while(getch() != 27)
//			MySleep(1000)

//	4. Terminate the child threads by setting repeat to FALSE (0)

//  5. Wait for the child threads to terminate (call pthread_join())

	clrscr();
	gotoxy(1, 1);
	printf("Bye!\n");

	EnableCursor(1);			// enable cursor

	return 0;
}

void InitParam(ThreadParam *param, int width, int height)
{
	// TO DO: implement this function to initialize param

	// fill param->width, param->height using the parameters

	// set x to a random number in [1, width]
	// set y to a random number in [1, height]

	// set dx to a random number in [-3, +3]
	// set dy to a random number in [-2, +2]
	param->width = width;
	param->height = height;


	param->x = rand() % width + 1;
	param->y = rand() % height + 1;
	param->dx = rand() % 7 - 3;		// dx in [-3, +3]
	param->dy = rand() % 5 - 2;		// dy in [-2, +2]
}

void* MoveBall(void *vparam)
{
	ThreadParam *param = (ThreadParam*) vparam;

	// TO DO: implement this function 

	while(repeat){
		// save current coordinate
		int oldx = param->x;
		int oldy = param->y;
		
		// update horizontal coordinate
		int x1 = param->x;
		int y1 = param->y;
		int dy1 = param->dy;
		int dx1 = param->dx; 
		int width1 = param->width;
		int height1 = param->height;

		x1 += dx1;

		// horizontal bouncing
		if(x1 <= 0){
			x1 = 1 + (1 - x1);
			dx1 = -dx1;
		} else if(x1 > width1){
			x1 = width1 - (x1 - width1) - 1;
			dx1 = -dx1;
		}

		// update vertical coordinate
		y1 += dy1;

		// vertical bouncing
		if(y1 <= 0){
			y1 = 1 + (1 - y1);
			dy1 = -dy1;
		} else if(y1 > height1){
			y1 = height1 - (y1 - height1) - 1;
			dy1 = -dy1;
		}

		// delete previous ball
		PrintXY(oldx, oldy, " ");

		// draw new ball
		PrintXY(x1, y1, "*");


		param->x = x1;
		param->y = y1;
		param->dy = dy1;
		param->dx = dx1;
		param->width = width1;
		param->height = height1;

		// delay
		MySleep(50);
	}
	//	move the animation code (the while-loop in the main()) here and modify it to work correctly
	//	delete the if(kbhit()) part

	return NULL;
}

