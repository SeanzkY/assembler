
typedef struct binaryData{
    char digits[12];
}binaryData;

typedef struct binaryList
{   binaryData** bin;
    int size;
}binaryList;



binaryList* initBinaryList();

void addTwoBinaryLists(binaryList* lst1, binaryList* lst2);

binaryList* strLiteralToBinary(char* strLiteral, int* dc);


binaryList* dataLiteralToBinary(char* dataLiteral, int* dc);


binaryList* commandToBinary(char* command, char* line , int* ic);