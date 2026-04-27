#define MAX_BINARY_SIZE 12

#include "../firstPass/firstPass.h"

typedef enum LinkingInfos{
    A='A',
    R='R',
    E='E'
}LinkingInfo;

typedef struct binaryData{
    char digits[MAX_BINARY_SIZE];
    unsigned int pos;
    LinkingInfo info;
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


binaryList* commandToBinary(char* command, char* line , int* ic, LabelTable* table, char* fileName, int checkErrors);

int commitExtFile(char* fileName);

void closeExtFile();