#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../tokenization/symbolGenerator.h"
#include "../settings.h"
#include "./exception.h"




char* savedKeywords[] = {"mov", "cmp", "add", "sub", "lea", "clr","not","inc","dec","jmp","bne","jsr","red"
    ,"prn","rst","stop", ".entry", ".extern", "string", ".data", "mcrogen","r0","r1","r2","r3","r4","r5"
    ,"r6","r7"};

int isEndOfFile(FILE* file){
    long pos = ftell(file);     
    int res = fgetc(file) == EOF;
    fseek(file, pos, SEEK_SET); 
    return res;
    
}



/*reads line into buffer and return 0 if the line is too long*/
int readLineSuccess(char** buffer, FILE* file, int lineCouter){
    int isSuccess = 1;
    char* temp  = (char*)malloc(sizeof(char) * (MAX_LINE_SIZE+20));
    *buffer = (char*)malloc(sizeof(char) * (MAX_LINE_SIZE+20));
    if(!fgets(*buffer, (MAX_LINE_SIZE+20), file)){
        free(temp);
        free(*buffer);
        *buffer = NULL;
        return 1;
    }
    if(!strchr(*buffer, '\n') && !isEndOfFile(file)){
        printf("error in line %d, line is too long!\n", lineCouter);
        isSuccess = 0;
        while (!strchr(*buffer, '\n')) {
            if (!fgets(temp, (MAX_LINE_SIZE+20), file))        
                break;
            }
        *buffer = NULL;
    }   
    free(temp);
    return isSuccess;
}

int isMacroValid(MacroList* macroLst, char* line, char* macroName){
    int i;
    while(macroLst){
        if(strcmp(macroLst->value->name, macroName) == 0){
            printf("macro %s already exists\n", macroName);
            return 0;
        }
        macroLst = macroLst->next;
    }
    for(i=0;i<sizeof(savedKeywords)/ sizeof(char*);i++){
        if(strcmp(macroName, savedKeywords[i]) == 0){
            printf("macro %s is a saved keyword\n", macroName);
            return 0;
        }
    }
    return 1;
}

int isSymbolSyntaxException(Symbol* s, int line);

int isCommandParameterException(char* command, int paramsReceived);

int isUsedKeywordException(char* token);




