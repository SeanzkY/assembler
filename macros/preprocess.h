

#ifndef preprocessor
#define preprocessor


typedef struct Macro{
    char* name;
    char* commands;
}Macro;


typedef struct MacroList{
    Macro* value;
    struct MacroList* next;
}MacroList;


int writeMacroFile(char* fileNameWithoutExtension);

#endif 