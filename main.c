#include "stdio.h"
#include "stdlib.h"
#include <string.h>

#include "tokenization/tokenizer.h"
#include "macros/preprocess.h"
#include "firstPass/firstPass.h"
#include "secondPass/secondPass.h"
#include "CodeGeneration/binaryGenerator.h"

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
   int i;
   LabelTable* x;
    writeMacroFile("code");    
    x = createLabelTable("code");
    for(i=0;i<x->size;i++){
        printf("%s %d\n",x->labels[i]->name, x->labels[i]->address);
    }
    writeBinaryFile("code", x);
    closeExtFile();
    return 0;


}