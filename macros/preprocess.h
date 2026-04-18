


typedef struct Macro{
    char* name;
    char* commands;
}Macro;


typedef struct MacroList{
    Macro* value;
    struct MacroList* next;
}MacroList;


void writeMacroFile(char* fileNameWithoutExtension);