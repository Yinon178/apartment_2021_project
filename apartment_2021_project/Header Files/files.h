//
//  files.h
//  apartment_2021_project
//
//  Created by Yinon Horev on 13/01/2021.
//

#ifndef files_h
#define files_h

#include <stdio.h>
#include "history.h"

/* global variable declaration */
#define APARTMENTS_FILE_NAME "apartments.bin";
#define HISTORY_FILE_NAME "history.txt";
#define N 7 // short history len

typedef unsigned char BYTE;
extern int counter;

/* Text files */

void writeHistoryToTxtFile( HistoryList *historyList, char **short_term_history);

void printHistoryListToFile(FILE *saveHistory, HistoryListNode *head);

void printShortHistoryToFile(FILE *saveHistory, char **short_term_history);


/* Binary files - Every write function has compatible read function */

/* Codes data from aprtmentNode into 3 Bytes data structure*/
void parseToBitRepresentasion(BYTE *word1, BYTE* word2, BYTE* word3, ApartmentNode *aprtmentNode);

/* Decodes data to aprtmentNode fom 3 Bytes data structure*/
void parseFromBitRepresentasion(BYTE *word1, BYTE* word2, BYTE* word3, ApartmentNode *aprtmentNode);

void writeApartmentToBinaryFile(ApartmentNode *apartmentNode, FILE *filePtr);

void readApartmentFromBinaryFile(ApartmentNode *apartmentNode, FILE *filePtr);

void writeApartmentsToBinaryFile(apartmentList *apartmentList);

void readApartmentsFromBinaryFile(apartmentList *apartmentList);

void readHistoryTxtFile(HistoryList *historyList, char **short_term_history);

#endif /* files_h */
