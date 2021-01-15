//
//  apartment.h
//  apartment_2021_project
//
//  Created by Yinon Horev on 13/01/2021.
//

#ifndef apartment_h
#define apartment_h

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

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

struct apartmentNode* SortedMerge(struct apartmentNode* a, struct apartmentNode* b);

void freeApartList(apartmentList *lst);

/*MergeSort functions*/
void MergeSort(ApartmentNode** thead, bool ascend_flag);

void SplitList(ApartmentNode* source, ApartmentNode** front, ApartmentNode** back);

ApartmentNode* MergeSortedList(ApartmentNode* lst1, ApartmentNode* lst2, bool ascend_flag);

void printApartment(Apartment *apartment);

/* Checks if the serch conditions are met and filter the apartment list by it*/
void filterApartmentsConditions(apartmentList* apartmentlst, apartmentList* filteredApartmentlst, int maxPrice,
                                         int minPrice, short int minNumRooms,
                                         short int maxNumRooms, Date d, int enter);

#endif /* apartment_h */
