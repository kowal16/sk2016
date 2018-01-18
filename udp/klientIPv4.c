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



int main(int argc,char **argv)
{
    int sockfdUdpS;							/* deskryptor gniazda */
    int sockfdUdpR; 						/* deskryptor gniazda */
   /* int sockfdTcp;			*/			
                         		
    struct sockaddr_in server_addrUdpS; /* struktura przechowujaca dane o adresie odbierania */
    struct sockaddr_in server_addrUdpR;	/* struktura przechowujaca dane o adresie nadawania */
   /* struct sockaddr_in server_addrTcp;	*/

    char wiadomosc[255];             	/* nasz wiadomosc */
	char buffer[252];					/*wiadomosc odebrana*/
    if(argc != 2) 
    {	/* sprawdzenie ile argumentow podano, powinny być 3: nazwa programu, adres ip i rodzaj protokołu */
        printf("%s", "Podano zla ilosc parametrow.\n");
        return -1;
    } 
		
	memset(&server_addrUdpS, 0, sizeof(server_addrUdpS));		/* zerowanie struktury adresu nadawania */
	server_addrUdpS.sin_family = AF_INET;                	/* IPv4 */
	server_addrUdpS.sin_addr.s_addr = inet_addr(argv[1]);/* nadawanie na podany adres ip */
	server_addrUdpS.sin_port = htons(8796); 				/* nadawanie na porcie 8796 */       
		
	memset(&server_addrUdpR, 0, sizeof(server_addrUdpR));		/* zerowanie struktury adresu odbioru */
	server_addrUdpR.sin_family = AF_INET;               	/* IPv4 */
	server_addrUdpR.sin_addr.s_addr = htonl(INADDR_ANY);	/* slucham na wszystkich interfejsach */
	server_addrUdpR.sin_port = htons(8797);   			/* slucham na porcie 8797 */

	/*if(argv[3]==('-u'))								/*obsluga protokolu udp*/
    {*/
		/*printf("UDP \n");*/
		sockfdUdpR = socket(AF_INET, SOCK_DGRAM, 0);          	/* tworze gniazdo udp */
		bind(sockfdUdpR, (struct sockaddr *)&server_addrUdpR, sizeof(server_addrUdpR));	/* "podpinam" je pod konkretny port/adres */	
		sockfdUdpS = socket(AF_INET, SOCK_DGRAM, 0);   			/* tworze gniazdo udp - nie zwiazane z zadnym portem/adresem */  
		
		while(1)
		{
			fgets(wiadomosc, sizeof(wiadomosc), stdin);	/* wczytywanie wiadomosci z klawiatury */
			wiadomosc[strcspn(wiadomosc, "\n")] = 0;		/* porownywanie 2 napisow */
			if((sendto(sockfdUdpS, wiadomosc, sizeof(wiadomosc), 0,(struct sockaddr *)&server_addrUdpS, sizeof(server_addrUdpS)))==-1) /* nadawanie komunikatu */
			{
				perror("Blad wysylania wiadomosci.\n");		/*obsluga bledu*/
			}
			if((recvfrom(sockfdUdpR, buffer, sizeof(buffer), 0, NULL, NULL))==1) /* odczytuje wiadomosc */
			{				
				perror("Blad odebrania wiadomosci.\n");
			}
			else
			{
				printf("%s \n", buffer);		/*wypisanie wiadomosci odeslaniej przez serwer*/
			}
		}
	/*}*/					/*nie dzialajaca obsluga protokolu tcp*/
	/*if(argv[3]=='-t')
	{
		printf("TCP \n");
		sockfdTcp = socket(AF_INET, SOCK_STREAM,0);
		while(1)
		{
			if(connect(sockfdTcp, (struct sockaddr*)&server_addrUdpR, sizeof(server_addrUdpR))<0)
			{	
				perror("Blad polaczenia");
				return -1;
			}
			fgets(wiadomosc, sizeof(wiadomosc), stdin);*/	/* wczytywanie wiadomosci z klawiatury */
			/*wiadomosc[strcspn(wiadomosc, "\n")] = 0;	*/	/* porownywanie 2 napisow */
			/*write(sockfdTcp,wiadomosc, strlen(wiadomosc) + 1);
			read(sockfdTcp, wiadomosc, sizeof(wiadomosc));
			close(sockfdTcp); 
		}
	} */
    return 0;
}
