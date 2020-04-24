// If the comments are hard to read because of the color,
// type ':set background=dark'

/***

	This program moves bars left and right until the user types 'q' key.
	Usage: $ ./bar <no_bar>

	Currently, it moves only one bar.
	Extend this program to move multiple bars using multiple threads.
	- Launch multiple threads, each of which moves a bar.
	- The vertical interval between bars should be height / (no_bar + 1).

	Compile with Console.c with -pthread -D_LINUX flag
		Ex) gcc bar.c Console.c -pthread -D_LINUX


***/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <pthread.h>

#include "Console.h"

#define TRUE 1
#define FALSE 0

int repeat = TRUE;

typedef struct {
		int width, height;		// screen size
		char len;
		float interval;
		int x, y;				// current coordinate of the ball
		int dx;			// velocity of the ball
		int no_thread;
		int num;
} ThreadParam;
void InitParam(ThreadParam *param, int width, int height,int no_thread,int i);
void* MoveBar(void *vparam);

void DrawBar(int x, int y, int len, char c);

int main(int argc, char *argv[])
{
	int no_thread = 0;

	if(argc > 1)
		no_thread =atoi(argv[1]);
	if(no_thread == 0)
		no_thread = 5;

	srand(time(NULL));
	int width = getWindowWidth();
	int height = getWindowHeight() - 1;


	
	clrscr();
	gotoxy(1, 1);
	printf("Press 'q' to quit.\n");

	ThreadParam param[no_thread];

	for(int i = 0; i < no_thread; i++){
			InitParam(&param[i], width, height,no_thread,i);
	}

	pthread_t p_thread[no_thread];

	int thd_id = 0;

	for(int j = 0; j <no_thread; j++){  // create thread
			int thd_id = pthread_create(&p_thread[j],NULL, MoveBar,&param[j]);
	}

	if(thd_id < 0){
			perror("thread create error");
			exit(0);
	}

	while(1){
			//	MySleep(1000);
	  if(kbhit() && getch() == 'q'){
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
		pthread_join(p_thread[k],NULL);
	}



	// TO DO: Wait for 'q' key. (use getch() and while statement)

	// TO DO: Stop threads by setting repeat to FALSE

	// TO DO: Wait for the child threads

	gotoxy(1, 1);
	printf("Bye!\n");

//  Do not enable the next line until you complete the program.
	EnableCursor(TRUE);

	return 0;
}

void DrawBar(int x, int y, int len, char c)
{
	pthread_mutex_lock(&display_mutex);

	gotoxy(x, y);
	for(int i = 0; i < len; i++)
		putchar(c);
	fflush(stdout);

	pthread_mutex_unlock(&display_mutex);
}
void InitParam(ThreadParam *param, int width, int height, int no_thread,int i)
{

	param->width = width;
	param->height = height;
	param->num = i;

	param->len = rand() % 6 + 5;	// a random number in [5, 10]
	int step = rand() % 4 + 2;	// a random number in [2, 5]
	param->x = rand() % (width - step - 1) + 1; 

	// vertical interval between bars = height / (no_bar + 1)
	float interval = height / (float)(no_thread + 1);
	param->y = (int) interval*(i+1);

	param->dx = step;
	if(rand() % 2 == 0)
		param->dx = -param->dx;
}

void* MoveBar(void *vparam)
{
		ThreadParam *param = (ThreadParam*) vparam;

		// TO DO: implement this function 


	  
	char c = 'A' + param->num;
	DrawBar(param->x,param->y,param->len,c);
	while(repeat){

			int oldx = param->x;

			// update horizontal coordinate
			int x = param->x;
			int y = param->y;
			int dx = param->dx; 
			int width = param->width;
			int height = param->height;
			char len = param->len;

			x += dx;


		x += dx;
		if(x <= 0){
			dx = -dx;
			x = 1 - x;
		}

		if(x > width - len + 1){
			dx = -dx;
			x = width - len + 1 - (x - width + len - 1);
		}
	
		DrawBar(oldx, y, len, ' ');
		DrawBar(x, y, len, c);

		param->x = x;
		param->y = y;
		param->len = len;
		param->dx = dx;
		param->width = width;
		param->height = height;

		MySleep(50);
	}
		//	move the animation code (the while-loop in the main()) here and modify it to work correctly
		//	delete the if(kbhit()) part

		return NULL;
}
