#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "GeneralAPI.h"

struct RCDS_array {
    int kind;
    size_t length;
    union {
        int* intArray;
        RCDS_array** nestedArray;
    };
    int referenceCount;
};

size_t RCDS_TOTAL_LENGTH(RCDS_array* RC_array) {
    if(RC_array->kind == nestedArray) {
        size_t totalLength = 0;
        for(size_t i = 0; i < RC_array->length; i++) {
            totalLength += RCDS_TOTAL_LENGTH(RC_array->nestedArray[i]);
        }
        return totalLength;
    }
    else {
        return RC_array->length;
    }
}

size_t RCDS_SUBARRAY_LENGTH(RCDS_array* RC_array, size_t valistLength, ...) {
    va_list vl;
    va_start(vl, valistLength);
    for(size_t i = 0; i < valistLength; i++) {
        RC_array = RC_array->nestedArray[va_arg(vl, size_t)];
    }
    va_end(vl);
    return RC_array->length;
}

void RCDS_DELETE_ARRAY(RCDS_array* deleted_array) {
    if(deleted_array->kind == nestedArray) {
        free(deleted_array->nestedArray);
    } else {
        free(deleted_array->intArray);
    }
    free(deleted_array);
}

void recursivelyIncreaseRC(RCDS_array* RC_array, int increase) {
    RC_array->referenceCount += increase;
    if(RC_array->kind == nestedArray) {
        for(size_t i = 0; i < RC_array->length; i++) {
            recursivelyIncreaseRC(RC_array->nestedArray[i], increase);
        }
    }
}

struct RCDS_array* RCDS_GEN_ARRAY(int referenceC, int kind, size_t arrayLength, size_t valistLength, ...) {
    struct RCDS_array* RC_array = malloc(sizeof(struct RCDS_array));
    if (kind == nestedArray) {
        RCDS_array** valuesArray = malloc(sizeof(RCDS_array*)*arrayLength);
        va_list vl;
        va_start(vl, valistLength);
        for(int i = 0; i < valistLength && i < arrayLength; i++) {
            valuesArray[i] = va_arg(vl, RCDS_array*);
            recursivelyIncreaseRC(valuesArray[i], referenceC);
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

void RCDS_INC_RC(RCDS_array* RC_array) {
    if(RC_array->kind == nestedArray) {
        for(size_t i = 0; i < RC_array->length; i++) {
            RCDS_INC_RC(RC_array->nestedArray[i]);
        }
    }
    RC_array->referenceCount++;
}

void RCDS_DEC_RC(RCDS_array* RC_array) {
    if(RC_array->kind == nestedArray) {
        for(size_t i = 0; i < RC_array->length; i++) {
            RCDS_DEC_RC(RC_array->nestedArray[i]);
        }
    }
    RC_array->referenceCount--;
    if(RC_array->referenceCount == 0) {
        RCDS_DELETE_ARRAY(RC_array);
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

struct RCDS_array* RCDS_TAKE_SUBARRAY(int referenceC, RCDS_array* RC_array, size_t valistLength, ...) {
    va_list vl;
    va_start(vl, valistLength);
    for(;valistLength > 0; valistLength--) {
        RC_array = RC_array->nestedArray[va_arg(vl, int)];
    }
    va_end(vl);
    //The subarray has it's own reference count, so if the reference count is 1,
    //the subarray can just be taken with the new reference count (plus 1,
    //since the reference count will be decreased by 1 after this function)
    if(RC_array->referenceCount == 1) {
        recursivelyIncreaseRC(RC_array, referenceC);
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
