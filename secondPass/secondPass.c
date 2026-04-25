#include <stdlib.h>
#include <string.h>
#include "stdio.h"

#include "./secondPass.h"
#include "../fileHandle/fileRead.h"
#include "../tokenization/tokenizer.h"
#include "../tokenization/symbolGenerator.h"
#include "../CodeGeneration/binaryGenerator.h"




void writeBinaryFile(char* fileName, LabelTable* table){
    int i,j;
    char *line, *lineStart, *buffer, *temp;
    binaryList *commandsLst = initBinaryList(), *dataLst = initBinaryList();
    Symbol* currSymbol;
    LabelData* labelTemp;
    int ic = IC_START, dc = DC_START,  isLabel=0;
    openFile(fileName, ".am");
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
            printf("adding command: %s\n", currSymbol->name);
            commandToBinary(currSymbol->name, line, &ic, table);
            /*addTwoBinaryLists(commandsLst ,commandToBinary(currSymbol->name, line, &ic, table));*/
        }
        else if(currSymbol->type == COMMENT){
        }
        if(!isLabel){
            readNextLine(&line);
        }
        
    }
    for(i=0;i<commandsLst->size;i++){
        for(j=0;j<12;j++){
            printf("%c", commandsLst->bin[i]->digits[j]);
        }
        printf("\n");
    }
    printf("icf is %d\n", ic);
}