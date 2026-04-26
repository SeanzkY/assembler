#include "stdio.h"
#include "stdlib.h"
#include <string.h>

#include "tokenization/tokenizer.h"
#include "macros/preprocess.h"
#include "firstPass/firstPass.h"
#include "secondPass/secondPass.h"
#include "CodeGeneration/binaryGenerator.h"

int main(int argc, char* argv[]){
   int i,j, success;
   LabelTable* x;
   if(argc <= 1){
        printf("please provide a valid file to scan\n");
        return 1;
    }
    else{
        for(i=1;i<argc;i++){
            success = 1;
            if(writeMacroFile(argv[i])){
                x = createLabelTable(argv[i], &success);
                for(j=0;j<x->size;j++){
                    printf("%s %d\n",x->labels[j]->name, x->labels[j]->address);
                }
                writeBinaryFile(argv[i], x, success);
                closeExtFile();
                
            }

           
        }
        return 0;
    }
   


}