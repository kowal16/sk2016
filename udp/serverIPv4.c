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
#include <time.h>			/* Biblioteka do odczytywania czasu */




int main(int argc,char **argv)
{
    int sockfdUdpR;							/* deskryptory gniazd */
    int sockfdUdpS;							/* deskryptory gniazd */
    /*int sockfdTcp;  */                 		
    struct sockaddr_in server_addrUdpR; 	/* struktura przechowujaca dane Udp odebrane */
    struct sockaddr_in server_addrUdpS;		/* struktura przechowujaca dane Udp wysylane */
    char wiadomosc [255];				/* nasz wiadomosc */
    char bufforCzas[64];				/* przechowyjemy tu czas */
    /*char prot[2];*/
    char blad[]="blad zapytania";		/* komunikat o zlym zapytaniu */
	struct tm *czasTM;
	time_t czas;   
	          	
   
    if(argc != 1) 
    {	/* sprawdzenie ile argumentow podano, powinny być 1: nazwa programu*/
        printf("%s", "Podano zla ilosc parametrow.\n");
        return -1;
    } 
    /*strcpy(prot, argv[3]);*/
 
	memset(&server_addrUdpR, 0, sizeof(server_addrUdpR));		/* zerowanie struktury adresu */
	server_addrUdpR.sin_family = AF_INET;               	/* IPv4 */
	server_addrUdpR.sin_addr.s_addr = htonl(INADDR_ANY);	/* slucham na wszystkich interfejsach */
	server_addrUdpR.sin_port = htons(8796);   			/* slucham na porcie 8796 */
	
		
	memset(&server_addrUdpS, 0, sizeof(server_addrUdpS));		/* zerowanie struktury adresu */
	server_addrUdpS.sin_family = AF_INET;                	/* IPv4 */
	server_addrUdpS.sin_addr.s_addr = inet_addr("127.0.0.1");/* nadawanie na podany adres ip */
	server_addrUdpS.sin_port = htons(8797); 				/* nadawanie na porcie 8797 */
	 
   /* if(prot =='u')		/* obsluga protokolu udp */
    {*/
		/*printf("UDP \n"); */
		sockfdUdpR = socket(AF_INET, SOCK_DGRAM, 0);          	/* tworze gniazdo udp */
		bind(sockfdUdpR, (struct sockaddr *)&server_addrUdpR, sizeof(server_addrUdpR));	/* "podpinam" je pod konkretny port/adres */	
		sockfdUdpS = socket(AF_INET, SOCK_DGRAM, 0);   			/* tworze gniazdo udp - nie zwiazane z zadnym portem/adresem */        
		while(1)
		{	
			if((recvfrom(sockfdUdpR, wiadomosc, sizeof(wiadomosc), 0, NULL, NULL))==-1) /* odczytuje wiadomosc */
			{				
				perror("Blad odebrania wiadomosci.\n");	
			}
			
			if(strcmp(wiadomosc,"czas") == 0)		/* gdy wpiszemy "czas" to odeslemy godzine */
			{
				time( & czas);
				czasTM = localtime( & czas);		/* pobieranie czasu */
				strftime(bufforCzas, sizeof(bufforCzas),"%H:%M:%S", czasTM);	/* formatowanie czasu */
				if(sendto(sockfdUdpS, bufforCzas, sizeof(bufforCzas), 0,(struct sockaddr *)&server_addrUdpS, sizeof(server_addrUdpS))==-1)
				{	/* odsyłanie czasu */
					perror("Blad wyslania wiadomosci.\n");
				}
			}
			else
			{	/* komunikat o blednym zapytaniu */
				sendto(sockfdUdpS, blad, sizeof(blad), 0,(struct sockaddr *)&server_addrUdpS, sizeof(server_addrUdpS));
			}
		}		/*nidzialajaca obsluga protokolu tcp */
	/*}
	if(prot =='t')
	{
		printf("TCP \n");
		sockfdTcp = socket(AF_INET, SOCK_STREAM,0);
		bind(sockfdUdpR, (struct sockaddr *)&server_addrUdpR, sizeof(server_addrUdpR));*/	/* "podpinam" je pod konkretny port/adres */
		/*listen(sockfdTcp,SOMAXCONN);
		while(1)
		{
			int fd = accept(sockfdTcp,NULL,NULL);
			read(fd,wiadomosc,sizeof(wiadomosc));
			
			if(strcmp(wiadomosc,"czas") == 0)*/		/* gdy wpiszemy "czas" to odeslemy godzine */
			/*{
				time( & czas);
				czasTM = localtime( & czas);
				strftime(bufforCzas, sizeof(bufforCzas),"%H:%M:%S", czasTM);
				write(sockfdTcp,bufforCzas, strlen(bufforCzas) + 1);
				close(fd);
			}
			else
			{
				write(sockfdTcp, blad, strlen(blad) +1);
			}
		}	
	}
	if(prot!='t'||'u')
	{
		printf("niepoprawny protokol");
	}*/
    return 0;
}
