#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

int idKolejki;
int idKolejki2;

/* struktura tresci wiadomosci odebranej z kolejki */
struct wiadomosc {
	char user[32];
	char text[256];
};
/* struktura wiadomosc wysylanej poprzez kolejke komunikatow */
struct msg {
	long m_type; /* obowiazkowe pole typu */
	struct wiadomosc tresc;  /* treść wiadomości wyrażona w osobnej strukturze, w celu rozróżnienia nadawcy i wiadomości */
} msgRcv, msgSnd; /* msgSnd wiadomość wysyłana, msgRcv wiadomość wysyłana */

void zakoncz() { /* funkcja uruchamiana w przypadku wywołania prerwania z klawiatury */
	msgctl(idKolejki, IPC_RMID, 0); /* usunięcie kolejki komunikatów powstałej w  programie */
	msgctl(idKolejki2, IPC_RMID, 0); /* usunięcie kolejki komunikatów powstałej w programie */
	exit(0); /* wyjście z programu */
}

int main(void) {

	/* struktura do obslugi sygnal zakonczenia dzialania serwera */
	struct sigaction sygnaly;
	sygnaly.sa_handler = zakoncz;
	sigemptyset(&sygnaly.sa_mask);
	sygnaly.sa_flags = 0;
	/* sygnalem bedzie sygnal przerwania z klawiatury */
	sigaction(SIGINT, &sygnaly, NULL);

	/* otwarcie kolejki komunikatow do odzczytu wiadomosci przez serwer */
	if((idKolejki = msgget(ftok("./temp", 1), IPC_CREAT | 0600)) == -1) {
		perror("Blad otwarcia kolejki.\n"); /* jeśli otwiercie się nie powiedzie to wyświetl błąd i zakończ program */
		kill(getpid(), SIGINT);
	}	
	/* otwarcie kolejki komunikatow do wysylania wiadomosci z serwera */
	if((idKolejki2 = msgget(ftok("./temp", 2), IPC_CREAT | 0600)) == -1) {
		perror("Blad otwarcia kolejki.\n"); /* jeśli otwarcie się nie powiedzie to wyświetl błąd i zakończ program */
		kill(getpid(), SIGINT);
	}	

	while(1) {
		/* odbieranie wiadomości z kolejki*/
		if((msgrcv(idKolejki, (void*) &msgRcv, sizeof(msgRcv.tresc), msgRcv.m_type, 0)) == -1) {
			perror("Blad odczytu wiadomosci.\n");
			kill(getpid(), SIGINT);
		}
		/* ustawienie wiadomość wysłanej na wartosc wiadomość odebranej */
		msgSnd = msgRcv;
		/* wysyłanie wiadomości do kolejki */
		if((msgsnd(idKolejki2, (void*) &msgSnd, sizeof(msgSnd.tresc), 0)) == -1) {
			perror("Blad wysylania komunikatu.\n");
			kill(getpid(), SIGINT);
		} 
	}		
	
return 0;
}

