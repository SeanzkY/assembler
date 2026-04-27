
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "./fileRead.h"

FILE* fileWrite = NULL;



/*opens file and manage it for writing*/
int openFileWrite(char* fileNameWithoutExtension, char* extension){
    char* fileName = addExtenstionToName(fileNameWithoutExtension, extension);
    fileWrite = fopen(fileName, "w");
    if(!fileWrite){
        printf("error in creating file: %s \n", fileName);
        return 0;
    }
    return 1;
}

/*closes file if opened*/
void closeFileWrite(){
    if(fileWrite)
        fclose(fileWrite);
    fileWrite = NULL;
}

/*wrtie line to file if opened if opened*/
int writeNextLine(char* buffer){
    if(fileWrite){
        int res;
        res = fputs(buffer, fileWrite);
        return res;
    }
}