#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>
struct komunikat{
	long typ;
	int a,b;
	char tekst[512];
};

int main() {
  
  struct komunikat moj_komunikat;
  int kolejka_id;
  char bufor[512];
  key_t klucz;
  
  klucz=ftok("/users/lesk/jaymz",1);

  kolejka_id=msgget(klucz,0666 | IPC_CREAT);

  if(kolejka_id==-1) {
    printf("KlientA: funkcja msgget zwrocila blad !!!");
    exit(-1);
  }
  
    printf("KlientA: wprowadz 2 liczby :");
    scanf("%d %d", &moj_komunikat.a,&moj_komunikat.b);
    //fgets(bufor,512,stdin);
    moj_komunikat.typ=3;
   // strcpy(moj_komunikat.tekst,bufor);

    if(msgsnd(kolejka_id,(void *)&moj_komunikat,512,0)==-1) {
      printf("KlientA: funkcja msgsnd zwrocila blad !!!");
      exit(-1);
    }
    
    if(msgrcv(kolejka_id,(void *)&moj_komunikat,512,5,0)==-1) {
      printf("KlientA: funkcja msgrcv zwrocila blad !!!");
      exit(-1);
    }
    printf("KlientA: z kolejki odczytano sume %s",moj_komunikat.tekst);
    
  
  exit(0);
}
