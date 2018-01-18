#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

struct wiadomosc {
    char uzytkownik[32];
    char tresc[256];
};

int main(int argc, char **argv){
    /* tworze klucze dla pamieci wspoldzielonej oraz semaforow */
    key_t shm_key = ftok("/usr", 1); 
    key_t sem_key = ftok("/usr", 2);
    
    int shm_id, sem_id;
    /* struktury do zarzadzania semaforami */
    struct sembuf opis_operacji[2];
    
    /* wskaznik do pamieci zawierajacej wiadomosc */
    struct wiadomosc *msg;
    
    if (argc != 3){
        fprintf(stderr, "Program wymaga trzech argumentów!\n");
        return -1;
    }
    /* dostep do pamieci wspoldzielonej, oraz semafora */
    shm_id = shmget(shm_key, sizeof(struct wiadomosc), 0);
    
    /* "podlaczam sie" do istniejacego zbioru semaforow - dlatego jako ilosc podaje 0 */
    sem_id = semget(sem_key, 0, 0);
    
    if ((shm_id < 0) || (sem_id < 0)){
        fprintf(stderr, "Nie moge podlaczyc segmentu pamieci/semafora. Czy serwer dziala?\n");
        return -1;
    }
    
    /* "podlaczam" segment do uzycia w programie */
    msg = shmat(shm_id, NULL, 0);
    
    /* przygotowuje operacje na semaforach: */
    opis_operacji[0].sem_num = 0;
    opis_operacji[0].sem_flg = SEM_UNDO;
    opis_operacji[0].sem_op = -1;  /* czekam na dostep do segmentu*/    
    
    opis_operacji[1].sem_num = 1;
    opis_operacji[1].sem_flg = SEM_UNDO;
    opis_operacji[1].sem_op = -1;    /* w ktorym ma_ wiadomosci */
    
    /* wykonuje operacje na dwoch semaforach */
    semop(sem_id, opis_operacji, 2);
    
    /* wpisuje wiadomosc */
    printf("[%s] %s\n", msg->uzytkownik, msg->tresc);
    strncpy(msg->uzytkownik, argv[1], 32);
    strncpy(msg->tresc, argv[2], 256);
    
    /* podnosze semafor(y) - mozna czytac wiadomosc */
    opis_operacji[0].sem_op = 1;
    opis_operacji[1].sem_op = 1;    
    semop(sem_id, opis_operacji, 2);
    
    shmdt(msg);

    return 0;
}
