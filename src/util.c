#include "util.h"

/* 
 * convert string format number to unsigned int
 * params: string to convert
 * return: 
 *		number value
 *		-1 if invalid
 */
int stoui(char *s){
	int a = 0;
	for(int i = 0; i < strlen(s); i++) {
		if(s[i] < '0' || s[i] > '9') return -1;
		a  = a * 10 + s[i] - '0';
	}
	return a;
}

// Print error and then exit program
void error(char* mess){
	printf("Some error occured! %s\n", mess);
	exit(0);
}