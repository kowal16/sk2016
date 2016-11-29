#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>

int main(int argc, char **argv){
	
	struct hostent *entry = gethostbyname(argv[1]);
struct in_addr **adrs = (struct in_addr**)entry->h_addr_list;
int i = 0;
while (adrs[i] != NULL){
    printf("%s\n", inet_ntoa(*adrs[i]));
    i++;
}	
return 0;
}
