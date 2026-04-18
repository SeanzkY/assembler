#include "stdio.h"
#include "stdlib.h"
#include <string.h>

#include "tokenization/tokenizer.h"
#include "macros/preprocess.h"

int main(){
     /*
    char* x = (char*)malloc(100);
    char* res = NULL;
    strcpy(x, "          command  param1, param2\n   ");
    printf("first word is %sd\n", peekFirstWord(x));
    printf("left is %s\n", x);
   
    printf("second word is %sd\n", getNextWordParams(&x));
    printf("left is %s\n", x);

    x++;
    printf("third word is %sd\n", getNextWordParams(&(x)));
    printf("left is %s s\n", x);

    if(!res)
        return 0;

    */
    writeMacroFile("code");    

    
    return 0;


}