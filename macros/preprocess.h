

#ifndef MY_HEADER_H
#define MY_HEADER_H


typedef struct Macro{
    char* name;
    char* commands;
}Macro;


typedef struct MacroList{
    Macro* value;
    struct MacroList* next;
}MacroList;


void writeMacroFile(char* fileNameWithoutExtension);

#endif 