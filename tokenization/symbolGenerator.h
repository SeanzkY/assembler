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

Symbol* generateSymbol(char* token);