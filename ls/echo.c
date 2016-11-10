#include <unistd.h>
/* echo - program wypisujacy na standardowe wyjscie dane przeczytane 
 * ze standardowego wejscia */


int main(int argc, char **argv)
{
    char msg[] = "Wpisz \"x\" aby zakonczyc program\n";
    char ch = 'y';
    
    /* wypisuje komunikat na ekran - stdout*/
    write(1, msg, sizeof(msg));
    
    while (ch != 'x'){
        read(0, &ch, 1);   /* czytam jeden bajt z stdin */
        write(1, &ch, 1);  /* zapisuje jeden bajt do stdout */
    }
    
	return 0;
}
