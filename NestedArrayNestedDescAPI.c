#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum RCDS_DATA_TYPES {intArray, charArray, floatArray, doubleArray, RCDS_nestedArray};
typedef struct RCDS_nested RCDS_nested;

struct RCDS_nested {
    int kind;
    size_t length;
    union {
        int* intArray;
        char* charArray;
        float* floatArray;
        double* doubleArray;
        RCDS_nested** RCDS_nestedArray;
    };
};

struct RCDS_array {
    int referenceCount;
    int kind;
    size_t length;
    union {
        int* intArray;
        char* charArray;
        float* floatArray;
        double* doubleArray;
        RCDS_nested** RCDS_nestedArray;
    };
};

#define RCDS_GEN_ARRAY(RCDS_referenceC,RCDS_kind,RCDS_data...) RCDS_GEN_ARRAY1(RCDS_referenceC,RCDS_kind,RCDS_data)
#define RCDS_GEN_ARRAY1(RCDS_referenceC,RCDS_kind,RCDS_data...) ({ \
    struct RCDS_array* RC_array = malloc(sizeof(struct RCDS_array)); \
    if (RCDS_kind##Array == 4) { \
        size_t size = sizeof((RCDS_kind*[]){RCDS_data}); \
        RCDS_kind** valuesArray = malloc(size); \
        memcpy(valuesArray, (RCDS_kind*[]){RCDS_data}, size); \
        *RC_array = (const struct RCDS_array){.referenceCount = (int)RCDS_referenceC, .kind = (int)RCDS_kind##Array, .length = (size_t)size/sizeof(RCDS_kind*), .RCDS_kind##Array = valuesArray}; \
    } \
    else { \
        size_t size = sizeof((RCDS_kind*[]){RCDS_data}); \
        RCDS_kind** valuesArray = malloc(size); \
        memcpy(valuesArray, (RCDS_kind*[]){RCDS_data}, size); \
        *RC_array = (const struct RCDS_array){.referenceCount = (int)RCDS_referenceC, .kind = (int)RCDS_kind##Array, .length = (size_t)size/sizeof(RCDS_kind*), .RCDS_kind##Array = valuesArray}; \
    } \
    RC_array; \
})

#define RCDS_GEN_NESTED_ARRAY(RCDS_kind,RCDS_data...) ({ \
    size_t size = sizeof((RCDS_kind[]){RCDS_data}); \
    RCDS_kind* valuesArray = malloc(size); \
    memcpy(valuesArray, (RCDS_kind[]){RCDS_data}, size); \
    struct RCDS_nested* RC_array = malloc(sizeof(struct RCDS_nested)); \
    *RC_array = (const struct RCDS_nested){.kind = (int)RCDS_kind##Array, .length = (size_t)size/sizeof(RCDS_kind), .RCDS_kind##Array = valuesArray}; \
    RC_array; \
})

#define RCDS_GEN_EMPTY_ARRAY(RCDS_referenceC,RCDS_kind,RCDS_length) ({ \
    RCDS_kind* valuesArray = calloc((size_t)RCDS_length, sizeof(RCDS_kind)); \
    struct RCDS_array* RC_array = malloc(sizeof(struct RCDS_array)); \
    *RC_array = (const struct RCDS_array){.referenceCount = (int)RCDS_referenceC, .kind = (int)RCDS_kind##Array, .length = (size_t)RCDS_length, .RCDS_kind##Array = valuesArray}; \
    RC_array; \
})


int main() {
    printf("hello!\n");
    struct RCDS_array* test1 = RCDS_GEN_ARRAY(2, RCDS_nested, RCDS_GEN_NESTED_ARRAY(int, 1, 2), RCDS_GEN_NESTED_ARRAY(int, 3, 4));
    //struct RCDS_array* test2 = RCDS_GEN_ARRAY(3, int, 2, 4, 23, -23, -348, 0, 5);
    //struct RCDS_array* test3 = RCDS_GEN_EMPTY_ARRAY(2, float, 43);
    printf("referenceC:%d, kind:%d, length:%Iu, data:%d\n", test1->referenceCount, test1->kind, test1->length, test1->RCDS_nestedArray[1]->intArray[0]);
    //printf("referenceC:%d, kind:%d, length:%Iu, data:%d\n", test2->referenceCount, test2->kind, test2->length, test2->intArray[1]);
    return 0;
}
