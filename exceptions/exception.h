


#include <stdio.h>
#include "../macros/preprocess.h"

int readLineSuccess(char** buffer, FILE* file, int lineCouter);


int isMacroValid(MacroList* macroLst, char* line, char* macroName, int lineCounter);


int isSavedKeyWord(char* name);