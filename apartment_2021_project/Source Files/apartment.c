//
//  apartment.c
//  apartment_2021_project
//
//  Created by Yinon Horev on 13/01/2021.
//

#include "apartment.h"
#include <time.h>
#include "utils.h"
#include <string.h>


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
    if (address != NULL) { // if Null we will allocate memory later
        char *strAdress = (char *) malloc(strlen(address) * sizeof(char));
        strcpy(strAdress, address);
        apt->data.address = strAdress;
    }

    apt->data.code = code;
    apt->data.price = price;
    apt->data.rooms = rooms;
    apt->data.availableDate.day = availableDay;
    apt->data.availableDate.month = availableMonth;
    apt->data.availableDate.year = availableYear;
    apt->data.entryDate = currTime;
    
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
        free(curr->data.address);
        free(curr);
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


void printApartmentNode(ApartmentNode *node) {
    printApartment(&(node->data));
}

// Merging two sorted lists.
ApartmentNode* MergeSortedList(ApartmentNode* lst1, ApartmentNode* lst2, bool ascend_flag)
{
    ApartmentNode* result = NULL;

    // Base Cases
    if (lst1 == NULL)
        return (lst2);
    else if (lst2 == NULL)
        return (lst1);

    // recursively merging two lists
    if (ascend_flag) {
        if (lst1->data.price <= lst2->data.price) {
            result = lst1;
            result->next = MergeSortedList(lst1->next, lst2, ascend_flag);
        }
        else {
            result = lst2;
            result->next = MergeSortedList(lst1, lst2->next, ascend_flag);
        }
    } else {
        if (lst1->data.price >= lst2->data.price) {
            result = lst1;
            result->next = MergeSortedList(lst1->next, lst2, ascend_flag);
        }
        else {
            result = lst2;
            result->next = MergeSortedList(lst1, lst2->next, ascend_flag);
        }
    }
    return result;
}

// Splitting two into halves.
// If the size of the list is odd, then extra element goes in the first list.
void SplitList(ApartmentNode* source, ApartmentNode** front, ApartmentNode** back)
{
    ApartmentNode* ptr1;
    ApartmentNode* ptr2;
    ptr2 = source;
    ptr1 = source->next;

    // ptr1 is incrmented twice and ptr2 is icremented once.
    while (ptr1 != NULL) {
        ptr1 = ptr1->next;
        if (ptr1 != NULL) {
            ptr2 = ptr2->next;
            ptr1 = ptr1->next;
        }
    }

    // ptr2 is at the midpoint.
    *front = source;
    *back = ptr2->next;
    ptr2->next = NULL;
}


// Merge Sort
void MergeSort(ApartmentNode** thead, bool ascend_flag)
{
    ApartmentNode* head = *thead;
    ApartmentNode* ptr1;
    ApartmentNode* ptr2;

  // Base Case
    if ((head == NULL) || (head->next == NULL)) {
        return;
    }

    // Splitting list
    SplitList(head, &ptr1, &ptr2);

    // Recursively sorting two lists.
    MergeSort(&ptr1, ascend_flag);
    MergeSort(&ptr2, ascend_flag);

    // Sorted List.
    *thead = MergeSortedList(ptr1, ptr2, ascend_flag);
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

void printApartmentList(apartmentList *apartmentList) {
    ApartmentNode *curr = apartmentList->head;
    while (curr != NULL) {//loop till the end of list
        printApartmentNode(curr);
        curr = curr->next;
    }
}

void filterApartmentsConditions(apartmentList* apartmentlst, apartmentList* filteredApartmentlst, int maxPrice,
                                         int minPrice, short int minNumRooms,
                                         short int maxNumRooms,Date d, int enter){
    ApartmentNode* head = apartmentlst->head;
    while (head != NULL) {
        if (head->data.price >= minPrice
            && head->data.price < maxPrice
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

