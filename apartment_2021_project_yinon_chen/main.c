#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    time_t entryDate;
    char *address;
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
apartmentList makeEmptyList(void);
int isEmptyList(apartmentList lst);
void insertNodeToHead(apartmentList* lst, ApartmentNode* newHead);
ApartmentNode* createApartmentNode(int code, int price, int short rooms,
                                   int short entryDay,int short entryMonth,int short entryYear,
                                   int short availableDay,int short availableMonth,int short availableYear,
                                   char *address,ApartmentNode* next);
void checkMemoryAllocation(void* ptr);



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
            line = linen + (line - linep); // re-points to the next char location in memory
            linep = linen;
        }

        if((*line++ = c) == '\n') // break if the char is \n
            break;
    }
    *line = '\0'; // end of string
    return linep;
}

void commandHandler(char* inputLine){
    char command[14]; //short_history is the longest command with 13 chars
    char* spaceLocation = NULL;
    if (*inputLine != '!') {
        spaceLocation = strchr(inputLine, (int)' ');
        if ((spaceLocation - inputLine) > 10) {
            printf("Error in input string check command entered, debug: command string too long");
            exit(1);
        }
        if (spaceLocation == NULL){ // if space is not found in string then the command is not "apartemnt command"
            spaceLocation = strchr(inputLine, (int)'\n');
        }
        
    } else {
        spaceLocation = inputLine + 1; // if ! is the first ch then command starts
    }
    printf(" test %d", (int)(spaceLocation - inputLine));
    strncpy(command, inputLine, (int)(spaceLocation - inputLine));
    /* handler part */
    if (strcmp(command, "find-apt") == 0)
        printf("find-aptt init");
    else if (strcmp(command, "buy-apt") == 0)
        printf("buy-apt init");
    else if (strcmp(command, "add-apt") == 0)
        printf("add-apt init");
    else if (strcmp(command, "history") == 0)
        printf("history init");
    else if (strcmp(command, "delete-apt") == 0)
        printf("delete-apt init");
    else if (strcmp(command, "exit") == 0)
        printf("exit init");
    else if (strcmp(command, "!") == 0)
        printf("! init");
    else if (strcmp(command, "short_history") == 0)
        printf("short_history init");
}

apartmentList makeEmptyList()
{
    apartmentList result;
    
    result.head = NULL;
    return result;
}

int isEmptyList(apartmentList lst)
{
    return (lst.head == NULL);
}

ApartmentNode* createApartmentNode(int code, int price, int short rooms,
        int short entryDay,int short entryMonth,int short entryYear,
        int short availableDay,int short availableMonth,int short availableYear,
        char *address,ApartmentNode* next)
{

    time_t currTime;
    time(&currTime);
    ApartmentNode *apt= (ApartmentNode *)malloc (sizeof(ApartmentNode));
    checkMemoryAllocation(apt);

    apt->data.code=code;
    apt->data.price=price;
    apt->data.rooms=rooms;
    apt->data.availableDate.day=availableDay;
    apt->data.availableDate.month=availableMonth;
    apt->data.availableDate.year=availableYear;
    apt->data.entryDate=currTime;
    apt->data.address=address;
    apt->next=next;

    return apt;
}

void insertNodeToHead(apartmentList* lst, ApartmentNode* newHead)
{
    newHead->next = lst->head;
    lst->head = newHead;
}

void checkMemoryAllocation(void* ptr)
{
    if (ptr == NULL)
    {
        printf("memory allocation failed");
        exit(1);
    }
}
