//
//  main.c
//  apartment_2021_project
//
//  Created by Yinon Horev on 20/12/2020.
//

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define N 7

typedef struct date
{
    int short day;
    int short month;
    int short year;
}Date;

/* apartment struct defined by project demands */
typedef struct apartment
{
    int code;
    int price;
    int short rooms;
    Date availableDate;
    Date entryDate;
    char address[];
}Apartment;

/* apartment node in the linked list */
typedef struct apartmentNode
{
    struct apartmentNode *next;
    struct apartment data;
    
}ApartmentNode;


/* with dummy head and dummy tail */
typedef struct apartmentList
{
    ApartmentNode *head;
}apartmentList;


void printPrompt(void);
char* getLine(void);
void commandHandler(char* inputLine);



int main(int argc, const char * argv[]) {
    char *short_term_history[N], *inputLine;
    printPrompt();
    inputLine = getLine();
    commandHandler(inputLine);
    return 0;
}

void printPrompt(){
    printf("Please enter one of the following commands:\n"
           "add-apt, find-apt, buy-apt, delete-apt or exit\n"
           "For reconstruction commands, please enter:\n"
           "!!, !num, history, short_history or !num^str1^str2\n");
}


char* getLine() {
    char * line = malloc(100), * linep = line;
    int lenmax = 100, len = lenmax; // if we change lenmax to 1 we will realloc every char
    int c;

    if(line == NULL)
        return NULL;

    for(;;) {
        c = fgetc(stdin);
        if(c == EOF)
            break;

        if(--len == 0) {
            len = lenmax;
            char * linen = realloc(linep, lenmax *= 2);

            if(linen == NULL) {
                free(linep);
                return NULL;
            }
            line = linen + (line - linep);
            linep = linen;
        }

        if((*line++ = c) == '\n') // break if the char is \n
            break;
    }
    *line = '\0'; // end of string
    return linep;
}

void commandHandler(char* inputLine){
    
}
