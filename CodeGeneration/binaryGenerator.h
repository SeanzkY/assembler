#define MAX_BINARY_SIZE 12

#include "../firstPass/firstPass.h"

typedef struct binaryData{
    char digits[MAX_BINARY_SIZE];
}binaryData;

typedef struct binaryList
{   binaryData** bin;
    int size;
}binaryList;


char* completeToLabel(char* reference);

binaryList* initBinaryList();

void addTwoBinaryLists(binaryList* lst1, binaryList* lst2);

binaryList* strLiteralToBinary(char* strLiteral, int* dc);


binaryList* dataLiteralToBinary(char* dataLiteral, int* dc);


binaryList* commandToBinary(char* command, char* line , int* ic, LabelTable* table);