
#include "commands.h"


typedef enum SymbolType {
    LABEL,
    DECLARATION,
    COMMAND,
    COMMENT
    
} SymbolType;



typedef struct Symbol{
    char* name;
    SymbolType type;

}Symbol;


int getCommandFunct(char* command);

int getCommandOpcode(char* command);

int getCommandParamNumber(char* command);

int isCommandAllowedAddress(char* command, AddressType addr, int isSrc);

Symbol* generateSymbol(char* token);

int isNumeric(char c);

#