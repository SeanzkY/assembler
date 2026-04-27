#include <stdlib.h>
#include <string.h>
#include "stdio.h"

#include "./secondPass.h"
#include "../fileHandle/fileRead.h"
#include "../fileHandle/fileWrite.h"
#include "../tokenization/tokenizer.h"
#include "../tokenization/symbolGenerator.h"
#include "../CodeGeneration/binaryGenerator.h"

/* turn binary command of fixed size to hexa */
char* binaryCommandToHexa(char* bin, unsigned int sizeBin){
    int maxSizeHexa = sizeBin / 4;
    char* res = (char*)malloc(sizeof(char)  * (maxSizeHexa+1));
    int i, temp,j;
    for(i=0;i<sizeBin;i+=4){
        temp = 0;
        for(j=0;j<4;j++){
            temp += (int)(bin[sizeBin-i-1-j] - '0') << j;
        }
        if(temp > 9){
            res[maxSizeHexa -1 - i / 4] = (temp-10) + 'A';
        }
        else{
            res[maxSizeHexa -1 - i / 4] = temp + '0';
        }
    }
    res[maxSizeHexa] = '\0';
    return res;
}

/* this is the function for second passing
genFile meaning if need to generate file or not if 
there was an error or we dont have entire table now
it writes the entire hexa code to file and the ext file */
void writeBinaryFile(char* fileName, LabelTable* table, int genFile){
    int i;
    char *line, *buffer, *temp, *fileBuffer;
    binaryList *commandsLst = initBinaryList(), *dataLst = initBinaryList(), *tempList;
    Symbol* currSymbol;
    LabelData* labelTemp;
    int ic = IC_START, dc = DC_START,  isLabel=0;
    openFile(fileName, ".am");
    readNextLine(&line);
    while(line){
        buffer = getFirstWord(&line);
        currSymbol = generateSymbol(buffer);
        if(!currSymbol){
            if(line && line[0] != '\n')
                genFile = 0;
        }
        else if(currSymbol->type == LABEL){
            isLabel = 1;
        }
        else if(currSymbol->type == DECLARATION){
            isLabel = 0;
            if(strcmp(currSymbol->name, ".string") == 0 ){
                addTwoBinaryLists(dataLst, strLiteralToBinary(line, &dc)); 
                
            }
            else if(strcmp(currSymbol->name, ".data") == 0){
                addTwoBinaryLists(dataLst, dataLiteralToBinary(line, &dc));
            }
            else if(strcmp(currSymbol->name, ".entry") == 0 ){
                buffer = getFirstWord(&line);
                temp = completeToLabel(buffer);
                labelTemp =  getLabelFromTable(table, temp);
                free(temp);
                if(!labelTemp){
                    printf("entry: %s doesn't exist in file, line: %d\n", buffer, getCurrLineCouter());
                    genFile = 0;
                }
                else{
                    if(labelTemp->attr == CODE)
                    labelTemp->attr = CODE_AND_ENTRY;
                    else if(labelTemp->attr == DATA)
                        labelTemp->attr = DATA_AND_ENTRY;
                    else{
                        printf("entry: %s doesn't exist in file in correct way, line: %d\n", buffer, getCurrLineCouter());
                        genFile = 0;
                    }
                }
                free(buffer);
                
                
            }
        }
        else if(currSymbol->type == COMMAND){
            isLabel = 0;
            tempList = commandToBinary(currSymbol->name, line, &ic, table, fileName, 1);
            if(!tempList){
                genFile = 0;
            }
            else{
                addTwoBinaryLists(commandsLst , tempList);
            }

        }
        else if(currSymbol->type == COMMENT){
             isLabel = 0;
        }
        if(!isLabel){
            readNextLine(&line);
        }
        
    }
    /*if failure no need to write file*/
    if(genFile)
        openFileWrite(fileName, ".ob");
    fileBuffer = (char*)malloc(31);
    sprintf(fileBuffer, "   %d %d   \n", ic-IC_START, dc);
    writeNextLine(fileBuffer);
    free(fileBuffer);
    /*write data and code files*/
    for(i=0;i<commandsLst->size;i++){
        char* hexa = binaryCommandToHexa(commandsLst->bin[i]->digits, MAX_BINARY_SIZE);
        fileBuffer = (char*)malloc(4 + MAX_BINARY_SIZE/4 + 1 + 5);
        sprintf(fileBuffer, "%04u %s  %c\n", commandsLst->bin[i]->pos, hexa, commandsLst->bin[i]->info);
        free(hexa);
        writeNextLine(fileBuffer);
        free(fileBuffer);
      }  
     for(i=0;i<dataLst->size;i++){
        char* hexa = binaryCommandToHexa(dataLst->bin[i]->digits, MAX_BINARY_SIZE);
        fileBuffer = (char*)malloc(4 + MAX_BINARY_SIZE/4 + 1 + 5);
        sprintf(fileBuffer, "%04u %s  %c\n", dataLst->bin[i]->pos + ic, hexa, dataLst->bin[i]->info);
        free(hexa);
        writeNextLine(fileBuffer);
        free(fileBuffer);
    }
    closeFileWrite();
    if(genFile){
        commitExtFile(fileName);
        openFileWrite(fileName,".ent");
    }
        
    for(i=0;i<table->size;i++){
        fileBuffer = (char*)malloc(24 + strlen(table->labels[i]->name) + 10);
        if(table->labels[i]->attr == CODE_AND_ENTRY || table->labels[i]->attr == DATA_AND_ENTRY){
            temp = (char*)malloc(strlen(table->labels[i]->name));
            strncpy(temp, table->labels[i]->name,strlen(table->labels[i]->name) - 1);
            temp[strlen(table->labels[i]->name) - 1] = '\0';
            sprintf(fileBuffer, "%s %04u \n", temp, table->labels[i]->address);
            writeNextLine(fileBuffer);
            free(temp);
        }
        else{
            fileBuffer[0] = '\0';
        }
        free(fileBuffer);
    }
}