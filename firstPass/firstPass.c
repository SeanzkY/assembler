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
    res->name = name;
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
   
    
LabelTable* createLabelTable(char* fileName){
    char *line, *lineStart, *buffer;
    Symbol* currSymbol, *labelSymbol;
    LabelData* labelTemp;
    LabelTable* table = initTable();
    int ic = IC_START, dc = DC_START,  isLabel=0;;
    openFile(fileName, ".am");
    readNextLine(&line);
    while(line){
        lineStart = line;
        buffer = getFirstWord(&line);
        currSymbol = generateSymbol(buffer);
        if(currSymbol->type == LABEL){
            isLabel = 1;
            labelSymbol = currSymbol;
        }
        else if(currSymbol->type == DECLARATION){
            isLabel = 0;
            if(strcmp(currSymbol->name, ".string") == 0 ){
                strLiteralToBinary(line, &dc);
                if(isLabel){
                    addToTable(table, generateLabelData(labelSymbol->name, dc, DATA));
                }
            }
            else if(strcmp(currSymbol->name, ".data") == 0){
                dataLiteralToBinary(line, &dc);
                 if(isLabel){
                    addToTable(table, generateLabelData(labelSymbol->name, dc, DATA));
                }
            }
            else if(strcmp(currSymbol->name, ".external") == 0){
                labelTemp = getLabelFromTable(table, labelSymbol->name);
                if(labelTemp->attr != EXTERNAL){
                    printf("error label: %s\n is in the table as external and as not external\n", labelTemp->name);
                    free(labelTemp);
                }
                else{
                    dataLiteralToBinary(line, &dc);
                    addToTable(table, generateLabelData(currSymbol->name, 0, ENTRY));
                }
                
            }
            else if(strcmp(currSymbol->name, ".entry") != 0){
                printf("error in declaration %s is not defined\n", currSymbol->name);
            }
         
        }
        else if(currSymbol->type == COMMAND){
            isLabel = 0;
            if(isLabel){
                labelTemp = getLabelFromTable(table, labelSymbol->name);
                if(labelTemp){
                    printf("error label: %s\n is in the table as external and as not external\n", labelTemp->name);
                    free(labelTemp);
                }
                else{
                    addToTable(table, generateLabelData(labelSymbol->name, ic, CODE));
                }      
            }
        }
        else if(currSymbol->type == COMMENT){
            
        }
        else{
            printf("inexistent command found: %s\n", currSymbol->name);
        }
        free(buffer);
        readNextLine(&line);
    }
    return table;
}