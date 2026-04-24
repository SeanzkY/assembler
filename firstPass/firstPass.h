
#define IC_START 100
#define DC_START 0


typedef enum Attribute {
    CODE,
    DATA,
    ENTRY,
    EXTERNAL,
    DATA_AND_ENTRY,
    CODE_AND_ENTRY
    
} LabelType;    


typedef struct LabelData{
    char* name;
    int address;
    LabelType attr;

}LabelData;

typedef struct LabelTable{
    LabelData** labels;
    int size;

}LabelTable;



LabelData* getLabelFromTable(LabelTable* table, char* labelName);

LabelTable* createLabelTable(char* fileName);

