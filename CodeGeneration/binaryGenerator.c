#include "./binaryGenerator.h"
#include "../tokenization/tokenizer.h"
#include "../tokenization/symbolGenerator.h"
#include "../tokenization/commands.h"
#include "../firstPass/firstPass.h"
#include "../fileHandle/fileWrite.h"
#include "../fileHandle/fileRead.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_DIGITS_BINARY 12 

FILE* extFile = NULL;
int extFileSize = 0;
char** extFileData = NULL;


/*init binary list - list of binary commands*/
binaryList* initBinaryList(){
    binaryList* res = (binaryList*)malloc(sizeof(binaryList));
    res->size = 0;
    res->bin = NULL;
    return res;
}
/*add binary data to list*/
void addToBinaryList(binaryList* lst, binaryData* data){
    if(data){
        lst->bin = (binaryData**)realloc(lst->bin, sizeof(binaryData*) * (lst->size+1));
        lst->bin[lst->size] = data;
        lst->size++;
    }
  
}

/*combine two binary lists*/
void addTwoBinaryLists(binaryList* lst1, binaryList* lst2){
    int i;
    if(!lst2 || !lst1)
        return;
    for(i=0;i<lst2->size;i++){
        addToBinaryList(lst1, lst2->bin[i]);
    }
}

/*gets int and info about linking - return binary data*/
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

/*add : to the end - because external labels are saved with
: at the end*/
char* completeToLabel(char* reference){
    char* res = (char*)malloc((strlen(reference)+2) * sizeof(char));
    strcpy(res, reference);
    strcat(res, ":");   
    return res;
}

/*translate strLiteral to binary - add to dc the amount needed*/
binaryList* strLiteralToBinary(char* strLiteral, int* dc){
    binaryList* res = initBinaryList();
    int size, i;
    char* lineStart = strLiteral, *temp;
    strLiteral = getNextWordStrLiteral(&lineStart);
    if(!strLiteral){
        return NULL;
    }
    temp = getFirstWord(&lineStart);
    if(temp && strlen(temp) > 0){
        return NULL;
    }
    size =  strlen(strLiteral) - 1;
 
    if(strLiteral[0] != strLiteral[size] || strLiteral[0] != '\"'){
        return NULL;
    }
    for(i=1;i<size;i++){
        if((int)strLiteral[i] < 0 || (int)strLiteral[i] > 127){
            return NULL;
        }
    }
    for(i=1;i<size;i++){
        addToBinaryList(res, intToBinary((int)strLiteral[i], A, *dc));
        (*dc)++;
    }
    addToBinaryList(res, intToBinary(0,A,*dc));
    (*dc)++;
    return res;
}

/*check if data is a valid string start with - or + followed by numbers*/
int isValidNumericString(char* number){
    int i,start=0;
    if(!number)
        return 0;
    if(number[0] == '-' || number[0] == '+')
        start=1;
    if(strlen(number) - start <= 0)
        return 0;
    for(i=start;i<strlen(number);i++){
        if(!isNumeric(number[i]))
            return 0;
    }
    return 1;
   
}


/*turns data literal to binary - for after .data
also increase dc by amount needed*/
binaryList* dataLiteralToBinary(char* dataLiteral, int* dc){
    char *buffer;
    int decimalNumber;
    binaryList* res = initBinaryList();
    buffer = getNextWordParams(&dataLiteral);
    while(buffer && strlen(buffer) != 0){
        if(sscanf(buffer, "%d", &decimalNumber) != 1 || !isValidNumericString(buffer)) 
        {   
            return NULL;
        }
        else{
            addToBinaryList(res, intToBinary(decimalNumber, A, *dc));
        }
        (*dc)++;
       
        free(buffer);
        buffer = getNextWordParams(&dataLiteral);
    }
    if(buffer){
        return NULL;
    }
    return res;
}

/*gets operand and returns its address type*/
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

/*add one line to ext file
but doesnt do it only after
function commitExtFile is called 
if there is no need to add because program failed after*/
int writeExtFile(char* label, int pos){
    
    char* buffer = (char*)malloc(24 + strlen(label) + 10);
    sprintf(buffer, "%s %04u \n", label, pos);    
    extFileSize++;
    extFileData = (char**)realloc(extFileData ,sizeof(char*) * extFileSize);
    extFileData[extFileSize-1] = buffer;   
    return 1;

}

/*puts all the data written when use writeExtFile into the ext file*/
int commitExtFile(char* fileName){
    int res = 1, i;
    if(!extFile){
        char* fullFileName = addExtenstionToName(fileName, ".ext");
        extFile = fopen(fullFileName, "w");
    }
    for(i=0;i<extFileSize;i++){
        res = fputs(extFileData[i], extFile) && res;
    }
    
    return res;
}

/*closes the ext file*/
void closeExtFile(){
    if(extFile){
        fclose(extFile);
        extFileSize = 0;
        extFileData = NULL;
        extFile = NULL;
    }
}

/*translate operand to binary struct
uses the label table and writes to ext file if needed*/
binaryData* translateOperand(char* operand, AddressType num, int ic, LabelTable* table, char* fileName){
    int decimalNumber;
    unsigned int res;
    LabelData* temp;
    LinkingInfo info = A;
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
            if(temp && temp->attr == EXTERNAL){
                writeExtFile(temp->name, ic);
                info = E;
            }
            else{
                printf("label: %s doesnt exist mentioned in line %d\n", operand, getCurrLineCouter());
                return NULL;
            }
        }
        if(temp->address - (ic) > (1 << (MAX_BINARY_SIZE-1)) - 1 || temp->address - ic + 1 < -1 *(1 << (MAX_BINARY_SIZE-1))){
            printf("jump out of range\n");
            return NULL;
        }
        decimalNumber = temp->address;
        return intToBinary(decimalNumber - (ic), info, ic);
    }
    else if(num == REGISTER_DIRECT){
        return intToBinary(1 << (int)(operand[1] -'0'), A, ic);
    }
    else if(num == DIRECT){
        operandWithAdder = completeToLabel(operand);
        temp = getLabelFromTable(table, operandWithAdder);
        info = R;
        if(!temp){
            temp = getLabelFromTable(table, operand);
            if(temp && temp->attr == EXTERNAL){
                writeExtFile(temp->name, ic);
                info = E;
            }
            else{
                printf("label: %s doesnt exist mentioned in line %d\n", operand, getCurrLineCouter());
                return NULL;
            }

        }
            
        res = (unsigned int)temp->address;
        
        return intToBinary(res, info, ic);
    }
    return NULL;
}

binaryList* commandToBinary(char* command, char* line , int* ic, LabelTable* table, char* fileName, int checkErrors){
    char* buffer;
    int commandParamNum = getCommandParamNumber(command);
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
        if(commandParamNum <= 0 || i == 2){
            if(checkErrors)
                printf("error in command %s line %d , got too many parameters\n", command,getCurrLineCouter());
            return NULL;
        }
        commandParamNum -= 1;
        if(checkErrors){
            if(getCommandParamNumber(command) == 1){
                 if(!isCommandAllowedAddress(command, getAddressType(buffer), 0)){
                    printf("error in command %s  line %d, parameters mismatch command\n", command ,getCurrLineCouter());
                }
            }
            else{
                if(!isCommandAllowedAddress(command, getAddressType(buffer), i == 0 ? 1 : 0)){
                    printf("error in command %s  line %d, parameters mismatch command\n", command ,getCurrLineCouter());
                }
            }
            commandData = translateOperand(buffer,getAddressType(buffer), *ic, table, fileName);

            if(!commandData)
                return NULL;
            addToBinaryList(res, commandData);
        }
        
        /*addToBinaryList(res, translateOperand(buffer,getAddressType(buffer), *ic, table));*/
        (*ic)++;
        operandAddressType[i] = getAddressType(buffer); 
        
        buffer = getNextWordParams(&line);
        
        i++;
    }
    if(buffer && strlen(buffer) == 0 && getCommandParamNumber(command) != 0 && checkErrors){
        printf("error in command %s line %d, incorrect format\n", command,getCurrLineCouter());
        return NULL;
    }
    if(checkErrors && commandParamNum > 0){
        printf("error in command %s line %d, not enough parameters\n", command, getCurrLineCouter());
        return NULL;
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
