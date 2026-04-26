


#include "preprocess.h"
#include "../fileHandle/fileRead.h"
#include "../exceptions/exception.h"
#include "../tokenization/tokenizer.h"
#include "../fileHandle/fileWrite.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>


/*this is just create new macro struct with the data it gets*/
MacroList* generateMacroNode(char* macroName, char* data){
    Macro* macro = (Macro*)malloc(sizeof(Macro));
    MacroList* ret = (MacroList*)malloc(sizeof(MacroList));
    macro->commands = data;
    macro->name = macroName;
    ret->value = macro;
    ret->next = NULL;
    return ret;
}

void freeMacroList(MacroList* list){
    MacroList* next = list;
    while(list){
        next = list->next;
        free(list->value->commands);
        free(list->value->name);
        free(list);
        list = next;
    }
}


Macro* getMacroNodeFromList(MacroList* list, char* macroName){
    while(list){
        if(strcmp(macroName, list->value->name) == 0)
            return list->value;
        list = list->next;
    }
    return NULL;
}


int isOnlySpace(char* word){
    while(*word && *word == ' ')
    {
        word++;
    }
    return strlen(word) == 0;
}
/*this function adds macro to list - the list it get is either empty or it gets the last element in that list
 - it also always change the pointer to point the last macro added*/
int addMacroToList(MacroList** macroLst, char* line, char* macroName){
    char* currWord = peekFirstWord(line);
    char* commands = NULL;
    char* currLine;
    int commandsSize = 0;
    if(currWord && !isOnlySpace(currWord)){
        printf("error in macro generator, %s not supposed to appear after macro name, line: %d \n", currWord, getCurrLineCouter());
    }
    if(!isMacroValid(*macroLst, line, macroName, getCurrLineCouter())){
        return 0;
    }
    readNextLine(&currLine);
    while(currLine && !strstr(currLine, "mcroend")){
        commands = (char*)realloc(commands, commandsSize + strlen(currLine) + 1);
        strcpy(commands + commandsSize, currLine);
        commandsSize += strlen(currLine);
        readNextLine(&currLine);
    }
    if(!currLine || strcmp(strtok(currLine," \n"), "mcroend") != 0){
        printf("error invalid mcroend\n");
        return 0;
    }
    if(!*macroLst){
        *macroLst = generateMacroNode(macroName, commands);
    }
    else{
        (*macroLst)->next = generateMacroNode(macroName, commands);
        (*macroLst) = (*macroLst)->next;
    }
    return 1;
}


int writeMacroFile(char* fileNameWithoutExtension){
    MacroList* macros = NULL;
    MacroList* macrosStart = NULL;
    char* currWord;
    char* macroName;
    char* currLine;
    char* currLineStart;
    int success;
    if(!openFile(fileNameWithoutExtension, ".as")){
        return 0;
    }
    if(!openFileWrite(fileNameWithoutExtension,".am")){
        closeFile();
        return 0;
    }
    success = readNextLine(&currLine);
    while(currLine){
        currLineStart = currLine;
        currWord = peekFirstWord(currLine);
        if(strcmp(currWord ,"mcro") == 0){
            getFirstWord(&currLine);
            if(!macros){
                addMacroToList(&macros, currLine, getFirstWord(&currLine));
                macrosStart = macros;
            }
            else{
                addMacroToList(&macros, currLine, getFirstWord(&currLine));
            }
        }
        else if(!isSavedKeyWord(currWord) && getMacroNodeFromList(macrosStart, currWord)){
            macroName = currWord;
            free(getFirstWord(&currLine));
            currWord = getFirstWord(&currLine);
            if(currWord && !isOnlySpace(currWord)){
                printf("error in macro appearance, %s not supposed to appear after macro name, line: %d \n", currWord, getCurrLineCouter());
            }
            writeNextLine(getMacroNodeFromList(macrosStart, macroName)->commands);
            free(macroName);
        }
        else{
            writeNextLine(currLine);
        }
        free(currWord);
        free(currLineStart);
        success = readNextLine(&currLine);
    }
    closeFile();
    closeFileWrite();
    freeMacroList(macrosStart);
    return 1;
}