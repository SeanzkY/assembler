#include <stdlib.h>
#include <string.h>
#include "stdio.h"

#include "./secondPass.h"
#include "../fileHandle/fileRead.h"
#include "../fileHandle/fileWrite.h"
#include "../tokenization/tokenizer.h"
#include "../tokenization/symbolGenerator.h"
#include "../CodeGeneration/binaryGenerator.h"


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


void writeBinaryFile(char* fileName, LabelTable* table){
    int i,j;
    char *line, *lineStart, *buffer, *temp, *fileBuffer;
    binaryList *commandsLst = initBinaryList(), *dataLst = initBinaryList();
    Symbol* currSymbol;
    LabelData* labelTemp;
    int ic = IC_START, dc = DC_START,  isLabel=0;
    openFile(fileName, ".am");
    openFileWrite(fileName, ".ob");
    readNextLine(&line);
    while(line){
        lineStart = line;
        buffer = getFirstWord(&line);
        currSymbol = generateSymbol(buffer);
        if(currSymbol->type == LABEL){
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
                if(labelTemp->attr == CODE)
                    labelTemp->attr = CODE_AND_ENTRY;
                else if(labelTemp->attr == DATA)
                    labelTemp->attr = DATA_AND_ENTRY;
                else{
                    printf("error in entry label: %s\n", buffer);
                }
                
            }
        }
        else if(currSymbol->type == COMMAND){
            isLabel = 0;
            addTwoBinaryLists(commandsLst ,commandToBinary(currSymbol->name, line, &ic, table, fileName));
        }
        else if(currSymbol->type == COMMENT){
        }
        if(!isLabel){
            readNextLine(&line);
        }
        
    }

    fileBuffer = (char*)malloc(31);
    sprintf(fileBuffer, "   %d %d   \n", ic-IC_START, dc);
    writeNextLine(fileBuffer);
    free(fileBuffer);
    for(i=0;i<commandsLst->size;i++){
        fileBuffer = (char*)malloc(4 + MAX_BINARY_SIZE/4 + 1 + 5);
        sprintf(fileBuffer, "%04u %s  %c\n", commandsLst->bin[i]->pos, binaryCommandToHexa(commandsLst->bin[i]->digits, MAX_BINARY_SIZE), commandsLst->bin[i]->info);
        writeNextLine(fileBuffer);
        free(fileBuffer);
      }  
     for(i=0;i<dataLst->size;i++){
        fileBuffer = (char*)malloc(4 + MAX_BINARY_SIZE/4 + 1 + 5);
        sprintf(fileBuffer, "%04u %s  %c\n", dataLst->bin[i]->pos + ic, binaryCommandToHexa(dataLst->bin[i]->digits, MAX_BINARY_SIZE), dataLst->bin[i]->info);
        writeNextLine(fileBuffer);
        free(fileBuffer);
    }
    closeFileWrite();
    openFileWrite(fileName,".ent");
    for(i=0;i<table->size;i++){
        fileBuffer = (char*)malloc(24 + strlen(table->labels[i]->name) + 10);
        if(table->labels[i]->attr == CODE_AND_ENTRY || table->labels[i]->attr == DATA_AND_ENTRY){
            temp = (char*)malloc(strlen(table->labels[i]->name));
            strncpy(temp, table->labels[i]->name,strlen(table->labels[i]->name) - 1);
            temp[strlen(table->labels[i]->name) - 1] = '\0';
            sprintf(fileBuffer, "%s %04u \n", temp, table->labels[i]->address);
            writeNextLine(fileBuffer);
        }
      
        free(fileBuffer);
    }
}