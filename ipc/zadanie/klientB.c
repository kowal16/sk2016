#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>

struct komunikat {
    long typ;
    int a,b;
    char tekst[512];
};

int main() {

    struct komunikat moj_komunikat;
    int kolejka_id;
    char bufor[512];
    key_t klucz;

    klucz = ftok("/users",1);

    kolejka_id = msgget(klucz,0666 | IPC_CREAT);

    if(kolejka_id == -1) {
        printf("KlientB: funkcja msgget zwrocila blad !!!");
        exit(-1);
    }

    printf("KlientB: wprowadz tekst :");
    fgets(bufor,512,stdin);
    moj_komunikat.typ = 1;
    strcpy(moj_komunikat.tekst,bufor);

    if(msgsnd(kolejka_id,(void *)&moj_komunikat,512,0) == -1) {
        printf("KlientB: funkcja msgsnd zwrocila blad !!!");
        exit(-1);
    }

    if(msgrcv(kolejka_id,(void *)&moj_komunikat,512,5,0) == -1) {
        printf("KlientB: funkcja msgrcv zwrocila blad !!!");
        exit(-1);
    }
    printf("KlientB: z kolejki odczytano ilosc znakow w podanym tekscie: %s\n",moj_komunikat.tekst);

    exit(0);
}
