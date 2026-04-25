#include "./binaryGenerator.h"
#include "../tokenization/tokenizer.h"
#include "../tokenization/symbolGenerator.h"
#include "../tokenization/commands.h"
#include "../firstPass/firstPass.h"

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
    if(!lst2 || !lst1)
        return;
    for(i=0;i<lst2->size;i++){
        addToBinaryList(lst1, lst2->bin[i]);
    }
}

binaryData* intToBinary(unsigned int decimalNumber, LinkingInfo info, unsigned int pos){
    /*this is the same function i used in mmn11 with minor changes*/
    binaryData* res = (binaryData*)malloc(sizeof(binaryData));
    const short BIT_MASK = 1;
    int i; 
    for(i=MAX_DIGITS_BINARY - 1; i>=0; i--) 
    {
        res->digits[MAX_DIGITS_BINARY - 1 - i] = (char)(((decimalNumber >> i) & BIT_MASK) + '0');
    }
    res->info = info;
    res->pos = pos;
    return res;
}

char* completeToLabel(char* reference){
    char* res = (char*)malloc((strlen(reference)+2) * sizeof(char));
    strcpy(res, reference);
    strcat(res, ":");   
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
        addToBinaryList(res, intToBinary((int)strLiteral[i], A, *dc));
        (*dc)++;
    }
    addToBinaryList(res, intToBinary(0,A,*dc));
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
            addToBinaryList(res, intToBinary(decimalNumber, A, *dc));
        }
        (*dc)++;
        free(buffer);
        buffer = getNextWordParams(&dataLiteral);
    }
    return res;
}

AddressType getAddressType(char* operand){
    if(operand[0] == '#')
        return IMMEDIATE;
    else if(operand[0] == '%')
        return RELATIVE;
    else if(operand[0] == 'r' && (operand[1] >= '0' && operand[1] <= '7'))
        return REGISTER_DIRECT;
    else
        return DIRECT;
}

binaryData* translateOperand(char* operand, AddressType num, int ic, LabelTable* table){
    int decimalNumber;
    unsigned int res;
    LabelData* temp;
    char* operandWithAdder;
    if(num == IMMEDIATE){
        if(sscanf(operand+1, "%d", &decimalNumber) != 1) 
        {   
            printf("unexpected error - incorrect data input format\n");
            return NULL;
        }
        return intToBinary(decimalNumber, A, ic);
        
    }
    else if(num == RELATIVE){
        operandWithAdder = completeToLabel(operand);
        temp = getLabelFromTable(table, operandWithAdder+1);
        if(!temp){
            
            temp = getLabelFromTable(table, operand+1);
            if(temp->attr == EXTERNAL){
            
            }
            else{
                printf("fallback to external - need to add check");
            }
        }
        if(temp->address - (ic) > (1 << (MAX_BINARY_SIZE-1)) - 1 || temp->address - ic + 1 < -1 *(1 << (MAX_BINARY_SIZE-1))){
            printf("jump out of range\n");
            return NULL;
        }
        decimalNumber = temp->address;
        return intToBinary(decimalNumber - (ic), A, ic);
    }
    else if(num == REGISTER_DIRECT){
        return intToBinary(1 << (int)(operand[1] -'0'), A, ic);
    }
    else if(num == DIRECT){
        operandWithAdder = completeToLabel(operand);
        temp = getLabelFromTable(table, operandWithAdder);
        if(!temp){
            temp = getLabelFromTable(table, operand);
            if(temp->attr == EXTERNAL){
                
            }
            else{
                printf("fallback to external - need to add check");
            }
        }
            
        res = (unsigned int)temp->address;
        return intToBinary(res, A, ic);
    }
    return NULL;
}

binaryList* commandToBinary(char* command, char* line , int* ic, LabelTable* table){
    char* buffer;
    int i = 0;
    binaryData* temp = (binaryData*)malloc(sizeof(binaryData)), *commandData;
    binaryList* res = initBinaryList();
    int operandAddressType[2] = {0,0};
    temp->info = A;
    temp->pos = *ic;
    addToBinaryList(res,temp);
    commandData = intToBinary(getCommandOpcode(command), A, *ic);
    /*command Data is only supposed to reach 4 digits so i need so only it's last 4 digits, and each part of the command has it's own size*/
    strncpy(temp->digits, commandData->digits + MAX_BINARY_SIZE - 4, 4);
    commandData = intToBinary(getCommandFunct(command), A, *ic);
    strncpy(temp->digits + 4, commandData->digits + MAX_BINARY_SIZE - 4, 4);
    buffer = getNextWordParams(&line);
    (*ic)++;
    while(buffer && strlen(buffer) != 0){

        if(table){
            addToBinaryList(res, translateOperand(buffer,getAddressType(buffer), *ic, table));
        }
        
        /*addToBinaryList(res, translateOperand(buffer,getAddressType(buffer), *ic, table));*/
        (*ic)++;
        operandAddressType[i] = getAddressType(buffer); 
        
        buffer = getNextWordParams(&line);
        
        i++;
        
    }
    if(i==1){
        operandAddressType[1] = operandAddressType[0];
        operandAddressType[0] = 0;
    }
    commandData = intToBinary(operandAddressType[0], A, *ic);
    strncpy(temp->digits + 8, commandData->digits + MAX_BINARY_SIZE - 2, 2);
    commandData = intToBinary(operandAddressType[1], A, *ic);
    strncpy(temp->digits + 10, commandData->digits + MAX_BINARY_SIZE - 2, 2);
    return res;
}
