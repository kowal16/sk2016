#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include<pwd.h>

/* odpowiednik perror() - wypisuje komunikat o bledzie na stderr */
void wypisz_blad(char *msg){
    /* tekst opisujacy ostatni blad */
    char *err_msg = strerror(errno);
    
    /* wypisuje na stderr komunikat i opis bledu */
    write(2, msg, strlen(msg));
    write(2, ": ", 2);
    write(2, err_msg, strlen(err_msg));
    write(2, "\n", 1);
}

/* funkcja wypisujaca tekst na stdout */
void wypisz_tekst(const char *msg){
    if (write(2, msg, strlen(msg)) < 0){
        wypisz_blad("Problem z pisaniem na stdout");
    }
}


void direntInfo(char *name){
	struct stat plik;
	struct passwd tak;	
/*typ pliku */
	if (lstat(name, &plik)==0){
	if(S_ISDIR(plik.st_mode)) wypisz_tekst("d");
	else
	if(S_ISLNK(plik.st_mode)) wypisz_tekst("l");
	else
	if(S_ISFIFO(plik.st_mode)) wypisz_tekst("p");
	else wypisz_tekst("-");
	/*uprawnienia plik*/
	if(plik.st_mode & S_IRUSR) wypisz_tekst("r"); else wypisz_tekst("-");
        if(plik.st_mode & S_IWUSR) wypisz_tekst("w"); else wypisz_tekst("-");
        if(plik.st_mode & S_IXUSR) wypisz_tekst("x"); else wypisz_tekst("-");
        if(plik.st_mode & S_IRGRP) wypisz_tekst("r"); else wypisz_tekst("-");
        if(plik.st_mode & S_IWGRP) wypisz_tekst("w"); else wypisz_tekst("-");
        if(plik.st_mode & S_IXGRP) wypisz_tekst("x"); else wypisz_tekst("-");
        if(plik.st_mode & S_IROTH) wypisz_tekst("r"); else wypisz_tekst("-");
        if(plik.st_mode & S_IWOTH) wypisz_tekst("w"); else wypisz_tekst("-");     
	if(plik.st_mode & S_IXOTH) wypisz_tekst("x "); else wypisz_tekst("- ");
	
/*	uid_t getuid(void);
	wypisz_tekst(tak.pw_uid);
*/
	wypisz_tekst(name);
	}
}
int main(int argc, char *argv[])
{
    struct dirent *wpis;
    DIR *katalog;
    
    if ((katalog = opendir(argv[1])) == NULL){
        wypisz_blad("Problem z otwarciem katalogu");
        return -1;
    }
    
    errno = 0; /* zeruje zmienna opsiujaca bledy */


    while ((wpis = readdir(katalog)) != NULL){
        direntInfo(wpis->d_name);
        wypisz_tekst("\n");
    }
	
    if (errno != 0){  /* jesli readdir() napotkal blad - zmieni errno */
        wypisz_blad("Problem z pobraniem danych z katalogu");
    }
    
    
    if (closedir(katalog) < 0){
        wypisz_blad("Problem z zamknięciem katalogu");
    }
	
	return 0;
}