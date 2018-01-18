#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>

int idKolejki;
int idKolejki2;

/* struktura tresci wiadomosci przechodzacej przez kolejke */
struct wiadomosc {
    char user[32];
    char text[256];
};

/* struktura wiadomosc wysylanej przez kolejke komunikatow */
struct msg {
    long m_type; /* obowiazkowe pole typu */
    struct wiadomosc tresc;  /* tresc wiadomosci opisana w strukturze w celu wyslania do kolejki nazwy uzytkownika oraz wiadomosci */
} msgRcv, msgSnd; /* msgSnd wiadomość wysylana, msgRcv wiadomość wysylana */

void zakoncz() { /* funkcja uruchamiana w przypadku wywolania prerwania z klawiatury */
	strcpy(msgSnd.tresc.text, "opuscil serwer."); /* wiadomosc o dołączeniu */
    msgSnd.m_type = 1;  /* okreslenie typu wiadomości */	
	msgsnd(idKolejki, (void*) &msgSnd, sizeof(msgSnd.tresc), 0);
	exit(0); /* wyjscie z programu */
}

int main(int argc, char **argv) {
    pid_t pid = 0;     /* pid potrzebny podczas wywolania funkcji fork() */
    
    /* sprawdzenie ile argumentow podano, powinny być 2: nazwa programu i nazwa uzytkownika */
    if(argc != 2) {
        printf("%s", "Podano zla ilosc parametrow.\n");
        return -1;
    }   

	/* struktura do obslugi sygnal zakonczenia dzialania serwera */
	struct sigaction sygnaly;
	sygnaly.sa_handler = zakoncz;
	sigemptyset(&sygnaly.sa_mask);
	sygnaly.sa_flags = 0;
	/* sygnalem bedzie sygnal przerwania z klawiatury */
	sigaction(SIGINT, &sygnaly, NULL);


	/* polaczenie z kolejka komunikatów do wysylki komunikatow na serwera */
    if((idKolejki = msgget(ftok("./temp", 1), 0600)) == -1) {
		perror("Blad otwarcia kolejki do wysylki.(klient)\n"); /* jeśli otwiercie się nie powiedzie to wyświetli błąd i zakończy program */
        kill(pid, SIGINT);
        return -1;
    }

	/* polaczenie z kolejka komunikatow do odbierania komunikatow z serwera */
	if((idKolejki2 = msgget(ftok("./temp", 2), 0600)) == -1) {
		perror("Blad otwarcia kolejki do odbioru(klient).\n"); /* jeśli otwiercie się nie powiedzie to wyświetli błąd i zakończy program */
		kill(pid, SIGINT);
		return -1;
	}

	strcpy(msgSnd.tresc.user, argv[1]); /* zapisanie uzytkownika z argumentu wywolania */
	strcpy(msgSnd.tresc.text, "dolaczyl do serwera."); /* wiadomosc o dolaczeniu uzytkownika */
    msgSnd.m_type = 8;  /* okreslenie typu wiadomosci */

	if((msgsnd(idKolejki, (void*) &msgSnd, sizeof(msgSnd.tresc), 0)) == -1) { /* wyslanie wiadomosci powitalnej */
		perror("Blad wysylania wiadomosci powitalnej.(klient)\n");
		kill(getpid(), SIGINT);
	}	

	if((pid = fork()) < 0) {
        perror("Blad tworzenia potomka.(klient)\n");
		kill(pid, SIGINT);
    } 

    if(pid > 0) { /* przodek: odpowiedzialny za wysylanie wiadomosci do kolejki */
        while(1) {
			fgets(msgSnd.tresc.text, sizeof(msgSnd.tresc.text), stdin); /* pobranie wiadomosci z klawiatury */
            if((msgsnd(idKolejki, (void*) &msgSnd, sizeof(msgSnd.tresc), 0)) == -1) { /* wyslanie wiadomosci */
				perror("Blad wysylania wiadomosci(klient).\n");
				kill(pid, SIGINT);
			}
        }
    } else { /* potomek: odpowiedzialny za odczytanie wiadomosci z kolejki */
        while(1) {
            /* odbieranie wiadomosci i jej wyswietlanie */
            if((msgrcv(idKolejki2, (void*)&msgRcv, sizeof(msgRcv.tresc), msgRcv.m_type, 0)) == -1) {
	            perror("Blad czytania wiadomosci(klient).\n");
               	kill(getppid(), SIGINT);  
	            return -1;
            }
            printf("\n [%s] : %s \n", msgRcv.tresc.user, msgRcv.tresc.text);
        }
    }
return 0;
}
/* Dziala tylko kounikacja 2 uzytkownikow (klient-serwer) w przypadku wiekszej ilosci klientow komunikaty sa odbierane przez klientow na zmiane,
 * teoretycznie wiem jak rozwiazac ten problem (Serwer powinien tworzyc kolejke, do ktorej by wpisywal wiadomosci dla kazdego klienta jaki sie z nim polaczy.
 * Nie wiem w jaki sposob serwer bedzie wiedzial kiedy ktos nowy sie z nim polaczy, teraz to tylko przeklada wiadomosci z jednej kolejki do drugiej) */ 

