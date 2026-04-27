#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbolGenerator.h"

#define END_SENTENCE '\n'
#define STRING_LITERAL_WRAPPER '\"'
#define DATA_LITERAL_SEPARATOR ','
#define FIRST_WORD_SEPARATOR ' '

/* set the pointer to the place where it start after all first
spaces */
void skipSpaces(char** wordStart){
    while(**wordStart && **wordStart == ' '){
        (*wordStart)++;
    }
}

/* set finish point to the string after all spaces
and no chars */
void trimEndSpaces(char** wordStart){
    char* start;
    int firstSpacePos;
    if(strchr(*wordStart, ' ')){
        start = strchr(*wordStart, ' ');
        while(*start && *start == ' '){
            start++;
        }
        if(*start == '\0'){
             firstSpacePos = strchr(*wordStart, ' ') - *wordStart;
            (*wordStart)[firstSpacePos] = '\0';
        }
    }
    
    
}

/* gets two pointers - to start and end
of string and returns it with new data allocated*/
char* strCopyWord(char* start, char* end){
    char* result;
    result = malloc((end-start + 2) * sizeof(char));
    strncpy(result, start ,end-start + 1);
    result[end-start+1] = '\0';
    return result;
}

/* split line - for string literal - for whats after .string  */
char* getNextWordStrLiteral(char** wordStart){
    char* currStr, *temp;
    skipSpaces(wordStart);

    if(!**wordStart){
        return NULL;
    }
    currStr = *wordStart;
    
    if(*currStr == STRING_LITERAL_WRAPPER){
        currStr++;
        while(*currStr && *currStr != END_SENTENCE && *currStr != STRING_LITERAL_WRAPPER){
            currStr++;
        }
        if(*currStr == STRING_LITERAL_WRAPPER){
            temp = strCopyWord(*wordStart, currStr);
            *wordStart = currStr+1;
            trimEndSpaces(wordStart);
            return temp;
        }
        else
            return NULL;
    }
    return NULL;
}

/* split line - for parameters - parameters for 
.data or parameters for commands */
char* getNextWordParams(char** wordStart){
    char* currStr;
    char* res;
    skipSpaces(wordStart);
      if(!**wordStart){
        return NULL;
    }
    currStr = *wordStart;
    while(*currStr && *currStr != END_SENTENCE && *currStr != DATA_LITERAL_SEPARATOR){
        currStr++;
    }
    res = strCopyWord(*wordStart, currStr-1);
    trimEndSpaces(&res);

    *wordStart = (currStr+1);
    return res;
}

/* split line - for first word in that
line */
char* getFirstWord(char** lineStart){
    char* currStr;
    char* res;
    skipSpaces(lineStart);
    currStr = *lineStart;
    while(*currStr && *currStr != END_SENTENCE && *currStr != FIRST_WORD_SEPARATOR){
        currStr++;
    }
    res = strCopyWord(*lineStart, currStr-1);
    *lineStart = currStr;
    return res;
}

/* split line - for first word in that
line  - without changing original line*/
char* peekFirstWord(char* lineStart){
    char* cpyLineStart = lineStart;
    return getFirstWord(&cpyLineStart);
}

