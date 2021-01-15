//
//  files.c
//  apartment_2021_project
//
//  Created by Yinon Horev on 13/01/2021.
//

#include "files.h"
#include "history.h"

int counter = 1; // counter is the next apartment code. Starts from 1 as writen in instructions

void  readHistoryTxtFile(HistoryList *historyList, char **short_term_history)
{
    FILE *historyFilePtr;
    char *fileName = "history.txt", *line;
    char **filelines = NULL;
    int lineLen, numOfLines = 0 ;
    historyFilePtr= fopen(fileName, "r");
    if(historyFilePtr != NULL) {
        while (1) {
            fscanf(historyFilePtr, "%d", &lineLen);
            if (feof(historyFilePtr)) {
                break;
            }
            filelines = realloc(filelines, (sizeof(char*)* (numOfLines+1)));
            line = (char *) malloc(sizeof(char) * (lineLen + 1)); // adding 1 chars  for "\0"
            
            for (int cursor = 0; cursor < lineLen; cursor++) {
                line[cursor] = fgetc(historyFilePtr);
            }
            
            line[lineLen] = '\0';
            filelines[numOfLines] = line;
            numOfLines++;
        }
        
        for (int i = (numOfLines-1); i >= 0; i--) { // calling history handler backwards bcause file is written backwards
            historyHandler(filelines[i], short_term_history, historyList);
        }
        
        free(filelines);
    }
    else
        return; // file doesn't exist yet
    fclose(historyFilePtr);
}

void writeHistoryToTxtFile( HistoryList *historyList, char **short_term_history)
{
    FILE *historyFilePtr;
    HistoryListNode * head=historyList->head;
    char *fileName = HISTORY_FILE_NAME;
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

void parseToBitRepresentasion(BYTE *word1, BYTE* word2, BYTE* word3, ApartmentNode *aprtmentNode){
    *word1 = *word2 = *word3 = 0;
    *word1 |= (aprtmentNode->data.rooms << 4);
    *word1 |= (aprtmentNode->data.availableDate.day >> 1);
    *word2 |= ((aprtmentNode->data.availableDate.day & 0x01) << 7);
    *word2 |= ((aprtmentNode->data.availableDate.month) << 3);
    *word2 |= ((aprtmentNode->data.availableDate.year - 2000) >> 4);
    *word3 |= (((aprtmentNode->data.availableDate.year - 2000) & 0x0F) << 4);
    
}

void parseFromBitRepresentasion(BYTE *word1, BYTE* word2, BYTE* word3, ApartmentNode *aprtmentNode){
    aprtmentNode->data.rooms = aprtmentNode->data.availableDate.day = aprtmentNode->data.availableDate.month = aprtmentNode->data.availableDate.year = 0;
    aprtmentNode->data.rooms |= (*word1 >> 4);
    aprtmentNode->data.availableDate.day |= ((*word1 & 0x0F) << 1); // every line uses mask to extract the relevant bits from the Byte
    aprtmentNode->data.availableDate.day |= ((*word2 & 0x80) >> 7);
    aprtmentNode->data.availableDate.month |= ((*word2 & 0x78) >> 3);
    aprtmentNode->data.availableDate.year |= ((*word2 & 0x07) << 4);
    aprtmentNode->data.availableDate.year |= (((*word3) & 0xF0) >> 4);
    aprtmentNode->data.availableDate.year += 2000;
    
}

void writeApartmentToBinaryFile(ApartmentNode *apartmentNode, FILE *filePtr){
    short int addressSize = strlen(apartmentNode->data.address);
    BYTE MSBWord, middleWord, LSBWord;
    fwrite(&apartmentNode->data.code, sizeof(short int), 1, filePtr);
    fwrite(&addressSize, sizeof(short int), 1, filePtr);
    fwrite(apartmentNode->data.address, sizeof(char), addressSize, filePtr); // \0 will be omitted
    fwrite(&(apartmentNode->data.price), sizeof(int), 1, filePtr);
    fwrite(&(apartmentNode->data.entryDate), sizeof(time_t), 1, filePtr);
    parseToBitRepresentasion(&MSBWord, &middleWord, &LSBWord, apartmentNode);
    fwrite(&MSBWord, sizeof(BYTE), 1, filePtr);
    fwrite(&middleWord, sizeof(BYTE), 1, filePtr);
    fwrite(&LSBWord, sizeof(BYTE), 1, filePtr);
    
}

void readApartmentFromBinaryFile(ApartmentNode *apartmentNode, FILE *filePtr){
    short int addressSize;
    BYTE MSBWord, middleWord, LSBWord;
    fread(&apartmentNode->data.code, sizeof(short int), 1, filePtr);
    fread(&addressSize, sizeof(short int), 1, filePtr);
    apartmentNode->data.address = malloc((addressSize+1) * sizeof(char));
    apartmentNode->data.address[addressSize] = '\0';
    fread(apartmentNode->data.address, sizeof(char), addressSize, filePtr); // \0 will be omitted
    fread(&(apartmentNode->data.price), sizeof(int), 1, filePtr);
    fread(&(apartmentNode->data.entryDate), sizeof(time_t), 1, filePtr);
    fread(&MSBWord, sizeof(BYTE), 1, filePtr);
    fread(&middleWord, sizeof(BYTE), 1, filePtr);
    fread(&LSBWord, sizeof(BYTE), 1, filePtr);
    parseFromBitRepresentasion(&MSBWord, &middleWord, &LSBWord, apartmentNode);
    
    
}


void writeApartmentsToBinaryFile(apartmentList *apartmentList)
{
    FILE *filePtr;
    ApartmentNode *head= apartmentList->head;
    ApartmentNode *curr= head;
    int apartmentCount = 0;
    char *fileName = APARTMENTS_FILE_NAME;
    filePtr = fopen(fileName, "wb");

    if(filePtr == NULL) {
        printf("Something went wrong while opening %s for wb \n", fileName);
        exit(1);
    }
    if (head == NULL) {
        fclose(filePtr);
        return; // no apartments to save
    }
    /* Counting apartments */
    while (curr != NULL) {
        curr = curr->next;
        apartmentCount++;
    }
    fwrite(&counter, sizeof(int), 1, filePtr);
    fwrite(&apartmentCount, sizeof(int), 1, filePtr);
    curr = head;
    while (curr != NULL) {
        writeApartmentToBinaryFile(curr, filePtr);
        curr = curr->next;
    }


    fclose(filePtr);

}

void readApartmentsFromBinaryFile(apartmentList *apartmentList)
{
    FILE *filePtr;
    ApartmentNode *curr = NULL;
    int apartmentCount = 0;
    char *fileName = APARTMENTS_FILE_NAME;
    filePtr = fopen(fileName, "rb");

    if(filePtr == NULL) {
        // First run.
        return;
    }
    fread(&counter, sizeof(int), 1, filePtr);
    fread(&apartmentCount, sizeof(int), 1, filePtr);
    for (int i = 0; i < apartmentCount; i++) {
        curr = createApartmentNode(0, 0, 0, 0, 0, 0, NULL, NULL); // init without values so it can be read from binary file
        readApartmentFromBinaryFile(curr, filePtr);
        insertNodeToTail(apartmentList, curr);
    }

    fclose(filePtr);

}

void printHistoryListToFile(FILE *saveHistory, HistoryListNode *head)
{
    while(head!=NULL){
        printHistoryListToFile(saveHistory, head->next);
        fprintf(saveHistory, "%d", (int)strlen(head->command));
        fprintf(saveHistory, "%s", head->command);
        return;
    }
}

void printShortHistoryToFile(FILE *saveHistory, char **short_term_history)
{
    for(int i=0;i < N;i++){
        if(short_term_history[i]!=NULL){
            fprintf(saveHistory, "%d", (int)strlen(short_term_history[i]));
            fprintf(saveHistory,"%s",short_term_history[i]);
        }
    }
}
