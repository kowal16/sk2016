#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include<pwd.h>
#include<grp.h>




int main(){
	
	struct passwd *s = getpwuid(getuid()); 	
	 
	printf("uid %d\n",getuid());
	printf("guid %d\n",getgid());
	printf("nazwa %s\n",s->pw_name);
	

return 0;

}
