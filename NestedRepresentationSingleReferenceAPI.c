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

struct RCDS_array* recursivelyCopyArray(RCDS_array* copied_array) {
    struct RCDS_array* RC_array = malloc(sizeof(struct RCDS_array));
    if(copied_array->kind == nestedArray) {
        RC_array->referenceCount = IS_NESTED;
        RC_array->kind = copied_array->kind;
        RC_array->length = copied_array->length;
        RC_array->nestedArray = malloc(sizeof(RCDS_array*)*RC_array->length);
        for(size_t i = 0; i < RC_array->length; i++) {
            RC_array->nestedArray[i] = recursivelyCopyArray(copied_array->nestedArray[i]);
        }
        return RC_array;
    }
    else {
        RC_array->referenceCount = IS_NESTED;
        RC_array->kind = copied_array->kind;
        RC_array->length = copied_array->length;
        RC_array->intArray = malloc(sizeof(int)*RC_array->length);
        for(size_t i = 0; i < RC_array->length; i++) {
            RC_array->intArray[i] = copied_array->intArray[i];
        }
        return RC_array;
    }
}

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
    deleted_array = NULL;
}

struct RCDS_array* RCDS_GEN_ARRAY(int referenceC, int kind, size_t arrayLength, size_t valistLength, ...) {
    struct RCDS_array* RC_array = malloc(sizeof(struct RCDS_array));
    if (kind == nestedArray) {
        RCDS_array** valuesArray = malloc(sizeof(RCDS_array*)*arrayLength);
        va_list vl;
        va_start(vl, valistLength);
        struct RCDS_array* tempArray;
        for(int i = 0; i < valistLength && i < arrayLength; i++) {
            tempArray = va_arg(vl, RCDS_array*);
            if(tempArray->referenceCount != 1) {
                valuesArray[i] = recursivelyCopyArray(tempArray);
                RCDS_DEC_RC(tempArray);
            }
            else {
                valuesArray[i] = tempArray;
                valuesArray[i]->referenceCount = IS_NESTED;
            }
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
        RC_array = RC_array->nestedArray[va_arg(vl, size_t)];
    }
    int element = RC_array->intArray[va_arg(vl, size_t)];
    va_end(vl);
    return element;
}

struct RCDS_array* RCDS_MOD_ELEMENT(int referenceC, int value, RCDS_array* RC_array, size_t valistLength, ...) {
    if(RC_array->referenceCount == 1) {
        RC_array->referenceCount = referenceC + 1;
        va_list vl;
        va_start(vl, valistLength);
        for(;valistLength > 1; valistLength--) {
            RC_array = RC_array->nestedArray[va_arg(vl, size_t)];
        }
        RC_array->intArray[va_arg(vl, size_t)] = value;
        va_end(vl);
        return RC_array;
    }
    else {
        struct RCDS_array* new_RC_array = recursivelyCopyArray(RC_array);
        new_RC_array->referenceCount = referenceC;
        va_list vl;
        va_start(vl, valistLength);
        for(;valistLength > 1; valistLength--) {
            new_RC_array = new_RC_array->nestedArray[va_arg(vl, size_t)];
        }
        new_RC_array->intArray[va_arg(vl, size_t)] = value;
        va_end(vl);
        return new_RC_array;
    }
}

struct RCDS_array* RCDS_TAKE_SUBARRAY(int referenceC, RCDS_array* RC_array, size_t valistLength, ...) {
    va_list vl;
    va_start(vl, valistLength);
    for(;valistLength > 0; valistLength--) {
        RC_array = RC_array->nestedArray[va_arg(vl, size_t)];
    }
    va_end(vl);
    struct RCDS_array* subarray = malloc(sizeof(struct RCDS_array));
    subarray = recursivelyCopyArray(RC_array);
    subarray->referenceCount = referenceC;
    return subarray;
}

struct RCDS_array* RCDS_INSERT_SUBARRAY(int referenceC, RCDS_array* subarray, RCDS_array* RC_array, size_t valistLength, ...) {
    if(RC_array->referenceCount == 1) {
        RC_array->referenceCount = referenceC + 1;
        va_list vl;
        va_start(vl, valistLength);
        for(;valistLength > 1; valistLength--) {
            RC_array = RC_array->nestedArray[va_arg(vl, size_t)];
        }
        if(subarray->referenceCount == 1) {
            RC_array->nestedArray[va_arg(vl, size_t)] = subarray;
            subarray->referenceCount = IS_NESTED;
        }
        else {
            RC_array->nestedArray[va_arg(vl, size_t)] = recursivelyCopyArray(subarray);
        }
        va_end(vl);
        return RC_array;
    }
    else {
        struct RCDS_array* new_RC_array = recursivelyCopyArray(RC_array);
        new_RC_array->referenceCount = referenceC;
        va_list vl;
        va_start(vl, valistLength);
        for(;valistLength > 1; valistLength--) {
            new_RC_array = new_RC_array->nestedArray[va_arg(vl, size_t)];
        }
        if(subarray->referenceCount == 1) {
            new_RC_array->nestedArray[va_arg(vl, size_t)] = subarray;
            subarray->referenceCount = IS_NESTED;
        }
        else {
            new_RC_array->nestedArray[va_arg(vl, size_t)] = recursivelyCopyArray(subarray);
        }
        va_end(vl);
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
