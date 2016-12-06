#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>       /* memset() */
#include <arpa/inet.h>    /* htonl() */
#include <unistd.h>


int main(){
    int sockfd; 
    char buffer[8192];                      /* deskryptor gniazda */
    struct sockaddr_in server_addr;   /* adres serwera */

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;                /* IPv4 */
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); /* slucham na wszystkich interfejsach */
    server_addr.sin_port = htons(12346);             /* slucham na porcie 12211 */
    
    /* tworze gniazdo - na razie nie zwiazane z zadnym portem/adresem */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    /* "podpinam" gniazdo pod konkretny port/adres */
    bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	listen(sockfd,SOMAXCONN);
	int fd = accept(sockfd,NULL,NULL);
    
    read(fd, buffer, sizeof(buffer));
    printf("odebralem: %s\n", buffer);
    
    write(fd, buffer, strlen(buffer) + 1);
   
	printf("odeslalem: %s\n", buffer);
   
    close(fd);    
   
    return 0;
}
