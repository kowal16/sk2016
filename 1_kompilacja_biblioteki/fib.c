#include <stdio.h>
#include "fib.h"

int fib(int n){
	if ( n <= 1 ) 
		return n;

	int f_1 = 0, f_2 = 1, m, i;

	for (i = 2; i <= n; i++){
		m = f_1 + f_2;
		f_1 = f_2;
		f_2 = m;
	}

	return f_2;
}
