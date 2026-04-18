#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbolGenerator.h"

#define END_SENTENCE '\n'
#define STRING_LITERAL_WRAPPER '\''
#define DATA_LITERAL_SEPARATOR ','
#define FIRST_WORD_SEPARATOR ' '

void skipSpaces(char** wordStart){
    while(**wordStart && **wordStart == ' '){
        (*wordStart)++;
    }
}

void trimEndSpaces(char** wordStart){
    if(strchr(*wordStart, ' ')){
        int firstSpacePos = strchr(*wordStart, ' ') - *wordStart;
        (*wordStart)[firstSpacePos] = '\0';
    }
    
    
}

char* strCopyWord(char* start, char* end){
    char* result;
    result = malloc((end-start + 2) * sizeof(char));
    strncpy(result, start ,end-start + 1);
    result[end-start+1] = '\0';
    return result;
}

/*this function is only for string literal - wrapped by STRING_LITERAL_WRAPPER*/
char* getNextWordStrLiteral(char** wordStart){
    char* currStr;
    skipSpaces(wordStart);
    if(!**wordStart){
        return NULL;
    }
    currStr = *wordStart;
    /*incase of string value*/
    if(*currStr == STRING_LITERAL_WRAPPER){
        currStr++;
        while(*currStr && *currStr != END_SENTENCE && *currStr != STRING_LITERAL_WRAPPER){
            currStr++;
        }
        if(*currStr == STRING_LITERAL_WRAPPER){
            return strCopyWord(*wordStart, currStr);
        }
        else
            return NULL;
    }
    return NULL;
}

/*this function is only for params - .data params and command param - separated by DATA_LITERAL_SEPARATOR*/
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
    *wordStart = currStr;
    return res;
}

/*this function is only for the first word in the line - its always separate the other by FIRST_WORD_SEPARATOR*/
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

char* peekFirstWord(char* lineStart){
    char* cpyLineStart = lineStart;
    return getFirstWord(&cpyLineStart);
}

/*this function returns*/
void splitLine(char** lineStart){
    Symbol* firstTokenRes = NULL;
    while(**lineStart && **lineStart != END_SENTENCE){
        if(firstTokenRes){
            if(firstTokenRes->type == LABEL){
                firstTokenRes = generateSymbol(getFirstWord(lineStart));
            }
            else if(firstTokenRes->type == COMMAND){
                while(**lineStart && **lineStart != END_SENTENCE){
                    getNextWordParams(lineStart);
                }
            }
            else if(firstTokenRes->type == DECLARATION){
                if(strcmp(firstTokenRes->name, ".string") == 0){
                    getNextWordStrLiteral(lineStart);
                }
                else if(strcmp(firstTokenRes->name, ".data") == 0){
                    while(**lineStart && **lineStart != END_SENTENCE){
                        getNextWordParams(lineStart);
                    }
                }
                else{
                    printf("exception\n");
                }
            }
        }
        else{
            firstTokenRes = generateSymbol(getFirstWord(lineStart));
        }
    }
}