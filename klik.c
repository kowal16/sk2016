#include<stdio.h>

void klikniecia(){
	int clicks = 0;
	int krok = 1;
	clicks += krok;
	printf("klikniec: %d\n", clicks);
}

int main(int argc, char **argv){
	int i;
	for(i = 0; i < 5; i++){
		klikniecia();
	}
	return 0;
}
