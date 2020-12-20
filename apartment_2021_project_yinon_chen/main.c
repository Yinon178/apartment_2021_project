#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
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
apartmentList makeEmptyList(void);
int isEmptyList(apartmentList lst);
void insertNodeToHead(apartmentList* lst, ApartmentNode* newHead);
ApartmentNode* createApartmentNode(int code, int price, int short rooms,
                                   int short entryDay,int short entryMonth,int short entryYear,
                                   int short availableDay,int short availableMonth,int short availableYear,
                                   char *address,ApartmentNode* next);
void checkMemoryAllocation(void* ptr);




int main(int argc, const char * argv[]) {
    char *short_term_history[N];
    printPrompt();
    return 0;
}

void printPrompt(void){
    printf("Please enter one of the following commands:\n"
           "add-apt, find-apt, buy-apt, delete-apt or exit\n"
           "For reconstruction commands, please enter:\n"
           "!!, !num, history, short_history or !num^str1^str2\n");
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
