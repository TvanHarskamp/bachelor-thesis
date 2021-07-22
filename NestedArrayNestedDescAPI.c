#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "GeneralAPI.h"

#define IS_NESTED -1000
enum RCDS_DATA_TYPES {intArray, charArray, floatArray, doubleArray, nestedArray};
typedef struct RCDS_array RCDS_array;

struct RCDS_array {
    int kind;
    size_t length;
    union {
        int* intArray;
        RCDS_array** nestedArray;
    };
    int referenceCount;
};

void RCDS_DELETE_ARRAY(RCDS_array* deleted_array) {
    if(deleted_array->kind == nestedArray) {
        for(int i = 0; i < deleted_array->length; i++) {
            RCDS_DELETE_ARRAY(deleted_array->nestedArray[i]);
        }
        free(deleted_array->nestedArray);
    } else {
        free(deleted_array->intArray);
    }
    free(deleted_array);
}

struct RCDS_array* RCDS_GEN_ARRAY(int referenceC, int kind, size_t arrayLength, size_t valistLength, ...) {
    struct RCDS_array* RC_array = malloc(sizeof(struct RCDS_array));
    if (kind == nestedArray) {
        RCDS_array** valuesArray = malloc(sizeof(RCDS_array*)*arrayLength);
        va_list vl;
        va_start(vl, valistLength);
        for(int i = 0; i < valistLength && i < arrayLength; i++) {
            valuesArray[i] = va_arg(vl, RCDS_array*);
        }
        va_end(vl);
        *RC_array = (const struct RCDS_array){.kind = kind, .length = arrayLength, .nestedArray = valuesArray, .referenceCount = referenceC};
    }
    else {
        int* valuesArray = malloc(sizeof(int)*arrayLength);
        va_list vl;
        va_start(vl, valistLength);
        for(int i = 0; i < valistLength && i < arrayLength; i++) {
            valuesArray[i] = va_arg(vl, int);
        }
        va_end(vl);
        *RC_array = (const struct RCDS_array){.kind = kind, .length = arrayLength, .intArray = valuesArray, .referenceCount = referenceC};
    }
    return RC_array;
}

struct RCDS_array* RCDS_GEN_EMPTY_ARRAY(int referenceC, int kind, size_t arrayLength) {
    struct RCDS_array* RC_array = malloc(sizeof(struct RCDS_array));
    int* valuesArray = calloc(arrayLength, sizeof(int));
    *RC_array = (const struct RCDS_array){.referenceCount = referenceC, .kind = kind, .length = arrayLength, .intArray = valuesArray};
    return RC_array;
}

int* RCDS_SELECT_ELEMENT(RCDS_array* RC_array, size_t valistLength, ...) {
    va_list vl;
    va_start(vl, valistLength);
    for(;valistLength > 1; valistLength--) {
        RC_array = RC_array->nestedArray[va_arg(vl, int)];
    }
    int* element = RC_array->intArray + va_arg(vl, int);
    va_end(vl);
    return element;
}

void RCDS_PRINT_ARRAY(RCDS_array* printed_array) {
    if(printed_array->kind == nestedArray) {
        printf("Array contains:\n");
        for(size_t i = 0; i < printed_array->length; i++) {
            printf("\t");
            RCDS_PRINT_ARRAY(printed_array->nestedArray[i]);
        }
        printf("\n");
    } else {
        printf("\tvaluesArray: [");
        for(size_t i = 0; i < printed_array->length-1; i++) {
            printf("%d, ", printed_array->intArray[i]);
        }
        if(printed_array->length-1 >= 0) {
            printf("%d]\n", printed_array->intArray[printed_array->length-1]);
        } else {
            printf("]\n");
        }
    }
}

int main() {
    printf("hello!\n");
    struct RCDS_array* test1 = RCDS_GEN_ARRAY(2, nestedArray, 2, 2,
        RCDS_GEN_ARRAY(IS_NESTED, nestedArray, 2, 2,
            RCDS_GEN_ARRAY(IS_NESTED, intArray, 13, 3, 1, 2, 3),
            RCDS_GEN_ARRAY(IS_NESTED, intArray, 4, 4, 4, 5, 6, 7)
        ),
        RCDS_GEN_ARRAY(IS_NESTED, nestedArray, 3, 3,
            RCDS_GEN_ARRAY(IS_NESTED, intArray, 5, 5, 8, 9, 10, 11, 12),
            RCDS_GEN_ARRAY(IS_NESTED, intArray, 2, 2, 13, 14),
            RCDS_GEN_ARRAY(IS_NESTED, intArray, 1, 1, 15)
        )
    );
    struct RCDS_array* test2 = RCDS_GEN_ARRAY(3, intArray, 10, 5, 23, -23, -348, 0, 5);
    struct RCDS_array* test3 = RCDS_GEN_EMPTY_ARRAY(2, intArray, 30);
    RCDS_PRINT_ARRAY(test1);
    RCDS_PRINT_ARRAY(test2);
    RCDS_PRINT_ARRAY(test3);
    printf("\nSelection of [1][0][3] in test1: %d\n", *RCDS_SELECT_ELEMENT(test1, 3, 1, 0, 3));
    printf("\nSelection of [4] in test2: %d\n", *RCDS_SELECT_ELEMENT(test2, 1, 4));
    return 0;
}
