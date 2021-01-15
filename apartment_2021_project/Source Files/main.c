//  apartment_2021_project
//Submiters ינון חורב: 319038758 and 315614263 : חן קוטלר
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <limits.h>
#include "apartment.h"
#include "utils.h"
#include "history.h"
#include "files.h"

extern int counter; // global counter

/* Calls the relevant command by the input from the user*/
void commandHandler(char *inputLine, apartmentList *aptList, HistoryList *historyList, char **short_term_history);

/* Apartment commands as described in the project instructions */
void addApt(char *inputLine, apartmentList *aptList);

void buyApt(char *inputLine, apartmentList *aptList);

void findApt(char *inputLine, apartmentList *aptList);

void deleteApt(char *inputLine, apartmentList *aptList);

void gracefulExit(apartmentList *aptList, HistoryList *historyList, char **short_term_history);

void recommand(char *inputLine, HistoryList *historyList, char **short_term_history, apartmentList *aptList);

void shortHistory(char **short_term_history, HistoryList *historyList);

void history(char **short_term_history, HistoryList *historyList);

/*  ! functions */
int main(int argc, const char *argv[]) {
    char *short_term_history[N], *inputLine;
    initShortList(short_term_history);
    apartmentList aptList;
    aptList = makeEmptyList();
    readApartmentsFromBinaryFile(&aptList);
    HistoryList historyList;
    historyList = makeEmptyHistoryList();
    printPrompt();
    readHistoryTxtFile(&historyList,short_term_history);
    while (1) {
        inputLine = getLine();
        commandHandler(inputLine, &aptList, &historyList, short_term_history);

    }

    return 0;
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
    if (strcmp(command, "find-apt") == 0) {
        historyHandler(inputLine, short_term_history, historyList);
        findApt(inputLine, aptList);
    }
    else if (strcmp(command, "buy-apt") == 0) {
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
        historyHandler(inputLine, short_term_history, historyList);
        deleteApt(inputLine, aptList);
    }
    else if (strcmp(command, "exit") == 0){
        gracefulExit(aptList, historyList, short_term_history);
    }
    else if (strcmp(command, "!") == 0){
        recommand(inputLine, historyList, short_term_history, aptList);
    }
    else if (strcmp(command, "short_history") == 0) {
        shortHistory(short_term_history, historyList);
    }
    free(inputLine); // Some commands use strtok which manipulates string therefore it must be freed here
}

void shortHistory(char **short_term_history, HistoryList *historyList){
    HistoryListNode *curr;
    curr = historyList->head;
    int indexForPrint=1;

    while(curr!=NULL){
        curr=curr->next;
        indexForPrint++;
    }
    for(int i=(N - 1);i>=0;i--){
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
    for(int i=(N - 1);i>=0;i--){
        if(short_term_history[i]!=NULL) {
            printf("%d: %s", indexForPrint, short_term_history[i]);
            indexForPrint++;
        }
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
    free(tokens);
}

void buyApt(char *inputLine, apartmentList *aptList) {
    char **tokens = NULL;
    tokens = tokenize(inputLine);
    deleteApartmentByCode((int) strtol(tokens[1], NULL, 10), aptList);
    free(tokens);
}

void deleteApt(char *inputLine, apartmentList *aptList) {
    char **tokens = NULL;
    tokens = tokenize(inputLine);
    deleteApartmentByDay((int) strtol(tokens[2], NULL, 10), aptList);
    free(tokens);
}

void findApt(char *inputLine, apartmentList *aptList){
    /* This function will build a dynamic if statment and serch for apartments that checks out with this if*/
    apartmentList filteredApartmentList = makeEmptyList();
    int maxPrice=INT_MAX, minPrice=0, enter=INT_MAX, i=1;
    Date d;
    /* initalizing to a passed date */
    d.day = 31;
    d.month = 12;
    d.year = 2127; // maximal supported date of app
    short int minNumRooms=-1, maxNumRooms=INT8_MAX;
    char **tokens = tokenize(inputLine);
    bool fromLowToHigh=false, fromHighTolow=false;
    
    /* Parsing string */
    while (tokens[i] != NULL) {
        if (!strcmp(tokens[i], "-MaxPrice")) {
            i++;
            maxPrice = (int)strtol(tokens[i], NULL, 10);
        }
        else if (!strcmp(tokens[i], "-MinPrice")) {
            i++;
            minPrice = (int)strtol(tokens[i], NULL, 10);
        }
        else if (!strcmp(tokens[i], "-MinNumRooms")) {
            i++;
            minNumRooms = (short int)strtol(tokens[i], NULL, 10);
        }
        else if (!strcmp(tokens[i], "-MaxNumRooms")) {
            i++;
            maxNumRooms = (short int)strtol(tokens[i], NULL, 10);
        }
        else if (!strcmp(tokens[i], "-Date")) {
            i++;
            sscanf(tokens[i], "%2hd%2hd%4hd", &d.day, &d.month, &d.year);
        }
        else if (!strcmp(tokens[i], "-Enter")) {
            i++;
            enter = (short int)strtol(tokens[i], NULL, 10);
        }
        else if (!strcmp(tokens[i], "-s\n")) {
            fromLowToHigh=true;
        }
        else if (!strcmp(tokens[i], "-sr\n")) {
            fromHighTolow=true;
        }
        else {
            printf("Error parsing %s \nline number: %d", tokens[i], __LINE__);
            exit(1);
        }
        i++;
    }
    filterApartmentsConditions(aptList, &filteredApartmentList , maxPrice, minPrice, minNumRooms, maxNumRooms, d, enter);
    
    if (fromLowToHigh)
        MergeSort(&(filteredApartmentList.head), true);
    else if (fromHighTolow)
        MergeSort(&(filteredApartmentList.head), false);
    
    printApartmentList(&filteredApartmentList);
    free(tokens);
    
}

void gracefulExit(apartmentList *aptList, HistoryList *historyList, char **short_term_history) {
    writeHistoryToTxtFile(historyList, short_term_history);
    writeApartmentsToBinaryFile(aptList);
    freeApartList(aptList);
    freeHistory(short_term_history, historyList);
    printf("Good Bye!\n");
    exit(0);
}

void recommand(char *inputLine, HistoryList *historyList, char **short_term_history, apartmentList *aptList){
    char *tempArchivedLine = NULL, *archivedLine = NULL;
    if (strlen(inputLine) <= 3) { // either !! or !<num>
        if (inputLine[1] == '!') {
            archivedLine = short_term_history[0];
            tempArchivedLine = malloc(sizeof(char) * strlen(inputLine));
            checkMemoryAllocation(tempArchivedLine);
            strcpy(tempArchivedLine, archivedLine);
            
        } else {
            inputLine++;
            int index;
            sscanf(inputLine, "%d", &index);
            archivedLine = getStrFromArchive(index, historyList, short_term_history);
            tempArchivedLine = malloc(sizeof(char) * strlen(inputLine));
            checkMemoryAllocation(tempArchivedLine);
            strcpy(tempArchivedLine, archivedLine);
        }
    } else { // String switch
        int index;
        char *str1, *str2;
        str1 = calloc(sizeof(char), strrchr(inputLine,'^') - strchr(inputLine,'^'));
        str2 = calloc(sizeof(char), strrchr(inputLine, '\n') - strrchr(inputLine,'^'));
        sscanf(inputLine, "!%d^%[^'^']^%[^\n]\n", &index, str1, str2);
        archivedLine = getStrFromArchive(index, historyList, short_term_history);
        tempArchivedLine = strReplace(str1, str2, archivedLine);
    }
    commandHandler(tempArchivedLine, aptList, historyList, short_term_history);
}
