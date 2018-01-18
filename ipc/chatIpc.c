#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <signal.h>
#include <stdlib.h>

struct wiadomosc {
    char uzytkownik[32];
    char tresc[256];
};

int shm_id, sem_id;
struct wiadomosc *msg;

union semun {
    int     val;            /* wartosc dla SETVAL */
    struct  semid_ds *buf;  /* bufor dla IPC_STAT & IPC_SET */
    u_short *array;         /* tablica dla GETALL & SETALL */
};

void koniec(int signum){
    printf("Koncze dzialanie!\n");
    /* zwalniam zasoby */
    shmdt(msg);
    shmctl(shm_id, IPC_RMID, NULL);
    semctl(sem_id, 0, IPC_RMID);
    exit(0);
}

int main(void){
    /* tworze klucze dla pamieci wspoldzielonej oraz semaforow */
    key_t shm_key = ftok("/usr", 1); 
    key_t sem_key = ftok("/usr", 2);

    /* struktury do zarzadzania semaforami */
    struct sembuf opis_operacji[2];
    union semun sem_arg;

    /* obsluga sygnalu */
    struct sigaction sa;
    sa.sa_handler = koniec;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);

    /* tworze segment pamieci wspoldzielonej,  mieszczacy 1 wiadomosc */
    shm_id = shmget(shm_key, sizeof(struct wiadomosc), IPC_CREAT | 0600);

    /* "podlaczam" segment do uzycia w programie (korzystamy analogicznie jak z malloc) */
    msg = shmat(shm_id, NULL, 0);

    /* tworze i inicjalizuje semafor(y) */
    sem_id = semget(sem_key, 2, IPC_CREAT | 0600);
    
    /* inicjalizuje wartości początkowe */
    sem_arg.val = 1;  /* mozna zajac segment */
    semctl(sem_id, 0, SETVAL, sem_arg);
    sem_arg.val = 0;  /* nie ma wiadomosci do czytania */
    semctl(sem_id, 1, SETVAL, sem_arg);

    
    /* glowna petla serwera  */
    printf("Serwer dziala! Czekam na wiadomosci od klientow\n");
    while (1) {
        /* przygotowuje operacje do wykonania na semaforach */
        opis_operacji[0].sem_num = 0;
        opis_operacji[0].sem_op = -1; /* chce zajac segment */
        opis_operacji[0].sem_flg = SEM_UNDO;
        
        opis_operacji[1].sem_num = 1;
        opis_operacji[1].sem_op = -1; /* z zapisana wiadomoscia */ 
        opis_operacji[1].sem_flg = SEM_UNDO;
   
        /* wykonuje operacje na dwoch semaforach */
        semop(sem_id, opis_operacji, 2);
        
        /* wypisuje wiadomosc */
        printf("[%s] %s\n", msg->uzytkownik, msg->tresc);
        
        /* podnosze semafor - mozna wpisac wiadomosc */
        opis_operacji[0].sem_num = 0;
        opis_operacji[0].sem_op = 1; /* chce "zwolnic" segment */
        opis_operacji[0].sem_flg = SEM_UNDO;
        
        /* wykonuje operacje na jednym(!) semaforze */
        semop(sem_id, opis_operacji, 1);
    }    

    return 0;
}
