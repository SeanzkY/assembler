
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

FILE* fileWrite = NULL;


char* addExtenstionToNameWrite(char* fileNameWithoutExtension, char* extension){
    char* fileName = (char*)malloc(strlen((fileNameWithoutExtension) + strlen(extension) + 1) * sizeof(char) );
    strcpy(fileName,fileNameWithoutExtension);
    strcat(fileName, extension);
    return fileName;
}


int openFileWrite(char* fileNameWithoutExtension, char* extension){
    char* fileName = addExtenstionToNameWrite(fileNameWithoutExtension, extension);
    fileWrite = fopen(fileName, "w");
    if(!fileWrite){
        printf("error in creating file: %s \n", fileName);
        return 0;
    }
    return 1;
}


void closeFileWrite(){
    if(fileWrite)
        fclose(fileWrite);
    fileWrite = NULL;
}


int writeNextLine(char* buffer){
    if(fileWrite){
        int res;
        res = fputs(buffer, fileWrite);
        return res;
    }
}