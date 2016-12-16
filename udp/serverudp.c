#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>       /* memset() */
#include <arpa/inet.h>    /* htonl() */
#include <unistd.h>
#include <signal.h>

struct msg{
    char name[16];
    char text[255];
};

int main(int argc,char **argv){
    int sockfd; 
    int pid;                      /* deskryptor gniazda */
    struct sockaddr_in server_addrS; 
    struct sockaddr_in server_addrR;   /* adres serwera */
    struct msg wiadomosc;             /* nasz wiadomosc */
    char ip[16];
    
    if(argc != 2) {						/* sprawdzenie ile argumentow podano, powinny być 3: nazwa programu,nazwa uzytkownika i adres ip */
        printf("%s", "Podano zla ilosc parametrow.\n");
        return -1;
    }  
    strcpy(wiadomosc.name, argv[1]);
    printf("%s podaj ip z jakim chces się połaczyc: ",wiadomosc.name);
    fgets(ip, sizeof(ip), stdin);
    

    if((pid = fork()) < 0) 
    {
        perror("Blad tworzenia potomka.\n");
		kill(pid, SIGINT);
    } 
    
    if(pid>0){
		memset(&server_addrS, 0, sizeof(server_addrS));
		server_addrS.sin_family = AF_INET;                /* IPv4 */
		server_addrS.sin_addr.s_addr = inet_addr(ip); 
		server_addrS.sin_port = htons(8945); 
		sockfd = socket(AF_INET, SOCK_DGRAM, 0);   /*tworze gniazdo - na razie nie zwiazane z zadnym portem/adresem */         
    
		while(1)
		{
			fgets(wiadomosc.text, sizeof(wiadomosc.text), stdin);
			if((sendto(sockfd, &wiadomosc, sizeof(wiadomosc), 0,(struct sockaddr *)&server_addrS, sizeof(server_addrS)))==-1)
			{
				perror("Blad wysylania wiadomosci.\n");
				kill(pid, SIGINT);
			}
		}
	}
		else
		{
			memset(&server_addrR, 0, sizeof(server_addrR));
			server_addrR.sin_family = AF_INET;                /* IPv4 */
			server_addrR.sin_addr.s_addr = htonl(INADDR_ANY); /* slucham na wszystkich interfejsach */
			server_addrR.sin_port = htons(8945);   				/* slucham na porcie 8945 */
			sockfd = socket(AF_INET, SOCK_DGRAM, 0);          /*tworze gniazdo - na razie nie zwiazane z zadnym portem/adresem */
    		
    
			
			bind(sockfd, (struct sockaddr *)&server_addrR, sizeof(server_addrR));	/* "podpinam" gniazdo pod konkretny port/adres */
			
			while(1)
			{
				/* teraz gniazdo jest gotowe i moge czytac dane */
				recvfrom(sockfd, &wiadomosc, sizeof(wiadomosc), 0, NULL, NULL);
				printf("[%s] %s\n", wiadomosc.name, wiadomosc.text);
			}
		}

    return 0;
}
