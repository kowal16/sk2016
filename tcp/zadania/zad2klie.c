#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>

int main() {
    int fd;
    struct sockaddr_in srv_addr;
    char buffer[8192];
    char wiad[] = "SiemkaPL"; 
    
    
    
    fd = socket(AF_INET, SOCK_STREAM, 0);
    
    bzero(&srv_addr, sizeof(srv_addr));
    srv_addr.sin_family      = AF_INET;
    srv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    srv_addr.sin_port        = htons(12346);
    
    if (connect(fd, (struct sockaddr*)&srv_addr, sizeof(srv_addr)) < 0){
        perror("blad polaczenia");
        return -1;
    }
    
   write(fd, wiad, strlen(wiad) + 1);
   read(fd, buffer, sizeof(buffer));
   printf("odebralem: %s\n", buffer);
    close(fd);
    
    return 0;
}
