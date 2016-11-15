#include<stdio.h>
#include<sys/types.h>
#include<sys/msg.h>
#include<sys/ipc.h>




int main(){

	key_t klucz = ftok("/tmp/tab_og");
	if(klucz < 0){
		:perror("Ftok error")
		return -1
	}
	while(1){
		int id = msgget( key, IPC_CREAT | 0600);
	

	}

return 0;
}
