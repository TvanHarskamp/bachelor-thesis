#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum RCDS_DATA_TYPES {intArray, charArray, floatArray, doubleArray, nestedArray};
#define OPEN_SQUARE_BRACKET -10000
#define CLOSE_SQUARE_BRACKET -10001

struct RCDS_array {
    int referenceCount;
    int kind;
    size_t depth;
    size_t* segmentShape;
    size_t** segments;
    union {
        int* intArray;
        char* charArray;
        float* floatArray;
        double* doubleArray;
    };
};

struct RCDS_nested_array {
    int kind;
    size_t depth;
    size_t* segmentShape;
    size_t** segments;
    union {
        int* intArray;
        char* charArray;
        float* floatArray;
        double* doubleArray;
    };
};

#define RCDS_GEN_ARRAY(RCDS_referenceC,RCDS_kind,RCDS_data...) ({ \
    struct RCDS_array* RC_array = malloc(sizeof(struct RCDS_array)); \
    if(RCDS_kind##Array == 4) { \
        printf("cool\n"); \
    } else { \
        size_t size = sizeof((RCDS_kind[]){RCDS_data}); \
        RCDS_kind* valuesArray = malloc(size); \
        memcpy(valuesArray, (RCDS_kind[]){RCDS_data}, size); \
        size_t* segmentShape = malloc(sizeof(size_t)); \
        segmentShape[0] = 1; \
        size_t** segments = malloc(sizeof(size_t*)); \
        segments[0] = malloc(sizeof(size_t*)); \
        segments[0][0] = size; \
        *RC_array = (const struct RCDS_array) {.referenceCount = RCDS_referenceC, .kind = (int)RCDS_kind##Array, .depth = 1, .segmentShape = segmentShape, .segments = segments, .RCDS_kind##Array = valuesArray}; \
    } \
    RC_array; \
})

#define RCDS_GEN_NESTED_ARRAY(RCDS_kind,RCDS_data...) ({ \
    if(RCDS_kind##Array == 4) { \
        printf("cool\n"); \
    } else { \
        RCDS_kind, OPEN_SQUARE_BRACKET, RCDS_data, CLOSE_SQUARE_BRACKET; \
    } \
})

#define RCDS_GEN_EMPTY_ARRAY(RCDS_referenceC,RCDS_kind,RCDS_length) ({ \
    RCDS_kind* valuesArray = calloc((size_t)RCDS_length, sizeof(RCDS_kind)); \
    struct RCDS_array* RC_array = malloc(sizeof(struct RCDS_array)); \
    *RC_array = (const struct RCDS_array){.referenceCount = (int)RCDS_referenceC, .kind = (int)RCDS_kind##Array, .depth = (size_t)RCDS_length, .RCDS_kind##Array = valuesArray}; \
    RC_array; \
})

int main() {
    printf("hello!\n");
    struct RCDS_array* test1 = RCDS_GEN_ARRAY(3, int, 2, 4, 23, -23, -348, 0, 5);
    struct RCDS_array* test2 = RCDS_GEN_EMPTY_ARRAY(2, float, 43);
    printf("referenceC:%d, kind:%d, data:%d\n", test1->referenceCount, test1->kind, test1->intArray[4]);
    return 0;
}
