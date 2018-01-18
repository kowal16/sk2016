#include <time.h>
#include <stdio.h>
#include <unistd.h>

int main(){
	char buffor[64];
	struct tm *czasTM;
	time_t czas;
	
	
		time( & czas);
		czasTM = localtime( & czas);
		strftime(buffor, sizeof(buffor),"%H:%M:%S", czasTM);
		printf("\"%s\"\n", buffor);
		
	
	
	return 0;
}
	
