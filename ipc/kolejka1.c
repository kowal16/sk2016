/*server chyba?*/
#include<stdio.h>
#include<sys/types.h>
#include<sys/msg.h>
#include<sys/ipc.h>
#include<errno.h>


int main(){
	
	key_t klucz = ftok("/tmp/tab_og",6);
	if(klucz < 0){
		perror("Ftok error");
		return -1;
	}
	int id = msgget( klucz, IPC_CREAT | 0600);
	char msg_buffer[100];
	int msg_size = sizeof(msg_buffer);
	
	while(1){
		msgrcv(id,msg_buffer,msg_size,150,1);
		printf("%s",msg_buffer);
	}

return 0;
}
     
