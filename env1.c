extern char **environ;
#include<stdio.h>
#include<stdlib.h>

char *getenv(const char *name);
int setenv(const char *name, const char *value, int overwrite);
int putenv(char *string);

int unserenv(const char *name);

int main(){

printf("Witaj %s\n",getenv);


return 0;
}
