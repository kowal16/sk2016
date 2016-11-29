#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>       /* memset() */
#include <arpa/inet.h>    /* htonl() */
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


struct msg{
    char name[16];
    char text[255];
};

int main(int argc,char **argv){
    int sockfd;                       /* deskryptor gniazda */
    struct sockaddr_in server_addr;   /* adres serwera */
    struct msg *wiadomosc;             /* czytana  wiadomosc */
     
 
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;                /* IPv4 */
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    server_addr.sin_port = htons(12211);             
    
    /* tworze gniazdo - na razie nie zwiazane z zadnym portem/adresem */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    strncpy(wiadomosc->name, argv[1], 16);
    strncpy(wiadomosc->text, argv[2], 255);
    
    /* teraz gniazdo jest gotowe i moge czytac dane */
    
        sendto(sockfd, &wiadomosc, sizeof(wiadomosc), 0,(struct sockaddr *)&server_addr, sizeof(server_addr));
     
    

    return 0;
}
