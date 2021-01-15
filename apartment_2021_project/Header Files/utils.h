//
//  utils.h
//  apartment_2021_project
//
//  Created by Yinon Horev on 13/01/2021.
//

#ifndef utils_h
#define utils_h

#include <stdio.h>
#include "apartment.h"

char **tokenize(char *line);

char* strReplace(char* search, char* replace, char* subject);

int compareDates(struct date d1, struct date d2);

/*Prints application manual*/
void printPrompt(void);

/*Reads a line from the user*/
char *getLine(void);

void checkMemoryAllocation(void *ptr);

#endif /* utils_h */
