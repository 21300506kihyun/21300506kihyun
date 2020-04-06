#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>
#include <unistd.h>
#define MAX_STR_SIZE 100
#define BUFF_SIZE 100


typedef struct {
		long  data_type;
		int   data_num;
		char  data_buff[BUFF_SIZE];
}t_data;

int repeat_receiver = 1;
int income = 0;
void *receiver(void *arg){

		int  rcvq_id = *(int*) arg;
		t_data  data;
		char buffer[MAX_STR_SIZE];
		while(repeat_receiver == 1 )
		{					
									// queue id   수신한 데이터 구조 	
				if (msgrcv( rcvq_id, &data, sizeof( t_data) - sizeof( long), 0,IPC_NOWAIT ) > 0)
				{
 						strcpy(buffer, data.data_buff);
 						printf("                          [incoming] :");
						printf("                                       %s", buffer);
 						printf("\n");
 						printf("[msg] :");
						fflush(stdout);

 //						//printf("[msg]");
 						usleep(1000);
						income = 1;
				}
//				else{
//						perror( "msgrcv() 실패");
//						exit( 1);
//				}
		}
		exit(1);
}


void *sender(void *agv){
		char str_read[MAX_STR_SIZE];
		str_read[99] = 0; // for safety
		int      sendq_id =*(int *) agv;
		int		 magsnd;
		int      ndx   = 0;
		t_data   data;

		while( 1 )
		{		
				printf("[msg] :");
				if(income == 0){
				fgets(str_read, MAX_STR_SIZE, stdin);
				str_read[strlen(str_read) -1] = 0; // remove space
				
				 if(strcmp(str_read, "quit") == 0){
					 repeat_receiver = 0;
					 break;
				}

				data.data_type = 1;   // data type을 그냥 하나만
				data.data_num  = ndx++; 
				sprintf( data.data_buff,"%s", str_read);
								// queue is 전송할 자료		
				if ( -1 == msgsnd(sendq_id, &data, sizeof( t_data) - sizeof( long), IPC_NOWAIT))
				{
						perror( "msgsnd() 실패");
						exit( 1);
				}
				}
				income = 0;
		}
}
int main(int argc, char *argv[])      // 옵션의 개수와 옵션 문자열을 배열로 받음
{

	int snd_que , rcv_que;
	//int snd_key ,rcv_key = 0;
		if(argc < 2){
			printf("Usage: ./hw3_example <snd_key> <rcv_key>\n");
			exit(0);
			}
	//printf("%s\n", argv[1]);
	int	snd_key = atoi(argv[1]);
	int rcv_key = atoi(argv[2]);
	printf("snd_key =%d , rcv_key = %d\n", snd_key, rcv_key);
//	/* Message Queue를 생성합니다. */ 
	if((snd_que = msgget(snd_key, IPC_CREAT | 0666)) == -1){
		perror("msgget error");
		return 1;
	}

	if((rcv_que = msgget(rcv_key, IPC_CREAT | 0666)) == -1){
		perror("msgget error");
		return 1;
	}

	pthread_t send;
	pthread_t receive;

	pthread_create(&send,NULL,sender,(void*)&snd_que);
	pthread_create(&receive,NULL,receiver,(void*)&rcv_que);

	pthread_join(send,NULL);
	pthread_join(receive,NULL);

	if (msgctl(snd_que, IPC_RMID, NULL) == -1) { perror("msgctl error"); exit(1); } // deallocate
	if (msgctl(rcv_que, IPC_RMID, NULL) == -1) { perror("msgctl error"); exit(1); }

}
	






















