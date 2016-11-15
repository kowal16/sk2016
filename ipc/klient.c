/*klient chyba?*/
#include<stdio.h>
#include<sys/types.h>
#include<sys/msg.h>
#include<sys/ipc.h>
#include<errno.h>

struct my_msg{
	long mtype;
	char wiad[20];
}


int main(){
	
	key_t klucz = ftok("/tmp/tab_og");
	if(klucz < 0){
		perror("Ftok error")
		return -1
	}
	char msg_buffer[100];
	int msg_szie = siezeof(msg_buffer);
	
	while(1){
		msgrcv(klucz,)
       

return 0;
}
~         
