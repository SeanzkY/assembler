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

addressTypesAllowed* createAddressTypeAllowed(int size, AddressType* allowed){
    int i;
    addressTypesAllowed* res = (addressTypesAllowed*)malloc(sizeof(addressTypesAllowed));
    res->size = size;
    res->address = (AddressType*)malloc(sizeof(AddressType) * size);
    for(i=0;i<size;i++){
        res->address[i] = allowed[i];
    }
    return res;
}

void initCommandsAllowed(){
    int i;
    addressTypesAllowed* allowed1 = createAddressTypeAllowed(3, (addressTypesAllowed*){IMMEDIATE, DIRECT, REGISTER_DIRECT});
    addressTypesAllowed* allowed2 = createAddressTypeAllowed(1, (addressTypesAllowed*){DIRECT});
    addressTypesAllowed* allowed3 = createAddressTypeAllowed(2, (addressTypesAllowed*){DIRECT, REGISTER_DIRECT});
    addressTypesAllowed* allowed4 = createAddressTypeAllowed(2, (addressTypesAllowed*){DIRECT, RELATIVE});
    for(i=0;i<4;i++){
        commands[i].src = allowed1;
    }
    commands[4].src = allowed2;
    
    commands[1].dst = allowed1;
    commands[0].dst = allowed3;
    for(i=2;i<9;i++){
        commands[i].dst = allowed3;
    }
    for(i=9;i<12;i++){
        commands[i].dst = allowed4;
    }
    commands[12].dst = allowed3;
    commands[13].dst = allowed1;
}

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

Symbol* isLabel(char* token){
    if(token && token[strlen(token) - 1] == ':')
        return allocateSymbol(token, LABEL);
    return NULL;
}

Symbol* isDeclaration(char* token){
     if(token && token[0] == '.')
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
    for(i=0; i < sizeof(res) / sizeof(Symbol*);i++){
        if(res[i])
            return res[i];
    }
    return NULL;

}