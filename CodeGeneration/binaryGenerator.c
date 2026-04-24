#include "./binaryGenerator.h"
#include "../tokenization/tokenizer.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_DIGITS_BINARY 12 

binaryList* initBinaryList(){
    binaryList* res = (binaryList*)malloc(sizeof(binaryList));
    res->size = 0;
    res->bin = NULL;
    return res;
}

void addToBinaryList(binaryList* lst, binaryData* data){
    lst->bin = (binaryData**)realloc(lst->bin, sizeof(binaryData*) * (lst->size+1));
    lst->bin[lst->size] = data;
    lst->size++;
}

void addTwoBinaryLists(binaryList* lst1, binaryList* lst2){
    int i;
    for(i=0;i<lst2->size;i++){
        addToBinaryList(lst1, lst2->bin[i]);
    }
}

binaryData* intToBinary(unsigned int decimalNumber){
    /*this is the same function i used in mmn11 with minor changes*/
    binaryData* res = (binaryData*)malloc(sizeof(binaryData));
    const short BIT_MASK = 1;
    int i; 
    for(i=MAX_DIGITS_BINARY - 1; i>=0; i--) 
    {
        res->digits[MAX_DIGITS_BINARY - 1 - i] = (char)(((decimalNumber >> i) & BIT_MASK) + '0');
    }
    return res;
}

binaryList* strLiteralToBinary(char* strLiteral, int* dc){
    binaryList* res = initBinaryList();
    int size, i;
    strLiteral = getNextWordStrLiteral(&strLiteral);
    size =  strlen(strLiteral) - 1;
    if(strLiteral[0] != strLiteral[size] || strLiteral[0] != '\"'){
        printf("unexpected error - incorrect string input format\n");
        return NULL;
    }
    for(i=1;i<size;i++){
        addToBinaryList(res, intToBinary((int)strLiteral[i]));
        (*dc)++;
    }
    addToBinaryList(res, intToBinary(0));
    (*dc)++;
    return res;
}

binaryList* dataLiteralToBinary(char* dataLiteral, int* dc){
    char *buffer;
    int decimalNumber;
    binaryList* res = initBinaryList();
    buffer = getNextWordParams(&dataLiteral);
        while(buffer && strlen(buffer) != 0){
        printf("data is %s\n", buffer);
        if(sscanf(buffer, "%d", &decimalNumber) != 1) 
        {   
            printf("unexpected error - incorrect data input format\n");
            return NULL;
        }
        else{
            addToBinaryList(res, intToBinary(decimalNumber));
        }
        (*dc)++;
        free(buffer);
        buffer = getNextWordParams(&dataLiteral);
    }
    return res;
}

binaryList* commandToBinary(char* command, char* line , int* ic){

    char* buffer;
    buffer = getNextWordParams(&line);
    (*ic)++;
    while(buffer && strlen(buffer) != 0){
        printf("param is %s\n", buffer);
        buffer = getNextWordParams(&line);
        (*ic)++;
    }
}
