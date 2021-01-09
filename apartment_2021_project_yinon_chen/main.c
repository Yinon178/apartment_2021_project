#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

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
void commandHandler(char *inputLine, apartmentList *aptList, HistoryList *historyList,char **short_term_history);

/*utility functions apartment linked list*/
apartmentList makeEmptyList(void);

int isEmptyList(apartmentList lst);

void insertNodeToHead(apartmentList *lst, ApartmentNode *newHead);

void printApartmentNode(ApartmentNode *node);

ApartmentNode *createApartmentNode(int code, int price, int short rooms,
                                   int short availableDay, int short availableMonth, int short availableYear,
                                   char *address, ApartmentNode *next);

void deleteApartmentByCode(int code, apartmentList *lst);

void checkMemoryAllocation(void *ptr);

int checkIfNeedToDelete(time_t entryDate, int day);

void deleteApartmentByDay(int day, apartmentList *lst);

/*Archives the line to short term history and if needed to long term*/
void archiveLine(char *short_term_history[]); // TODO: Chen please add long term history management. Yinon what is this function?

/* Apartment commands as described in the project instructions */
void addApt(char *inputLine, apartmentList *aptList);

void buyApt(char *inputLine, apartmentList *aptList);

void deleteApt(char *inputLine, apartmentList *aptList);

void gracefulExit(apartmentList *aptList);

/* Utility */
char **tokenize(char *line);

void printApartment(Apartment *apartment);

/* History commands*/

void initShortList(char **short_term_history);

void historyHandler(char *inputLine, char **short_term_history, HistoryList *historyList);

HistoryList makeEmptyHistoryList(void);

void insertToArchive(HistoryList *historyList, char *command);

HistoryListNode *createNewListNode(char *command);

bool isEmptyHisList(HistoryList *historyList);

void insertNodeToEndList(HistoryList *historyList, HistoryListNode *head);

void shortHistory(char **short_term_history, HistoryList *historyList);

void history(char **short_term_history, HistoryList *historyList);

void freeHistory(char **short_term_history, HistoryList *historyList);

char *getStrFromArchive(int index, HistoryList *historyList, char **short_term_history);

/* Text files */

void writeHistoryToTxtFile( HistoryList *historyList, char **short_term_history);

void printHistoryListToFile(FILE *saveHistory, HistoryListNode *head);

void printShortHistoryToFile(FILE *saveHistory, char **short_term_history);


int main(int argc, const char *argv[]) {
    char *short_term_history[N], *inputLine;
    initShortList(short_term_history);
    apartmentList aptList;
    aptList = makeEmptyList();
    HistoryList historyList;
    historyList = makeEmptyHistoryList();
    printPrompt();
    while (1) {
        inputLine = getLine();
        commandHandler(inputLine, &aptList, &historyList, short_term_history);

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

void commandHandler(char *inputLine, apartmentList *aptList, HistoryList *historyList, char **short_term_history) {
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
    if (strcmp(command, "find-apt") == 0){
        historyHandler(inputLine, short_term_history, historyList);
        printf("find-aptt init");
    }
    else if (strcmp(command, "buy-apt") == 0){
        historyHandler(inputLine, short_term_history, historyList);
        buyApt(inputLine, aptList);

    }
    else if (strcmp(command, "add-apt") == 0){
        historyHandler(inputLine, short_term_history, historyList);
        addApt(inputLine, aptList);
    }
    else if (strcmp(command, "history") == 0){
        history(short_term_history, historyList);
    }
    else if (strcmp(command, "delete-apt") == 0){
        deleteApt(inputLine, aptList);
    }
    else if (strcmp(command, "exit") == 0){
        writeHistoryToTxtFile(historyList, short_term_history);
        gracefulExit(aptList);
        freeHistory(short_term_history, historyList);
    }
    else if (strcmp(command, "!") == 0){
        printf("! init");
    }
    else if (strcmp(command, "short_history") == 0) {
        shortHistory(short_term_history, historyList);
    }
}

void shortHistory(char **short_term_history, HistoryList *historyList){
    HistoryListNode *curr;
    curr = historyList->head;
    int indexForPrint=1;

    while(curr!=NULL){
        curr=curr->next;
        indexForPrint++;
    }
    for(int i=6;i>=0;i--){
        if(short_term_history[i]!=NULL) {
            printf("%d: %s", indexForPrint, short_term_history[i]);
            indexForPrint++;
        }
    }
}

void history(char **short_term_history, HistoryList *historyList){
    HistoryListNode *curr;
    curr = historyList->head;
    int indexForPrint=1;

    while(curr!=NULL){
        printf("%d: %s",indexForPrint,curr->command);
        curr=curr->next;
        indexForPrint++;
    }
    for(int i=6;i>=0;i--){
        if(short_term_history[i]!=NULL) {
            printf("%d: %s", indexForPrint, short_term_history[i]);
            indexForPrint++;
        }
    }
}


void historyHandler(char *inputLine, char **short_term_history, HistoryList *historyList) {
    char *inputLineCopy = malloc(sizeof(char) * strlen(inputLine)); // TODO: free it up in exit
    checkMemoryAllocation(inputLineCopy);
    strcpy(inputLineCopy, inputLine);
    int currentIndex = 0;

    if(short_term_history[0]==NULL){
        short_term_history[0] = inputLineCopy;
    }
    else if(short_term_history[6] == NULL)
    {
        while (short_term_history[currentIndex] != NULL){
            currentIndex++;
        }
        for (; currentIndex >= 0; currentIndex--) {
            short_term_history[currentIndex + 1] = short_term_history[currentIndex];
        }
        short_term_history[0] = inputLineCopy;
    }

    else//if the array is full
    {
        insertToArchive(historyList, short_term_history[6]);
        for (currentIndex = 6; currentIndex >= 0; currentIndex--) {
            short_term_history[currentIndex + 1] = short_term_history[currentIndex];
        }
        short_term_history[0] = inputLineCopy;

    }


}

void initShortList(char **short_term_history){
    for(int i=0;i<=6;i++)
    {
        short_term_history[i]=NULL;
    }
}

void insertToArchive(HistoryList *historyList, char *command) {
    HistoryListNode *newHead;
    newHead = createNewListNode(command);
    insertNodeToEndList(historyList, newHead);
}


HistoryListNode *createNewListNode(char *command) {
    HistoryListNode *res;
    res = (HistoryListNode *) malloc(sizeof(HistoryListNode));
    res->command = command;
    res->next = NULL;
    return res;
}

void insertNodeToEndList(HistoryList *historyList, HistoryListNode *head) {
    if (isEmptyHisList(historyList)) {
        head->next = NULL;
        historyList->head = historyList->tail = head;
    } else {
        historyList->tail->next=head;
        historyList->tail=head;
    }
}

bool isEmptyHisList(HistoryList *historyList) {
    if (historyList->head == NULL)
        return true;
    else
        return false;
}

void freeHistory(char **short_term_history, HistoryList *historyList) {
    HistoryListNode *curr = historyList->head;
    HistoryListNode *next = NULL;

    while (curr != NULL) { //loop till the end of list
        next = curr->next;
        free(curr->command);
        free(curr);
        curr = next;
    }

}

apartmentList makeEmptyList() {
    apartmentList result;

    result.head = result.tail = NULL;
    return result;
}

int isEmptyList(apartmentList lst) {
    return (lst.head == NULL);
}

char *getStrFromArchive(int index, HistoryList *historyList, char **short_term_history)
{
    HistoryListNode *curr=historyList->head;
    int i=0;
    while(curr!=NULL){
        if(i==index){
            return curr->command;
        }
        curr=curr->next;
    }
    for(int j=6;j>=0;j--){
        if(i==index){
            return short_term_history[j];
        }
    }
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

void insertNodeToHead(apartmentList *lst, ApartmentNode *newHead) {
    if (isEmptyList(*lst) == true)
        lst->head = lst->tail = newHead;
    else {
        lst->tail->next = newHead;
        lst->tail = newHead;
    }
    newHead->next = NULL;
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

    if (checkIfNeedToDelete(curr->data.entryDate, day))//check if we need to free all list
    {
        lst->head = NULL;
    }

    while (curr != NULL) {//loop till the end of list

        next = curr->next;
        if (checkIfNeedToDelete(curr->data.entryDate, day))//check if the node was created in the right range time
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
        free(curr);
        curr = next;
    }
}

int checkIfNeedToDelete(time_t entryDate, int day) {
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
                                        (short int) strtol(tokens[6], NULL, 10),
                                        tokens[1], NULL);

    insertNodeToHead(aptList, apartmentNode);
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
           "Database entry date: %d-%02d-%02d",
           apartment->code,
           apartment->address,
           apartment->rooms,
           apartment->price,
           apartment->availableDate.day,
           apartment->availableDate.month,
           apartment->availableDate.year,
           tm.tm_mday, tm.tm_mon, tm.tm_year - 100);
}

HistoryList makeEmptyHistoryList() {
    HistoryList result;
    result.head = NULL;
    return result;
}

void writeHistoryToTxtFile( HistoryList *historyList, char **short_term_history)
{
    FILE *historyFilePtr;
    HistoryListNode * head=historyList->head;
    char *fileName = "history.txt";
    historyFilePtr= fopen(fileName, "w");

    if(historyFilePtr == NULL) {
        printf("File %s does not exist\n", fileName);
        exit(1);
    }
        if (head == NULL) {

            printShortHistoryToFile(historyFilePtr, short_term_history);
        }
        else
            {
            printShortHistoryToFile(historyFilePtr, short_term_history);
            printHistoryListToFile(historyFilePtr, head);
        }


        fclose(historyFilePtr);

}

void printHistoryListToFile(FILE *saveHistory, HistoryListNode *head)
{

    HistoryListNode *curr;
    curr=head;

    while(curr!=NULL){
        printHistoryListToFile(saveHistory, curr->next);
        fprintf(saveHistory, "%s", curr->command);

    }

}

void printShortHistoryToFile(FILE *saveHistory, char **short_term_history)
{
    for(int i=6;i>=0;i--){
        if(short_term_history[i]!=NULL){
            fprintf(saveHistory,"%s",short_term_history[i]);
        }
    }
}

