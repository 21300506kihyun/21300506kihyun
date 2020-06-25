#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

#define BUFFER_SIZE 5
#define MAX_MESSAGE 64
char buffer[BUFFER_SIZE][MAX_MESSAGE];
int in = 0; // buffer에서의 in의 위치
int out = 0; // buffer에서의 out위치
int count = 0; // 현제 buffer안에 있는 msg갯수
pthread_mutex_t mutex;
sem_t empty, full;
int empty_val, full_val = 0; // sem value받을 변수
int repeat = 1;// Producer and Consumer threads terminates


void DisplayBuffer()
{
		printf("Buffer contents:\n");
		printf("\tcount = %d, in = %d, out = %d\n",
						count, in, out);

		int p = out;

		for(int i = 0; i < count; i++){
				printf("\t%d] %s\n", p, buffer[p]);
				p = (p + 1) % BUFFER_SIZE;
		}
}

void *Producer(void* data){
		while(repeat){
				char msg[MAX_MESSAGE];
				sleep(rand() % 3 + 1);
				int no_messages = 10;
				char *messages[64] = {
						"Nice to see you!",
						"Aal izz well!",
						"I love you! God loves you!",
						"God loves you and has a wonderful plan for your life.",
						"God bless you!",
						"You are genius!",
						"Cheer up!",
						"Everything is gonna be okay.",
						"You are so precious!",
						"Hakuna matata!"
				};
				int choose = rand() % no_messages;
				strcpy(msg,messages[choose]);

				printf("[Producer] Created a message \"%s\"\n", msg);
				//entry section
				sem_wait(&empty);
				pthread_mutex_lock(&mutex);
				printf("---------------------- PRODUCER ------------------------\n");
				printf("Producer is entering critical section\n");
				// Critical section
				printf("[Producer] \"%s\" ==> buffer\n", msg);
				strcpy(buffer[in],msg);
				count = (count + 1);
				in = (in + 1) % BUFFER_SIZE;  //buffer의 넣는 시작지점 증가

				DisplayBuffer();


				//exit section
				printf("Producer is leaving critical section\n");
				printf("--------------------------------------------------------\n");
				pthread_mutex_unlock(&mutex);
				sem_post(&full);

		}
}

void *Consumer(void* data){
		while(repeat){
				char msg[MAX_MESSAGE];
				//entry section
				sem_wait(&full);
				pthread_mutex_lock(&mutex);
				printf("---------------------- CONSUMSER ------------------------\n");
				printf("Consumer is entering critical section\n");
				// Critical section

				strcpy(msg, buffer[out]);
				count = (count  - 1) ;
				out = (out + 1) % BUFFER_SIZE;  //buffer에서 꺼내오는 위치
				printf("[Consumer] buffer ==> \"%s\"\n", msg);

				DisplayBuffer();


				//exit section
				printf("Consumer is leaving critical section.\n");
				printf("--------------------------------------------------------\n");
				pthread_mutex_unlock(&mutex);
				sem_post(&empty);

				printf("[Consumer] Consumed a message \"%s.\"\n", msg );
				sleep(rand() % 3 + 2);
		}
}



int main(int argc, char *argv[])
{


		int result = 0;
		int duration = 0;
		pthread_t producer, consumer;

		srand(time(NULL));

		// when main() sets repeat to zero.

		if(argc > 1)
				duration = atoi(argv[1]);
		if(duration == 0)
				duration = 30;              // default value of duration

		// mutext init
		pthread_mutex_init(&mutex,NULL);
		//semaphores sem_init
		if(sem_init(&empty,0,BUFFER_SIZE) < 0){ // empty값 BUFFER_SIZE로 초기화
				printf("sem_init error");
				exit(0);
		};
		if(sem_init(&full,0,0) < 0){ // full값 0로 초기화
				printf("sem_init error");
				exit(0);
		};

		if(pthread_create(&producer,NULL, Producer,NULL)< 0){
				printf("pthread create error");
				exit(0);
		}
		if(pthread_create(&consumer,NULL, Consumer,NULL)< 0){
				printf("pthread create error");
				exit(0);
		}

		DisplayBuffer();

		sleep(duration); //duration동안 기다리기

		repeat = 0; // to terminate producer and Consumer.

		sem_getvalue(&empty,&empty_val);
		if(empty_val== 0){ // 만약 empty값이 0이라면 producer가 계속기다리고 있기 때문에 풀어주기 위해서
				sem_post(&empty);
		}
		sem_getvalue(&full,&full_val);
		if(full_val== 0){ // 만약 empty값이 0이라면 producer가 계속기다리고 있기 때문에 풀어주기 위해서
				sem_post(&full);
		}


		pthread_join(producer,(void *)&result); //Wait for the producer and consumer
		pthread_join(consumer,(void *)&result);

		sem_destroy(&full); // Destroy mutex, full, and empty.
		sem_destroy(&empty);
		pthread_mutex_destroy(&mutex);

		printf("Bye!\n");

}

