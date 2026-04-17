typedef enum SymbolType {
    MACRO_START,
    MACRO_END,
    MACRO_DECLARATION,
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