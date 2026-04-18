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


Symbol* generateSymbol(char* token);