#include "symbolGenerator.h"
#include "commands.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define EMPTY_ADDRS_LST {EMPTY_ADDRESS, EMPTY_ADDRESS, EMPTY_ADDRESS, EMPTY_ADDRESS}
#define ADDRESS_TYPE_1 {IMMEDIATE, DIRECT, REGISTER_DIRECT, EMPTY_ADDRESS}
#define ADDRESS_TYPE_2  {DIRECT,EMPTY_ADDRESS,EMPTY_ADDRESS,EMPTY_ADDRESS}
#define ADDRESS_TYPE_3 {DIRECT, REGISTER_DIRECT, EMPTY_ADDRESS, EMPTY_ADDRESS}
#define ADDRESS_TYPE_4 {DIRECT, RELATIVE, EMPTY_ADDRESS, EMPTY_ADDRESS}
#define MAX_LABEL_SIZE 31

CommandData commands[16] = {
    {"mov", 0 , 0, ADDRESS_TYPE_1, ADDRESS_TYPE_3},
    {"cmp", 0 , 1, ADDRESS_TYPE_1, ADDRESS_TYPE_1 },
    {"add", 10, 2, ADDRESS_TYPE_1, ADDRESS_TYPE_3 },
    {"sub", 11 , 2, ADDRESS_TYPE_1, ADDRESS_TYPE_3 },
    {"lea", 0 , 4, ADDRESS_TYPE_2, ADDRESS_TYPE_3 },
    {"clr", 10 , 5, EMPTY_ADDRS_LST, ADDRESS_TYPE_3},
    {"not", 11 , 5, EMPTY_ADDRS_LST, ADDRESS_TYPE_3 },
    {"inc", 12 , 5, EMPTY_ADDRS_LST, ADDRESS_TYPE_3 },
    {"dec", 13 , 5, EMPTY_ADDRS_LST, ADDRESS_TYPE_3 },
    {"jmp", 10 , 9, EMPTY_ADDRS_LST, ADDRESS_TYPE_4},
    {"bne", 11 , 9, EMPTY_ADDRS_LST, ADDRESS_TYPE_4 },
    {"jsr", 12 , 9, EMPTY_ADDRS_LST, ADDRESS_TYPE_4 },
    {"red", 0 , 12, EMPTY_ADDRS_LST, ADDRESS_TYPE_3 },
    {"prn", 0 , 13, EMPTY_ADDRS_LST, ADDRESS_TYPE_1 },
    {"rts", 0 , 14, EMPTY_ADDRS_LST, EMPTY_ADDRS_LST },
    {"stop", 0 , 15, EMPTY_ADDRS_LST, EMPTY_ADDRS_LST }
};



CommandData* getCommandData(char* command){
    int i;
    CommandData* res;
    for(i=0;i<sizeof(commands) / sizeof(CommandData);i++){
        if(strcmp(commands[i].command, command) == 0){
            res = (CommandData*)malloc(sizeof(CommandData));
            *res =  commands[i];
            return res;
        }
    }
    return NULL;
}

int isCommandAllowedAddress(char* command, AddressType addr, int isSrc){
    int i;
    AddressType* cmp;
    CommandData* res = getCommandData(command);
    if(isSrc)
        cmp = res->src;
    else
        cmp = res->dst;

    for(i=0;i<4;i++){
        if(cmp[i] == addr)
            return 1;
    }
    free(res);
    return 0;
}

int getCommandParamNumber(char* command){
    int i = 0;
    CommandData* res = getCommandData(command);
    AddressType* src = res->src;
    AddressType* dst = res->dst;
    free(res);
    if(src[0] != EMPTY_ADDRESS)
        i++;
    if(dst[0] != EMPTY_ADDRESS)
        i++;
    return i;
}

int getCommandFunct(char* command){
    CommandData* res = getCommandData(command);
    int funct = res->funct;
    free(res);
    return funct;
}

int getCommandOpcode(char* command){
    CommandData* res = getCommandData(command);
    int opCode = res->opCode;
    free(res);
    return opCode;
}   

Symbol* allocateSymbol(char* token, SymbolType type){
    Symbol* res = (Symbol*)malloc(sizeof(Symbol));
    res->name = token;
    res->type = type;
    return res;
}

Symbol* isCommand(char* token){
    CommandData* res = getCommandData(token);
    if(res){
        free(res);
        return allocateSymbol(token, COMMAND);
    }
    return NULL;
}

int isAlpha(char c){
    return ((c <= 'z' && c >= 'a')  || (c <= 'Z' && c >= 'A'));
}

int isNumeric(char c){
    return (c <= '9' && c >= '0');
}

Symbol* isLabel(char* token){
    int i;
    if(token && token[strlen(token) - 1] == ':' && strlen(token) <= MAX_LABEL_SIZE){
        if(!isAlpha(token[0])){
            return NULL;

        }
        for(i=1;i<strlen(token)-1;i++){
             if(!isAlpha(token[i]) && !isNumeric(token[i])){
                return NULL;

             }
        }
        return allocateSymbol(token, LABEL);
    }
    return NULL;
}

Symbol* isDeclaration(char* token){
     if(token && token[0] == '.')
        return allocateSymbol(token, DECLARATION);
    return NULL;
}

Symbol* isComment(char* token){
    if(token && token[0] == ';')
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
    for(i=0; i < sizeof(res) / sizeof(Symbol*);i++){
        if(res[i])
            return res[i];
    }
    return NULL;

}