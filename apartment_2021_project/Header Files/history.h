//
//  history.h
//  apartment_2021_project
//
//  Created by Yinon Horev on 13/01/2021.
//

#ifndef history_h
#define history_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "utils.h"

#define N 7 // short history len

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

/* History commands*/

void initShortList(char **short_term_history);

HistoryList makeEmptyHistoryList(void);

void historyHandler(char *inputLine, char **short_term_history, HistoryList *historyList);

HistoryList makeEmptyHistoryList(void);

void insertToArchive(HistoryList *historyList, char *command);

HistoryListNode* createNewHistoryNode(char* command);

bool isEmptyHistoryList(HistoryList *historyList);

void insertHistoryNodeToTail(HistoryList *historyList, HistoryListNode *head);

void freeHistory(char **short_term_history, HistoryList *historyList);

char *getStrFromArchive(int index, HistoryList *historyList, char **short_term_history);

int get_N(void);

#endif /* history_h */
