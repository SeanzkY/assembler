#include "stdio.h"
#include "stdlib.h"
#include <string.h>

#include "tokenization/tokenizer.h"

int main(){
    char* x = (char*)malloc(100);
    char* res;
    strcpy(x, " data1c ,   data2 ");
    res = getNextWordParams(&x);
    if(!res)
        return 0;
    printf("%ss\n",res);
    return 0;
}