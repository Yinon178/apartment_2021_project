//
//  history.c
//  apartment_2021_project
//
//  Created by Yinon Horev on 13/01/2021.
//

#include "history.h"


void historyHandler(char *inputLine, char **short_term_history, HistoryList *historyList) {
    char *inputLineCopy = malloc(sizeof(char) * strlen(inputLine)); // TODO: free it up in exit
    checkMemoryAllocation(inputLineCopy);
    strcpy(inputLineCopy, inputLine);
    int currentIndex = 0;

    if(short_term_history[0]==NULL){
        short_term_history[0] = inputLineCopy;
    }
    else if(short_term_history[(N - 1)] == NULL)
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
        insertToArchive(historyList, short_term_history[(N - 1)]);
        for (currentIndex = (N - 1); currentIndex >= 0; currentIndex--) {
            short_term_history[currentIndex + 1] = short_term_history[currentIndex];
        }
        short_term_history[0] = inputLineCopy;

    }


}

void initShortList(char **short_term_history){
    for(int i=0;i<=(N - 1);i++)
    {
        short_term_history[i]=NULL;
    }
}

void insertToArchive(HistoryList *historyList, char *command) {
    HistoryListNode *newHead;
    newHead = createNewHistoryNode(command);
    insertHistoryNodeToTail(historyList, newHead);

}


HistoryListNode* createNewHistoryNode(char* command)
{
    HistoryListNode* res;
    res = (HistoryListNode*)malloc(sizeof(HistoryListNode));
    res->command=command;

    res->next = NULL;
    return res;
}

void insertHistoryNodeToTail(HistoryList *historyList, HistoryListNode *head)
{
    if (isEmptyHistoryList(historyList) == true)
    {
        head->next = NULL;
        historyList->head = historyList->tail = head;
    } else {
        historyList->tail->next=head;
        historyList->tail=head;
    }
}

bool isEmptyHistoryList(HistoryList *historyList)
{
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
    
    for (int i = 0; i < 7; i++) {
        if(short_term_history[i]!=NULL)
        free(short_term_history[i]);
    }

}

char *getStrFromArchive(int index, HistoryList *historyList, char **short_term_history)
{
    HistoryListNode *curr=historyList->head;
    int i=0;
    index--;
    while(curr != NULL){
        if(i==index){
            return curr->command;

        }
        i++;
        curr=curr->next;
    }
    for(int j=(N - 1);j>=0;j--){
        if(short_term_history[j] != NULL){
            if(i==index){
                return short_term_history[j];
            }
            i++;
        }
    }
    printf("Error getting str from archive");
    exit(1);
    return NULL;
}

HistoryList makeEmptyHistoryList() {
    HistoryList result;
    result.head = NULL;
    return result;
}

int get_N(void){ return N; }
