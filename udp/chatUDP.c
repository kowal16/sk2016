#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>       	/* memset() */
#include <arpa/inet.h>    	/* htonl() */
#include <unistd.h>
#include <signal.h>			/* Biblioteka do obslugi sygnalu kill */
#include <netinet/in.h>		/* Biblioteka dla servera ultra60(freeBSD) */
#include <netdb.h>
#include <net/if.h>
#include <sys/signal.h>

struct msg
{
    char text[255];
};

int main(int argc,char **argv)
{
    int sockfd; 
    int pid;                      		/* deskryptor gniazda */
    struct sockaddr_in server_addrS; 	/* struktura przechowujaca dane o adresie nadawania */
    struct sockaddr_in server_addrR;	/* struktura przechowujaca dane o adresie odbierania */
    struct sockaddr_in rcv_addr;  		/* adres, z ktorego przyszla wiadomosc */
    struct msg wiadomosc;             	/* nasz wiadomosc */
   
    if(argc != 2) 
    {	/* sprawdzenie ile argumentow podano, powinny być 2: nazwa programu, ip lub nazwa hosta */
        printf("%s", "Podano zla ilosc parametrow.\n");
        return -1;
    } 
    
    if((pid = fork()) < 0) 				/* tworzenie potomka */
    {
        perror("Blad tworzenia potomka.\n");
		kill(pid, SIGINT);
    } 
    
    if(pid > 0)
    {	/* przodek jest odpowiedzialny za wysylanie wiadomosci */
		struct hostent *entry = gethostbyname(argv[1]);		/* struktura do ustalania adresu ip na podstawia nazwy */
		struct in_addr **adrs = (struct in_addr**)entry->h_addr_list;
		int i = 0;
		char host_name[100];
		strcpy(host_name, inet_ntoa(*adrs[i]));
		printf("Polaczyles sie z -- %s --\n", host_name);	/* wyswietlanie adresu z jakim sie polaczylismy */
		
		memset(&server_addrS, 0, sizeof(server_addrS));		/* zerowanie struktury adresu */
		server_addrS.sin_family = AF_INET;                	/* IPv4 */
		server_addrS.sin_addr.s_addr = inet_addr(host_name);/* nadawanie na podany adres ip */
		server_addrS.sin_port = htons(12354); 				/* nadawanie na porcie 8945 */
		sockfd = socket(AF_INET, SOCK_DGRAM, 0);   			/* tworze gniazdo udp - nie zwiazane z zadnym portem/adresem */         
    
		while(1)
		{
			fgets(wiadomosc.text, sizeof(wiadomosc.text), stdin);	/* wczytywanie wiadomosci z klawiatury */
			wiadomosc.text[strcspn(wiadomosc.text, "\n")] = 0;		/* porownywanie 2 napisow */
			if((sendto(sockfd, &wiadomosc, sizeof(wiadomosc), 0,(struct sockaddr *)&server_addrS, sizeof(server_addrS)))==-1) /* nadawanie komunikatu */
			{
				perror("Blad wysylania wiadomosci.\n");
				kill(pid, SIGINT);
			}
			if(strcmp(wiadomosc.text,"koniec")==0)	/* gdy wpiszemy "koniec" to zamkniemy nasze polaczenie */
			{
				break;
			}
			
		}
		close(sockfd);				/* zamkniecie socketu */
		kill(pid, SIGINT);			/* zamkniecie procesu */		
	}
	
	if(pid ==0)
	{	/* potomek jest odpowiedzialny za odbieranie wiadomosci */
		memset(&server_addrR, 0, sizeof(server_addrR));		/* zerowanie struktury adresu */
		server_addrR.sin_family = AF_INET;               	/* IPv4 */
		server_addrR.sin_addr.s_addr = htonl(INADDR_ANY);	/* slucham na wszystkich interfejsach */
		server_addrR.sin_port = htons(12354);   			/* slucham na porcie 8945 */
		sockfd = socket(AF_INET, SOCK_DGRAM, 0);          	/* tworze gniazdo udp */
		bind(sockfd, (struct sockaddr *)&server_addrR, sizeof(server_addrR));	/* "podpinam" je pod konkretny port/adres */

		while(1)
		{
			socklen_t rcv_addrlen = sizeof(rcv_addr);		/* okreslenie wielkosci adresu, z ktorego przychodza wiadomosci */
			
			if((recvfrom(sockfd, &wiadomosc, sizeof(wiadomosc), 0, (struct sockaddr *)&rcv_addr, &rcv_addrlen))==-1) /* odczytuje wiadomosc */
			{				
				perror("Blad odebrania wiadomosci.\n");
				kill(pid, SIGINT);	
			}
			
			if(strcmp(wiadomosc.text,"koniec") == 0)		/* gdy wpiszemy "koniec" to zamkniemy nasze polaczenie */
			{
				break;
			}
			
			else
			{	
				printf("[%s] %s\n", (char *)inet_ntoa(rcv_addr.sin_addr), wiadomosc.text); /* wypisanie odebranej wiadomosci */
			}
		}
		
		close(sockfd);				/* zamkniecie socketu */
		kill(pid, SIGINT);			/* zamkniecie procesu */		
	}

    return 0;
}
