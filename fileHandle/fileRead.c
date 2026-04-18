
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "../exceptions/exception.h"
#include "./fileRead.h"

FILE* file = NULL;
int lineCounter = 0;

char* addExtenstionToName(char* fileNameWithoutExtension, char* extension){
    char* fileName = (char*)malloc(strlen((fileNameWithoutExtension) + strlen(extension) + 1) * sizeof(char) );
    strcpy(fileName,fileNameWithoutExtension);
    strcat(fileName, extension);
    return fileName;
}

int openFile(char* fileNameWithoutExtension, char* extension){
    char* fileName = addExtenstionToName(fileNameWithoutExtension, extension);
    file = fopen(fileName, "r");
    if(!file){
        printf("file %s is an empty file\n", fileName);
        free(fileName);
        return 0;
    }
    free(fileName);
    return 1;
}

void closeFile(){
    fclose(file);
    file = NULL;
    lineCounter = 0;
}

int getCurrLineCouter(){
    return lineCounter;
}

int readNextLine(char** buffer){
    int res;
    lineCounter++;
    res = readLineSuccess(buffer, file,lineCounter);
    return res;
}