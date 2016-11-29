#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>       /* memset() */
#include <arpa/inet.h>    /* htonl() */
#include <unistd.h>

struct msg{
    char name[16];
    char text[255];
};

int main(void){
    int sockfd;                       /* deskryptor gniazda */
    struct sockaddr_in server_addr;   /* adres serwera */
    struct msg wiadomosc;             /* czytana  wiadomosc */

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;                /* IPv4 */
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); /* slucham na wszystkich interfejsach */
    server_addr.sin_port = htons(12211);             /* slucham na porcie 12211 */
    
    /* tworze gniazdo - na razie nie zwiazane z zadnym portem/adresem */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    /* "podpinam" gniazdo pod konkretny port/adres */
    bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    /* teraz gniazdo jest gotowe i moge czytac dane */
    while(1){
        recvfrom(sockfd, &wiadomosc, sizeof(wiadomosc), 0, NULL, NULL);
        printf("[%s] %s\n", wiadomosc.name, wiadomosc.text);
    }

    return 0;
}
