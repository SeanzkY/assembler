#include <stdlib.h>
#include <string.h>
#include "stdio.h"

#include "./firstPass.h"
#include "../fileHandle/fileRead.h"
#include "../tokenization/tokenizer.h"
#include "../tokenization/symbolGenerator.h"
#include "../CodeGeneration/binaryGenerator.h"

LabelData* generateLabelData(char* name,  int address, LabelType attr){
    LabelData* res = (LabelData*)malloc(sizeof(LabelData));
    res->name = (char*)malloc((strlen(name) + 1) * sizeof(char));
    strcpy(res->name, name);
    res->address = address;
    res->attr = attr;
    return res;
}

LabelTable* initTable(){
    LabelTable* res = (LabelTable*)malloc(sizeof(LabelTable));
    res->labels = NULL;
    res->size = 0;
    return res;
}

LabelTable* addToTable(LabelTable* table, LabelData* data){
    table->size += 1;
    table->labels = (LabelData**)(realloc(table->labels ,sizeof(LabelData*) * table->size));
    table->labels[table->size - 1] = data;
    return table;
}

LabelData* getLabelFromTable(LabelTable* table, char* labelName){
    int i;   
    for(i=0;i<table->size;i++){
        if(strcmp(table->labels[i]->name,labelName) == 0){
            return table->labels[i];
        }
    }
    return NULL;
}
   
    
LabelTable* createLabelTable(char* fileName, int* isSuccess){
    char *line, *lineStart, *buffer, *saveBuffer;
    Symbol* currSymbol, *labelSymbol;
    LabelData* labelTemp;
    LabelTable* table = initTable();
    int ic = IC_START, dc = DC_START,  isLabel=0, i;
    openFile(fileName, ".am");
    readNextLine(&line);
    while(line){
        /*printf("curr command is %s", lineStart);*/
        lineStart = line;
        buffer = getFirstWord(&line);
        currSymbol = generateSymbol(buffer);
        if(!currSymbol){
            printf("command doesnt exist, line: %d command: %s", retLineNum(),lineStart);
            *isSuccess = 0;
        }
        else if(currSymbol->type == LABEL){
            isLabel = 1;
            labelSymbol = currSymbol;
        }
        else if(currSymbol->type == DECLARATION){
            if(strcmp(currSymbol->name, ".string") == 0 ){
                if(isLabel){
                    isLabel = 0;
                    addToTable(table, generateLabelData(labelSymbol->name, dc, DATA));
                }
                if(!strLiteralToBinary(line, &dc)){
                    printf("unexpected error - incorrect string input format, line: %d string: %s",retLineNum(),line);
                    *isSuccess = 0;
                }
            }
            else if(strcmp(currSymbol->name, ".data") == 0){
                if(isLabel){
                    isLabel = 0;
                    addToTable(table, generateLabelData(labelSymbol->name, dc, DATA));
                }
                if(!dataLiteralToBinary(line, &dc)){
                    printf("unexpected error - incorrect data input format, line: %d data: %s",retLineNum(),line);
                    *isSuccess = 0;
                }
                
            }
            else if(strcmp(currSymbol->name, ".extern") == 0){
                if(isLabel){
                    isLabel = 0;
                }
                buffer = getFirstWord(&line);
                labelTemp = getLabelFromTable(table, buffer);
                if(labelTemp && labelTemp->attr != EXTERNAL){
                    printf("error label: %s is in the table as external and as not external line: %d\n", currSymbol->name, retLineNum());
                    *isSuccess = 0;
                }
                else{
                    addToTable(table, generateLabelData(buffer, 0, EXTERNAL));
                }
                
                if(buffer){
                    saveBuffer = getFirstWord(&line);
                    if(saveBuffer && strlen(saveBuffer) >= 0)
                    {
                        printf("error .ext: external label is not in correct format line: %d data after external: %s \n", retLineNum(), saveBuffer);
                        *isSuccess = 0;        
                    }
                }
                
                
            }
            else if(strcmp(currSymbol->name, ".entry") != 0){
                 if(isLabel){
                    isLabel = 0;
                }
                printf("error in declaration %s is not defined in line %d\n", currSymbol->name, getCurrLineCouter());
                *isSuccess = 0;
            }
         
        }
        else if(currSymbol->type == COMMAND){
            if(isLabel){
                isLabel = 0;
                labelTemp = getLabelFromTable(table, labelSymbol->name);
                if(labelTemp){
                    printf("error label: %s\n is in the table as external and as not external\n", labelTemp->name);
                    *isSuccess = 0;
                    free(labelTemp);
                }
                else{
                    addToTable(table, generateLabelData(labelSymbol->name, ic, CODE));
                }      
            }
    
            commandToBinary(currSymbol->name, line, &ic, NULL, fileName, 0);
        }
        else if(currSymbol->type == COMMENT){
            isLabel = 0;
        }
        else{
            printf("inexistent command found: %s\n", currSymbol->name);
            *isSuccess = 0;
        }
        if(!isLabel){
            free(buffer);
            readNextLine(&line);
        }
        
    }
    for(i=0;i<table->size;i++){
        if(table->labels[i]->attr == DATA){
            table->labels[i]->address += ic;
        }
    }
    closeFile();
    return table;
}