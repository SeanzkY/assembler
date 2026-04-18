#include "stdio.h"
#include "stdlib.h"
#include <string.h>

#include "tokenization/tokenizer.h"
#include "macros/preprocess.h"

int main(){
    char* x = (char*)malloc(100);
    char* res = NULL;
    strcpy(x, " data1c ,   data2 ");
    writeMacroFile("code");
    if(!res)
        return 0;
    printf("%ss\n",res);
    return 0;

}