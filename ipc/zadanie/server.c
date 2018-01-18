#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

int kolejka_id;

struct komunikat {
    long typ;
    int a,b;
    char tekst[512];
};

void zakoncz() {
    msgctl(kolejka_id, IPC_RMID, 0);
    exit(0);
}

int main() {

    struct sigaction sygnaly;
    sygnaly.sa_handler = zakoncz;
    sigemptyset(&sygnaly.sa_mask);
    sygnaly.sa_flags = 0;

    sigaction(SIGINT, &sygnaly, NULL);
    pid_t pid = 0;

    int suma;
    struct komunikat moj_komunikat;
    key_t klucz;

    klucz = ftok("/users",1);

    kolejka_id = msgget(klucz,0666 | IPC_CREAT);
    if(kolejka_id ==-1) {
        printf("Serwer: funkcja msgget zwrocila blad !!!");
        exit(-1);
    }

    if((pid = fork()) < 0) {
        perror("Blad tworzenia potomka.(klient)\n");
        kill(pid, SIGINT);
    }

    if(pid > 0) {
        while(1) {
            if(msgrcv(kolejka_id,(void *)&moj_komunikat,512,3,0) == -1) {
                printf("Serwer: funkcja msgrcv zwrocila blad !!!");
                exit(-1);
            } else {
                suma = moj_komunikat.a + moj_komunikat.b;
                sprintf(moj_komunikat.tekst,"%i",suma);
                moj_komunikat.typ = 5;

                if(msgsnd(kolejka_id,(void *)&moj_komunikat,512,0) == -1) {
                    printf("Serwer: funkcja msgsnd zwrocila blad !!!");
                    exit(-1);
                }
            }
        }
    } else {
        while(1) {
            if(msgrcv(kolejka_id,(void *)&moj_komunikat,512,1,0) == -1) {
                printf("Serwer: funkcja msgrcv zwrocila blad !!!");
                exit(-1);
            } else {
                suma = strlen(moj_komunikat.tekst)-1;
                sprintf(moj_komunikat.tekst,"%i",suma);
                moj_komunikat.typ = 5;

                if(msgsnd(kolejka_id,(void *)&moj_komunikat,512,0) == -1) {
                    printf("Serwer: funkcja msgsnd zwrocila blad !!!");
                    exit(-1);
                }
            }
        }
    }

}
