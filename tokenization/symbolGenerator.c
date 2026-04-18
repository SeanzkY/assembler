#include "symbolGenerator.h"
#include "commands.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

CommandData commands[16] = {
    {"mov", -1, 0, 0, NULL, NULL },
    {"cmp", -1, 0, 1, NULL, NULL },
    {"add", 10, 1, 2, NULL, NULL },
    {"sub", 11, 1, 2, NULL, NULL },
    {"lea", -1, 0, 4, NULL, NULL },
    {"clr", 10, 1, 5, NULL, NULL },
    {"not", 11, 1, 5, NULL, NULL },
    {"inc", 12, 1, 5, NULL, NULL },
    {"dec", 13, 1, 5, NULL, NULL },
    {"jmp", 10, 1, 9, NULL, NULL },
    {"bne", 11, 1, 9, NULL, NULL },
    {"jsr", 12, 1, 9, NULL, NULL },
    {"red", -1, 0, 12, NULL, NULL },
    {"prn", -1, 0, 13, NULL, NULL },
    {"rst", -1, 0, 14, NULL, NULL },
    {"stop", -1, 0, 15, NULL, NULL }
};

Symbol* allocateSymbol(char* token, SymbolType type){
    Symbol* res = (Symbol*)malloc(sizeof(Symbol));
    res->name = token;
    res->type = type;
    return res;
}

Symbol* isCommand(char* token){
    int i;
    printf("passing on: %ld lines\n", sizeof(commands) / sizeof(CommandData));
    for(i=0;i<sizeof(commands) / sizeof(CommandData);i++){
        if(strcmp(commands[i].command, token) == 0){
            return allocateSymbol(token, COMMAND);
        }
    }
    return NULL;
}

Symbol* isLabel(char* token){
    if(token && token[strlen(token) - 1] == ':')
        return allocateSymbol(token, LABEL);
    return NULL;
}

Symbol* isDeclaration(char* token){
     if(token && token[strlen(token) - 1] == '.')
        return allocateSymbol(token, DECLARATION);
    return NULL;
}

Symbol* isComment(char* token){
    if(token && token[strlen(token) - 1] == ';')
        return allocateSymbol(token, COMMENT);
    return NULL;
}


Symbol* generateSymbol(char* token){
    int i;
    Symbol* res[4];
    res[0] = isCommand(token);
    res[1] = isDeclaration(token);
    res[2] = isComment(token);
    res[3] = isLabel(token);
    printf("passing on: %ld lines\n", sizeof(res) / sizeof(Symbol*));
    for(i=0; i < sizeof(res) / sizeof(Symbol*);i++){
        if(res[i])
            return res[i];
    }
    return NULL;

}