//
//  utils.c
//  apartment_2021_project
//
//  Created by Yinon Horev on 13/01/2021.
//

#include "utils.h"
#include "apartment.h"
#include <string.h>
#include <stdlib.h>

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

char* strReplace(char* search, char* replace, char* subject) {

    int i, j, k;
    int searchSize = (int)strlen(search);
    int replaceSize = (int)strlen(replace);
    int size = (int)strlen(subject);
    char* ret;
    if (!searchSize) {
        ret = malloc(size + 1);
        for (i = 0; i <= size; i++) {
            ret[i] = subject[i];
        }
        return ret;
    }
    int retAllocSize = (int)((strlen(subject) + 1) * 2); // Allocation size of the return string.

    // let the allocation size be twice as that of the subject initially

    ret = malloc(retAllocSize);
    int bufferSize = 0; // Found characters buffer counter
    char* foundBuffer = malloc(searchSize); // Found character bugger
    for (i = 0, j = 0; i <= size; i++) {
        /**
         * Double the size of the allocated space if it's possible for us to surpass it
         **/
        if (retAllocSize <= j + replaceSize) {
            retAllocSize *= 2;
            ret = (char*) realloc(ret, retAllocSize);
        }
            /**
             * If there is a hit in characters of the substring, let's add it to the
             * character buffer
             **/

        else if (subject[i] == search[bufferSize]) {
            foundBuffer[bufferSize] = subject[i];
            bufferSize++;
            /**
             * If the found character's bugger's counter has reached the searched substring's
             * length, then there's a hit. Let's copy the replace substring's characters
             * onto the return string.
             **/

            if (bufferSize == searchSize) {
                bufferSize = 0;
                for (k = 0; k < replaceSize; k++) {
                    ret[j++] = replace[k];

                }
            }
        }

            /**
             * If the character is a miss, let's put everything back from the buffer
             * to the return string, and set the found character buffer counter to 0.

             **/

        else {
            for (k = 0; k < bufferSize; k++) {
                ret[j++] = foundBuffer[k];
            }
            bufferSize = 0;

            /**
             * Add the current character in the subject string to the return string.
             **/

            ret[j++] = subject[i];
        }
    }
    /**
     * Free memory
     **/
    free(foundBuffer);
    return ret;
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

void printPrompt() {
    printf("Please enter one of the following commands:\n"
           "add-apt, find-apt, buy-apt, delete-apt or exit\n"
           "For reconstruction commands, please enter:\n"
           "!!, !num, history, short_history or !num^str1^str2\n");
}

void checkMemoryAllocation(void *ptr) {
    if (ptr == NULL) {
        printf("memory allocation failed");
        exit(1);
    }
}
