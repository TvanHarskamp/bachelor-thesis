#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "GeneralAPI.h"

struct RCDS_array {
    int referenceCount;
    int kind;
    size_t length;
    union {
        int* intArray;
        RCDS_array** nestedArray;
    };
};

void RCDS_DELETE_ARRAY(RCDS_array* deleted_array) {
    if(deleted_array->kind == nestedArray) {
        for(int i = 0; i < deleted_array->length; i++) {
            if(deleted_array->nestedArray[i]->referenceCount == IS_NESTED) {
                RCDS_DELETE_ARRAY(deleted_array->nestedArray[i]);
            }
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
            valuesArray[i]->referenceCount = IS_NESTED;
        }
        va_end(vl);
        *RC_array = (const struct RCDS_array){.referenceCount = referenceC, .kind = kind, .length = arrayLength, .nestedArray = valuesArray};
    }
    else {
        int* valuesArray = malloc(sizeof(int)*arrayLength);
        va_list vl;
        va_start(vl, valistLength);
        for(int i = 0; i < valistLength && i < arrayLength; i++) {
            valuesArray[i] = va_arg(vl, int);
        }
        va_end(vl);
        *RC_array = (const struct RCDS_array){.referenceCount = referenceC, .kind = kind, .length = arrayLength, .intArray = valuesArray};
    }
    return RC_array;
}

struct RCDS_array* RCDS_GEN_EMPTY_ARRAY(int referenceC, int kind, size_t arrayLength) {
    struct RCDS_array* RC_array = malloc(sizeof(struct RCDS_array));
    int* valuesArray = calloc(arrayLength, sizeof(int));
    *RC_array = (const struct RCDS_array){.referenceCount = referenceC, .kind = kind, .length = arrayLength, .intArray = valuesArray};
    return RC_array;
}

void RCDS_INC_RC(RCDS_array* RC_array) {
    if(RC_array->referenceCount != IS_NESTED) {
        RC_array->referenceCount++;
    }
}

void RCDS_DEC_RC(RCDS_array* RC_array) {
    if(RC_array->referenceCount != IS_NESTED) {
        RC_array->referenceCount--;
        if(RC_array->referenceCount == 0) {
            RCDS_DELETE_ARRAY(RC_array);
        }
    }
}

int RCDS_SELECT_ELEMENT(RCDS_array* RC_array, size_t valistLength, ...) {
    va_list vl;
    va_start(vl, valistLength);
    for(;valistLength > 1; valistLength--) {
        RC_array = RC_array->nestedArray[va_arg(vl, int)];
    }
    int element = RC_array->intArray[va_arg(vl, int)];
    va_end(vl);
    return element;
}

void RCDS_MOD_ELEMENT(int value, RCDS_array* RC_array, size_t valistLength, ...) {
    va_list vl;
    va_start(vl, valistLength);
    for(;valistLength > 1; valistLength--) {
        RC_array = RC_array->nestedArray[va_arg(vl, int)];
    }
    RC_array->intArray[va_arg(vl, int)] = value;
    va_end(vl);
}

struct RCDS_array* RCDS_SELECT_SUBARRAY(int referenceC, RCDS_array* RC_array, size_t valistLength, ...) {
    int hasOneReference = 0;
    if(RC_array->referenceCount == 1) {
        hasOneReference = 1;
    }
    va_list vl;
    va_start(vl, valistLength);
    for(;valistLength > 0; valistLength--) {
        RC_array = RC_array->nestedArray[va_arg(vl, int)];
    }
    va_end(vl);
    //The top array that holds the RC_array will be deleted after this function if it's reference count
    //is 1, so we can just use the selected subarray with a new reference count if that is the case.
    if(hasOneReference == 1) {
        RC_array->referenceCount = referenceC;
        return RC_array;
    }
    else {
        struct RCDS_array* new_RC_array = malloc(sizeof(struct RCDS_array));
        *new_RC_array = (const struct RCDS_array)
            {.referenceCount = referenceC,
            .kind = RC_array->kind,
            .length = RC_array->length};
        if(RC_array->kind == nestedArray) {
            new_RC_array->nestedArray = RC_array->nestedArray;
        } else {
            new_RC_array->intArray = RC_array->intArray;
        }
        return new_RC_array;
    }
}

void RCDS_PRINT_ARRAY(RCDS_array* printed_array) {
    if(printed_array->kind == nestedArray) {
        printf("Reference count: %d, array contains:\n", printed_array->referenceCount);
        for(size_t i = 0; i < printed_array->length; i++) {
            printf("\t");
            RCDS_PRINT_ARRAY(printed_array->nestedArray[i]);
        }
        printf("\n");
    } else {
        printf("\tReference count: %d, valuesArray: [", printed_array->referenceCount);
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
