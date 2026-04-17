typedef enum SymbolType {
    MACRO_START,
    MACRO_END,
    MACRO_DECLARATION
    
} SymbolType;

typedef struct Symbol{
    char* name;
    SymbolType type;

}Symbol;


Symbol* generateSymbol(char* t);