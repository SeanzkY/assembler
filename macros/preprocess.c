#include "preprocess.h"
#include "../fileHandle/fileRead.h"

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

/*this function adds macro to list - the list it get is either empty or it gets the last element in that list
 - it also always change the pointer to point the last macro added*/
int addMacroToList(MacroList** macroLst, char* line, char* macroName){
    char* currWord = strtok(NULL, " ,\n");
    char* commands = NULL;
    char* currLine;
    int commandsSize = 0;
    if(currWord){
        printf("error in macro not supposed to appear after macro name \n");
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

/*this function generates the entire macro list*/
MacroList* generateMacroData(){
    MacroList* macros = NULL;
    MacroList* macrosStart = NULL;
    char* currLine;
    int success = readNextLine(&currLine);
    int isError = 0;
    char* currWord;
    while(currLine){
        currWord = strtok(currLine, " ,\n");
        while(currWord){
            if(strcmp(currWord,"mcro") == 0){
                currWord = strtok(NULL, " \n");
                if(!macros){
                    if(addMacroToList(&macros, currLine, currWord)){
                        macrosStart = macros;
                    }
                    else
                        success = 0;
                }
                else{
                     if(!addMacroToList(&macros, currLine, currWord)){
                        success = 0;
                    }
                }
                
            }
            else if(strcmp(currWord,"mcroend") == 0){
                printf("exception in macro\n");
                /*exception*/
            }
            currWord = strtok(NULL, " ,\n");
        }
        if(!readNextLine(&currLine))
            success = 0;
    }
    if(!success)
        return NULL;
    return macrosStart;
}


void writeMacroFile(char* fileNameWithoutExtension){
    MacroList* lst;
    openFile("code", ".as");
    lst = generateMacroData();
    while(lst){
        printf("macro name is %s\n", lst->value->name);
        printf("macro data is\n %s", lst->value->commands);
        lst = lst->next;
    }
    closeFile();
}