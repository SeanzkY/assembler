typedef enum AddressType{
    IMMEDIATE,
    DIRECT,
    RELATIVE,
    REGISTER_DIRECT
}AddressType;


typedef struct addressTypesAllowed{
    AddressType* address;
    int size;
}addressTypesAllowed;


typedef struct CommandData{
    char* command;
    int funct;
    int isFunctExist;
    int opCode;
    addressTypesAllowed* dst;
    addressTypesAllowed* src;
}CommandData;
