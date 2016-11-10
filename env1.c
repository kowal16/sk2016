extern char **environ;
#include<stdio.h>
#include<stdlib.h>
#include<cstring>
char *getenv(const char *name);
int setenv(const char *name, const char *value, int overwrite);
int putenv(char *string);

int unsetenv(const char *name);

int main(){
string name =*environ.name;
printf("Witaj %s\n",name);


return 0;
}
