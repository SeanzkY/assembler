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
    LabelTable* x = table;
    for(i=0;i<x->size;i++){
        printf("%s %d\n",x->labels[i]->name, x->labels[i]->address);
    }
    for(i=0;i<table->size;i++){
        if(strcmp(table->labels[i]->name,labelName) == 0){
            return table->labels[i];
        }
    }
    return NULL;
}
   
    
LabelTable* createLabelTable(char* fileName){
    char *line, *lineStart, *buffer;
    Symbol* currSymbol, *labelSymbol;
    LabelData* labelTemp;
    LabelTable* table = initTable();
    int ic = IC_START, dc = DC_START,  isLabel=0, i;
    openFile(fileName, ".am");
    readNextLine(&line);
    while(line){
        lineStart = line;
        buffer = getFirstWord(&line);
        currSymbol = generateSymbol(buffer);
        if(!currSymbol){
            printf("command: %s doesnt exist\n", buffer);
            return NULL;
        }
        if(currSymbol->type == LABEL){
            isLabel = 1;
            labelSymbol = currSymbol;
        }
        else if(currSymbol->type == DECLARATION){
            if(strcmp(currSymbol->name, ".string") == 0 ){
                if(isLabel){
                    isLabel = 0;
                    addToTable(table, generateLabelData(labelSymbol->name, dc, DATA));
                }
                strLiteralToBinary(line, &dc);
            }
            else if(strcmp(currSymbol->name, ".data") == 0){
                if(isLabel){
                    isLabel = 0;
                    addToTable(table, generateLabelData(labelSymbol->name, dc, DATA));
                }
                dataLiteralToBinary(line, &dc);
                
            }
            else if(strcmp(currSymbol->name, ".extern") == 0){
                labelTemp = getLabelFromTable(table, currSymbol->name);
                if(labelTemp && labelTemp->attr != EXTERNAL){
                    printf("error label: %s is in the table as external and as not external\n", currSymbol->name);
                }
                else{
                    free(buffer);
                    buffer = getFirstWord(&line);
                    addToTable(table, generateLabelData(buffer, 0, EXTERNAL));
                }
                
            }
            else if(strcmp(currSymbol->name, ".entry") != 0){
                printf("error in declaration %s is not defined\n", currSymbol->name);
            }
         
        }
        else if(currSymbol->type == COMMAND){
            if(isLabel){
                isLabel = 0;
                labelTemp = getLabelFromTable(table, labelSymbol->name);
                if(labelTemp){
                    printf("error label: %s\n is in the table as external and as not external\n", labelTemp->name);
                    free(labelTemp);
                }
                else{
                    addToTable(table, generateLabelData(labelSymbol->name, ic, CODE));
                }      
            }
            commandToBinary(currSymbol->name, line, &ic, NULL);
        }
        else if(currSymbol->type == COMMENT){
        }
        else{
            printf("inexistent command found: %s\n", currSymbol->name);
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
    printf("icf is %d\n", ic);
    return table;
}