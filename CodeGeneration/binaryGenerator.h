
typedef struct binaryData{
    char digits[12];
}binaryData;

typedef struct binaryList
{   binaryData** bin;
    int size;
}binaryList;




binaryList* strLiteralToBinary(char* strLiteral, int* dc);


binaryList* dataLiteralToBinary(char* dataLiteral, int* dc);
