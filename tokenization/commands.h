typedef enum AddressType{
    IMMEDIATE,
    DIRECT,
    RELATIVE,
    REGISTER_DIRECT,
    EMPTY_ADDRESS
}AddressType;


typedef struct addressTypesAllowed{
    AddressType* address;
    int size;
}addressTypesAllowed;


typedef struct CommandData{
    char* command;
    int funct;
    int opCode;
    AddressType src[4];
    AddressType dst[4];
}CommandData;
