#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/* 
 * convert string format number to unsigned int
 *	params: string to convert
 *	return: 
 *		number value
 *		-1 if invalid
 */
int stoui(char *s);

/*
 * Print error and then exit program
 *	params:
 *		char* mess error message
 */
void error(char* mess);

#endif