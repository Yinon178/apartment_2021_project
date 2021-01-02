#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <limits.h>

#define N 7

/* global variable declaration */
int counter = 1; // counter is the next apartment code. Starts from 1 as writen in instructions

typedef struct date {
    int short day;
    int short month;
    int short year;
} Date;

/* apartment struct defined by project demands */
typedef struct apartment {
    int code;
    int price;
    int short rooms;
    Date availableDate;
    time_t entryDate;
    char *address;
} Apartment;

/* apartment node in the linked list */
typedef struct apartmentNode {
    struct apartmentNode *next;
    struct apartment data;

} ApartmentNode;


/* with dummy head and dummy tail */
typedef struct apartmentList {
    ApartmentNode *head;
    ApartmentNode *tail;
} apartmentList;

/* history node in the linked list */
typedef struct historyListNode {
    char *command;
    struct historyListNode *next;

} HistoryListNode;

/* history list struct*/
typedef struct historyList {
    HistoryListNode *head;
    HistoryListNode *tail;

} HistoryList;

/*Prints application manual*/
void printPrompt(void);

/*Reads a line from the user*/
char *getLine(void);

/* Calls the relevant command by the input from the user*/
void commandHandler(char *inputLine, apartmentList *aptList, HistoryList *historyList);

/*utility functions apartment linked list*/
apartmentList makeEmptyList(void);

int isEmptyList(apartmentList lst);

void insertNodeToTail(apartmentList *lst, ApartmentNode *newHead);

void apartmentCpy(ApartmentNode* destination, ApartmentNode* source);

void printApartmentList(apartmentList *apartmentList);

void printApartmentNode(ApartmentNode *node);

ApartmentNode *createApartmentNode(int code, int price, int short rooms,
                                   int short availableDay, int short availableMonth, int short availableYear,
                                   char *address, ApartmentNode *next);

void deleteApartmentByCode(int code, apartmentList *lst);

void checkMemoryAllocation(void *ptr);

int isEnteredInLastDays(time_t entryDate, int day);

void deleteApartmentByDay(int day, apartmentList *lst);

/*Archives the line to short term history and if needed to long term*/
void archiveLine(char *short_term_history[]); // TODO: Chen please add long term history management

/* Apartment commands as described in the project instructions */
void addApt(char *inputLine, apartmentList *aptList);

void buyApt(char *inputLine, apartmentList *aptList);

void findApt(char *inputLine, apartmentList *aptList);

void deleteApt(char *inputLine, apartmentList *aptList);

void gracefulExit(apartmentList *aptList);

/* Utility */
char **tokenize(char *line);

void filterApartmentsConditions(apartmentList* apartmentlst, apartmentList* filteredApartmentlst, int maxPrice,
                                         int minPrice, short int minNumRooms,
                                         short int maxNumRooms, Date d, int enter);

int compareDates(struct date d1, struct date d2);

void printApartment(Apartment *apartment);

/* History commands*/
HistoryList makeEmptyHistoryList(void);

void insertToArchive(HistoryList *historyList,char *command);

HistoryListNode* createNewHistoryNode(char* command);

bool isEmptyHistoryList(HistoryList *historyList);

void insertHistoryNodeToHead(HistoryList *historyList, HistoryListNode *head);

int main(int argc, const char *argv[]) {
    char *short_term_history[N], *inputLine;
    int index = 0;//for history
    apartmentList aptList;
    aptList = makeEmptyList();
    HistoryList historyList;
    historyList = makeEmptyHistoryList();
    printPrompt();
    while (1) {
        inputLine = getLine();
        commandHandler(inputLine, &aptList, &historyList);
        index++;
    }

    return 0;
}

void printPrompt() {
    printf("Please enter one of the following commands:\n"
           "add-apt, find-apt, buy-apt, delete-apt or exit\n"
           "For reconstruction commands, please enter:\n"
           "!!, !num, history, short_history or !num^str1^str2\n");
}

char *getLine() {
    char *line = malloc(100), *linep = line;
    int lenmax = 100, len = lenmax; // if we change lenmax to 1 we will realloc every char
    int c;

    if (line == NULL)
        return NULL;

    for (;;) {
        c = fgetc(stdin);
        if (c == EOF)
            break;

        if (--len == 0) {
            len = lenmax;
            char *linen = realloc(linep, lenmax *= 2);

            if (linen == NULL) {
                free(linep);
                return NULL;
            }
            line = linen + (line - linep); // re-points to the next char location in memory
            linep = linen;
        }

        if ((*line++ = c) == '\n') // break if the char is \n
            break;
    }
    *line = '\0'; // end of string
    return linep;
}

void commandHandler(char *inputLine, apartmentList *aptList, HistoryList *historyList) {
    char command[14]; //short_history is the longest command with 13 chars
    char *spaceLocation = NULL;
    if (*inputLine != '!') {
        spaceLocation = strchr(inputLine, (int) ' ');
        if ((spaceLocation - inputLine) > 10) {
            printf("Error in input string check command entered, debug: command string too long");
            exit(1);
        }
        if (spaceLocation == NULL) { // if space is not found in string then the command is not "apartemnt command"
            spaceLocation = strchr(inputLine, (int) '\n');
        }

    } else {
        spaceLocation = inputLine + 1; // if ! is the first ch then command starts
    }
    strncpy(command, inputLine, (int) (spaceLocation - inputLine));
    *(command + (int) (spaceLocation - inputLine)) = '\0'; // marking end of string

    /* handler part */
    if (strcmp(command, "find-apt") == 0)
        findApt(inputLine, aptList);
    else if (strcmp(command, "buy-apt") == 0)
        buyApt(inputLine, aptList);
    else if (strcmp(command, "add-apt") == 0)
        addApt(inputLine, aptList);
    else if (strcmp(command, "history") == 0)
        printf("history init");
    else if (strcmp(command, "delete-apt") == 0)
        deleteApt(inputLine, aptList);
    else if (strcmp(command, "exit") == 0)
        gracefulExit(aptList);
    else if (strcmp(command, "!") == 0)
        printf("! init");
    else if (strcmp(command, "short_history") == 0)
        printf("short_history init");
}


void historyHandler(char *inputLine, int index, char *short_term_history[N], HistoryList *historyList) {
    char *inputLineCopy = malloc(sizeof(char) * strlen(inputLine)); // TODO: free it up in exit
    checkMemoryAllocation(inputLineCopy);
    strcpy(inputLineCopy, inputLine);

    if (index >= 6)//check if the array is full
    {
        insertToArchive(historyList, short_term_history[6]);

    }
    //if not full start to push string to th

    for (int i = index; i >= 1; i--) {
        short_term_history[i + 1] = short_term_history[i];
    }


    short_term_history[0] = inputLineCopy;

}

void insertToArchive(HistoryList *historyList,char *command)
{
    HistoryListNode *newHead;
    newHead = createNewHistoryNode(command);
    insertHistoryNodeToHead(historyList, newHead);

}


HistoryListNode* createNewHistoryNode(char* command)
{
    HistoryListNode* res;
    res = (HistoryListNode*)malloc(sizeof(HistoryListNode));
    res->command=command;
    res->next = NULL;
    return res;
}

void insertHistoryNodeToHead(HistoryList *historyList, HistoryListNode *head)
{
    if (isEmptyHistoryList(historyList) == true)
    {
        head->next = NULL;
        historyList->head = historyList->tail = head;
    }
    else
    {
        head->next = historyList->head;
        historyList->head = head;
    }
}

bool isEmptyHistoryList(HistoryList *historyList)
{
    if (historyList->head == NULL)
        return true;
    else
        return false;
}

apartmentList makeEmptyList() {
    apartmentList result;

    result.head = result.tail = NULL;
    return result;
}

int isEmptyList(apartmentList lst) {
    return (lst.head == NULL);
}

ApartmentNode *createApartmentNode(int code, int price, int short rooms,
                                   int short availableDay, int short availableMonth, int short availableYear,
                                   char *address, ApartmentNode *next) {

    time_t currTime;
    time(&currTime);
    ApartmentNode *apt = (ApartmentNode *) malloc(sizeof(ApartmentNode));
    checkMemoryAllocation(apt);

    char *strAdress = (char *) malloc(strlen(address) * sizeof(char));
    strcpy(strAdress, address);

    apt->data.code = code;
    apt->data.price = price;
    apt->data.rooms = rooms;
    apt->data.availableDate.day = availableDay;
    apt->data.availableDate.month = availableMonth;
    apt->data.availableDate.year = availableYear;
    apt->data.entryDate = currTime;
    apt->data.address = strAdress;
    apt->next = next;

    return apt;
}

void insertNodeToTail(apartmentList *lst, ApartmentNode *newHead) {
    if (isEmptyList(*lst) == true)
        lst->head = lst->tail = newHead;
    else {
        lst->tail->next = newHead;
        lst->tail = newHead;
    }
    newHead->next = NULL;
}

void apartmentCpy(ApartmentNode* destination, ApartmentNode* source){
    destination->data = source->data;
    destination->next = source->next;
}

void checkMemoryAllocation(void *ptr) {
    if (ptr == NULL) {
        printf("memory allocation failed");
        exit(1);
    }
}

void deleteApartmentByCode(int code, apartmentList *lst) {
    ApartmentNode *curr = lst->head;
    ApartmentNode *prev = lst->head;
    // If head node itself holds the key to be deleted
    if (curr != NULL && curr->data.code == code) {
        lst->head = curr->next;// Changed head
        free(curr);// free old head
        return;
    }

    // Search for the key to be deleted, keep track of the
    // previous node as we need to change 'prev->next'
    while (curr != NULL && curr->data.code != code) {
        prev = curr;
        curr = curr->next;
    }

    // If key was not present in linked list
    if (curr == NULL) return;

    // Unlink the node from linked list
    prev->next = curr->next;

    free(curr);  // Free memory

}

void deleteApartmentByDay(int day, apartmentList *lst) {
    ApartmentNode *curr = lst->head;
    ApartmentNode *next = lst->head;
    lst->tail = NULL;

    if (isEnteredInLastDays(curr->data.entryDate, day))//check if we need to free all list
    {
        lst->head = NULL;
    }

    while (curr != NULL) {//loop till the end of list

        next = curr->next;
        if (isEnteredInLastDays(curr->data.entryDate, day))//check if the node was created in the right range time
        {
            free(curr);
        } else {
            lst->tail = curr;
        }
        curr = next;
    }
}

void freeApartList(apartmentList *lst) {
    ApartmentNode *curr = lst->head;
    ApartmentNode *next = lst->head;

    while (curr != NULL) { //loop till the end of list
        next = curr->next;
        free(curr); // TODO: BUG
        curr = next;
    }
}

int isEnteredInLastDays(time_t entryDate, int day) {
    time_t curr;
    time(&curr);
    //Returns the difference of seconds between time1 and time2 (time1-time2).
    double diff_t = (difftime(curr, entryDate) / (60 * 60 * 24));
    if (diff_t <= day) {
        return 1;
    } else {
        return 0;
    }

}

void addApt(char *inputLine, apartmentList *aptList) {
//    char* cpInputLine = NULL;
//    strcpy(cpInputLine, inputLine);
    ApartmentNode *apartmentNode;
    char **tokens = tokenize(inputLine);

    apartmentNode = createApartmentNode(counter, (int) strtol(tokens[2], NULL, 10),
                                        (int) strtol(tokens[3], NULL, 10),
                                        (short int) strtol(tokens[4], NULL, 10),
                                        (short int) strtol(tokens[5], NULL, 10),
                                        (short int) (strtol(tokens[6], NULL, 10) + 2000),
                                        tokens[1], NULL);

    insertNodeToTail(aptList, apartmentNode);
    counter++;
    free(inputLine); // strtok manipulated string therefore it must be freed here
    free(tokens);
}

void buyApt(char *inputLine, apartmentList *aptList) {
    char **tokens = NULL;
    tokens = tokenize(inputLine);
    deleteApartmentByCode((int) strtol(tokens[1], NULL, 10), aptList);
}

void deleteApt(char *inputLine, apartmentList *aptList) {
    char **tokens = NULL;
    tokens = tokenize(inputLine);
    deleteApartmentByDay((int) strtol(tokens[2], NULL, 10), aptList);
}

void findApt(char *inputLine, apartmentList *aptList){
    /* This function will build a dynamic if statment and serch for apartments that checks out with this if*/
    apartmentList filteredApartmentList = makeEmptyList();
    int maxPrice=INT_MAX, minPrice=0, enter=INT_MAX, i=1, dummy;
    Date d;
    /* initalizing to a passed date */
    d.day = 1;
    d.month = 1;
    d.year = 2020;
    short int minNumRooms=-1, maxNumRooms=INT8_MAX;
    char **tokens = tokenize(inputLine);
    bool fromLowToHigh=false, fromHighTolow=false;
    
    /* Parsing string */
    while (tokens[i] != NULL) {
        if (!strcmp(tokens[i], "–MaxPrice")) {
            i++;
            maxPrice = (int)strtol(tokens[i], NULL, 10);
        }
        else if (!strcmp(tokens[i], "–MinPrice")) {
            i++;
            minPrice = (int)strtol(tokens[i], NULL, 10);
        }
        else if (!strcmp(tokens[i], "–MinNumRooms")) {
            i++;
            minNumRooms = (short int)strtol(tokens[i], NULL, 10);
        }
        else if (!strcmp(tokens[i], "–MaxNumRooms")) {
            i++;
            maxNumRooms = (short int)strtol(tokens[i], NULL, 10);
        }
        else if (!strcmp(tokens[i], "-Date")) {
            i++;
            sscanf(tokens[i], "%2hd%2hd%4hd", &d.day, &d.month, &d.year);
        }
        else if (!strcmp(tokens[i], "–Enter")) {
            i++;
            enter = (short int)strtol(tokens[i], NULL, 10);
        }
        else if (!strcmp(tokens[i], "-s")) {
            fromLowToHigh=true;
        }
        else if (!strcmp(tokens[i], "-sr")) {
            fromHighTolow=true;
        }
        else {
            printf("Error parsing %s \n line number: %d ", tokens[i], __LINE__);
            exit(1);
        }
        i++;
    }
    filterApartmentsConditions(aptList, &filteredApartmentList , maxPrice, minPrice, minNumRooms, maxNumRooms, d, enter);
    if (fromLowToHigh)
        printf("sorted fromLowToHigh");
    else if (fromHighTolow)
        printf("sorted fromHighTolow");
    else
        printApartmentList(&filteredApartmentList);
    
}

void filterApartmentsConditions(apartmentList* apartmentlst, apartmentList* filteredApartmentlst, int maxPrice,
                                         int minPrice, short int minNumRooms,
                                         short int maxNumRooms,Date d, int enter){
    ApartmentNode* head = apartmentlst->head;
    while (head != NULL) {
        if (head->data.price > minPrice
            && head->data.price <= maxPrice
            && head->data.rooms >= minNumRooms
            && head->data.rooms <= maxNumRooms
            && compareDates(d, head->data.availableDate) == 1
            && isEnteredInLastDays(head->data.entryDate, enter)) {
            ApartmentNode *apt = (ApartmentNode *) malloc(sizeof(ApartmentNode));
            checkMemoryAllocation(apt);
            apartmentCpy(apt, head);
            insertNodeToTail(filteredApartmentlst, apt);
        }
        head = head->next;
    }
    
}

void gracefulExit(apartmentList *aptList) {
    freeApartList(aptList);
    exit(0);
}


char **tokenize(char *line) {
    int i = 0, tlen = 0;
    char **tokens = NULL, *token, *delimiter;
    delimiter = " ";
    token = strtok(line, delimiter);
    while (token != NULL) {
        if (i == tlen) {
            // Allocate more space
            tlen += 3;
            tokens = realloc(tokens, tlen * sizeof *tokens);
            checkMemoryAllocation(tokens);
        }
        tokens[i] = token;
        if (*(token + strlen(line) + 1) == '\"') { // handles " mark and add a pointer to the corret token
            delimiter = "\"";
            token = strtok(NULL, delimiter);
            i += 1;
            tokens[i] = token; // skip the " mark
            delimiter = " ";
        }
        token = strtok(NULL, delimiter);
        i += 1;
    }
    tokens[i] = NULL;
    return tokens;
}

void printApartmentList(apartmentList *apartmentList) {
    ApartmentNode *curr = apartmentList->head;
    while (curr != NULL) {//loop till the end of list
        printApartmentNode(curr);
        curr = curr->next;
    }
}

void printApartmentNode(ApartmentNode *node) {
    printApartment(&(node->data));
}

void printApartment(Apartment *apartment) {

    struct tm tm = *localtime(&(apartment->entryDate)); // breakes down to a manageable struct of time
    printf("Apt details:\n"
           "Code: %d\n"
           "Address: %s\n"
           "Number of rooms: %d\n"
           "Price: %d\n"
           "Entry date: %d.%d.%d\n"
           "Database entry date: %d.%d.%d\n",
           apartment->code,
           apartment->address,
           apartment->rooms,
           apartment->price,
           apartment->availableDate.day,
           apartment->availableDate.month,
           apartment->availableDate.year,
           tm.tm_mday, tm.tm_mon + 1, tm.tm_year - 100);
}

int compareDates(struct date d1, struct date d2)
{
    if (d1.year < d2.year)
       return -1;

    else if (d1.year > d2.year)
       return 1;

    if (d1.year == d2.year)
    {
         if (d1.month<d2.month)
              return -1;
         else if (d1.month>d2.month)
              return 1;
         else if (d1.day<d2.day)
              return -1;
         else if(d1.day>d2.day)
              return 1;
         else
              return 0;
    }
    printf("Error\n line number: %d ", __LINE__);
    return 0;
}

HistoryList makeEmptyHistoryList() {
    HistoryList result;
    result.head = NULL;
    return result;
}
